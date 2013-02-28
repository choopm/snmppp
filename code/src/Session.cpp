// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <stdlib.h>
#include <sstream>
#include <stdexcept>
#include <SNMPpp/Session.hpp>


void SNMPpp::openSession( SNMPpp::SessionHandle &sessionHandle, const std::string &server, const std::string &community, const int version, const int retryAttempts )
{
	// make sure you call closeSession() to free up the handle before calling
	// openSession() because we're about to overwrite any previous handles
	sessionHandle = NULL;

	netsnmp_session session = {0};
	snmp_sess_init( &session );

	session.version			= version;
	session.retries			= retryAttempts;
	session.peername			= (char*)server.c_str();
	session.community		= (unsigned char*)community.c_str();
	session.community_len	= community.size();

	sessionHandle = snmp_sess_open( &session );
	if ( sessionHandle == NULL || snmp_sess_session( sessionHandle ) == NULL )
	{
		// Don't use snmp_sess_error() if the problem is with snmp_sess_open()!
		// Instead, fall back to the traditional snmp_error() and pass in the
		// original netsnmp_session structure.  See the man pages for
		// snmp_sess_error() and snmp_error() for details.
		int error1 = 0;
		int error2 = 0;
		char *msg  = NULL;
		snmp_error( &session, &error1, &error2, &msg );

		std::stringstream ss;
		ss	<< "Failed to open SNMP session to \"" << session.peername << "\". ["
			<< "e1=" << error1 << ", "
			<< "e2=" << error2;
		if ( msg != NULL && msg[0] != '\0' )
		{
			ss << ", " << msg;
		}
		ss << "]";

		free( msg );
		/// @throw std::runtime_error if snmp_sess_open() fails to return a valid new session.
		throw std::runtime_error( ss.str() );
	}

	return;
}


void SNMPpp::closeSession( SNMPpp::SessionHandle &sessionHandle )
{
	if ( sessionHandle )
	{
		snmp_sess_close( sessionHandle );
		sessionHandle = NULL;
	}

	return;
}
