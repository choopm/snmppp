// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <iostream>
#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/Trap.hpp>


int main( int argc, char *argv[] )
{
	std::cout << "Example code showing how to send a trap." << std::endl;

	// Remember that you must be a net-snmp agent to send out traps.  This
	// means if your application isn't already calling the net-snmp API to
	// initialize itself as AgentX, then you'll want to call
	// SNMPpp::initializeNetSnmpAgent() when your application starts.
	// Otherwise, a C++ exception will be thrown.

	std::cout << "\tInitializing AgentX..." << std::endl;
	SNMPpp::netsnmpLogSyslog();
	SNMPpp::initializeNetSnmpAgent();

	// This is the simple v2 trap we want to send out:
	const SNMPpp::OID o( ".1.3.6.1.4.1.38322.1.0.1" );

	std::cout << "\tSending " << o << "..." << std::endl;
	SNMPpp::sendV2Trap( o );

	// and only once we're done and our app is shutting down (or when we're
	// finished with SNMP) do we de-initialize the net-snmp AgentX code
	SNMPpp::shutdownNetSnmpAgent();

	std::cout << "\t...done!" << std::endl;

	return 0;
}
