// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <iostream>
#include <SNMPpp/SNMPpp.hpp>


int main( int argc, char *argv[] )
{
	std::cout	<< "Test the #include dependencies." << std::endl
				<< "With a single #include <SNMPpp/SNMPpp.hpp> we should have access to everything." << std::endl
				<< "VER=" << LIBSNMPPPVER << std::endl;

	SNMPpp::SessionHandle handle;
	SNMPpp::openSession( handle, "udp:localhost:161" );

	SNMPpp::OID o( ".1" );
	SNMPpp::PDU p( SNMPpp::PDU::kGetNext );
	p = SNMPpp::getNext( handle, p );

	p.free();
	
	SNMPpp::closeSession( handle );

	std::cout << "Done!" << std::endl;

	return 0;
}
