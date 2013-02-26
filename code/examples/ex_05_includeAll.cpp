// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <iostream>
#include <SNMPpp/SNMPpp.hpp>


int main( int argc, char *argv[] )
{
	std::cout	<< "By including a single file, all of the SNMPpp functionality is available." << std::endl
				<< "VER=" << LIBSNMPPPVER << std::endl;

	// net-snmp defaults to use stderr; switch to syslog
	SNMPpp::netsnmpLogSyslog();

	SNMPpp::SessionHandle sessionHandle;
	SNMPpp::openSession( sessionHandle, "udp:localhost:161" );

	SNMPpp::OID o( ".1" );
	SNMPpp::PDU pdu = SNMPpp::getNext( sessionHandle, o );

	// for debug purposes, dump out the PDU
	std::cout << pdu << std::endl;

	pdu.free();
	
	SNMPpp::closeSession( sessionHandle );

	// sending out traps needs AgentX, not session handles
	SNMPpp::initializeNetSnmpAgent();
	SNMPpp::OID trap( ".1.3.6.1.4.1.38322.1.0.1" );
	std::cout << "Sending out trap: " << trap << std::endl;
	SNMPpp::sendV2Trap( trap );
	SNMPpp::shutdownNetSnmpAgent();
	
	std::cout << "Done!" << std::endl;

	return 0;
}
