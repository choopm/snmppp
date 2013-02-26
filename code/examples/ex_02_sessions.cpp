// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <iostream>
#include <SNMPpp/Session.hpp>


int main( int argc, char *argv[] )
{
	std::cout << "Example code showing how to use SNMPpp::SessionHandle." << std::endl;

	// opening a session can throw, so you may need to catch if you want to be complete
	SNMPpp::SessionHandle sessionHandle = NULL;
	try
	{
		SNMPpp::openSession( sessionHandle, "udp:127.0.0.1:161", "public" );
	}
	catch ( const std::exception &e )
	{
		// nicely abort instead?
		std::cout << e.what() << std::endl;
		throw;
	}

	/* *********************************************************************
	 * Now your application runs here, using the sessionHandle for calls to
	 * SNMPpp.  Once done, you need to remember to close the session handle.
	 * *********************************************************************
	 */

	SNMPpp::closeSession( sessionHandle );

	return 0;
}
