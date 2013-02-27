// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <stdexcept>
#include <assert.h>
#include <iostream>
#include <SNMPpp/Session.hpp>


void checkSession( const std::string &server, const bool expectThrow = false )
{
	std::cout << "Setting up a session to \"" << server << "\"." << std::endl;

	// opening a session can throw, so be ready to catch
	SNMPpp::SessionHandle sessionHandle = NULL;
	try
	{
		SNMPpp::openSession( sessionHandle, server, "public" );
		assert( sessionHandle != NULL );

		// once done with a session handle, it must be closed
		SNMPpp::closeSession( sessionHandle );
		assert( sessionHandle == NULL );
		assert( expectThrow == false );
	}
	catch ( const std::exception &e )
	{
		if ( ! expectThrow )
		{
			std::cout << e.what() << std::endl;
		}
		assert( sessionHandle == NULL );
		assert( expectThrow );
	}

	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Test the session handle." << std::endl;
	SNMPpp::netsnmpLogSyslog();

	// UDP ones are unlikely to throw, since there is no SYN we wont know if it fails until we try to use it
	checkSession( "udp:127.0.0.1:161" );

	// but TCP sessions are likely to fail since they're immediately setup
	checkSession( "tcp:127.0.0.1:161" );

	// and if that previous one doesn't fail, then force a failure with this bad IP address
	checkSession( "tcp:0.1.2.3:161", true );

	checkSession( "foobar", true );

	checkSession( "udp6:[::1]:161" );	// loopback IPv6 address

	return 0;
}
