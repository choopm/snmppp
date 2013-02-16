// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <assert.h>
#include <iostream>
#include <SNMPpp/net-snmppp.hpp>


void checkSession( const std::string &server, const bool expectThrow = false )
{
	std::cout << "Setting up a session to \"" << server << "\"." << std::endl;

	// opening a session can throw, so be ready to catch
	try
	{
		SNMPpp::SessionHandle handle = NULL;
		SNMPpp::openSession( handle, server, "public" );
		assert( handle != NULL );

		// once done with a session handle, it must be closed
		SNMPpp::closeSession( handle );
		assert( handle == NULL );
		assert( expectThrow == false );
	}
	catch ( const std::exception &e )
	{
		std::cout << e.what() << std::endl;
		assert( expectThrow );
		std::cout << "(...an exception was expected for this previous one.)" << std::endl;
	}

	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Test some of the net-snmp session helper functionality." << std::endl;

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
