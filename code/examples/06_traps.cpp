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
	
	// Remember that *YOU MUST* be a net-snmp agent to send out traps.
	// This means if your application isn't already calling the net-snmp
	// API to initialize itself as AgentX, then you'll want to call
	// SNMPpp::initializeNetSnmpAgent() when your application starts.
	// Otherwise, a C++ exception will be thrown.  Like this one:
	try
	{
		std::cout << "\tSending " << o << " (but expect this to fail since AgentX is not initialized)" << std::endl;
		SNMPpp::sendV2Trap( o );

		assert( false ); // we should never get here since sendV2Trap() will throw a C++ exception
	}
	catch ( const std::runtime_error &e )
	{
		std::cout	<< "\tTRAP NOT SENT!" << std::endl
					<< "\tException caught: \"" << e.what() << "\"" << std::endl;
	}

	std::cout << "\tInitializing AgentX..." << std::endl;
	SNMPpp::initializeNetSnmpAgent();
	std::cout << "\tSending " << o << "..." << std::endl;
	SNMPpp::sendV2Trap( o );

	// and only once we're done and our app is shutting down (or when we're
	// finished with SNMP) do we de-initialize the net-snmp AgentX code
	SNMPpp::shutdownNetSnmpAgent();

	std::cout << "\t...done!" << std::endl;

	return 0;
}
