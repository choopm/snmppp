// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <iostream>
#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/Trap.hpp>
#include <stdexcept>
#include <assert.h>


int main( int argc, char *argv[] )
{
	SNMPpp::netsnmpLogSyslog();
	std::cout << "Test sending traps." << std::endl;

	// This is the simple v2 trap we want to send out:
	const SNMPpp::OID o( ".1.3.6.1.4.1.38322.1.0.1" );
	
	try
	{
		// this should fail since AgentX is not initialized
		SNMPpp::sendV2Trap( o );

		assert( false ); // we should never get here since sendV2Trap() will throw a C++ exception
	}
	catch ( const std::runtime_error &e )
	{
		// do nothing, we expected sendV2Trap() to fail
	}

	std::cout << "\tInitializing AgentX..." << std::endl;
	SNMPpp::initializeNetSnmpAgent();	
	std::cout << "\tSending " << o << "..." << std::endl;
	SNMPpp::sendV2Trap( o );
	SNMPpp::shutdownNetSnmpAgent();

	std::cout << "\t...done!" << std::endl;

	return 0;
}
