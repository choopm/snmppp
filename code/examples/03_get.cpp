// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <assert.h>
#include <iostream>
#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/Get.hpp>


void exampleGetNext( SNMPpp::SessionHandle &handle )
{
	std::cout << "Perform several iterations of \"getnext\" to walk through the MIB:" << std::endl;
	
	SNMPpp::PDU pdu( SNMPpp::PDU::kGetNext );
	pdu.addNullVar( SNMPpp::OID::kInternet );	// start with this OID

	for ( size_t idx = 0; idx < 10; idx ++ )
	{
		// keep calling "getNext" using whatever OID we last received
		pdu = SNMPpp::getNext( handle, pdu );

		// for debugging, display the OID and the value we retrieved from the SNMP server
		std::cout << pdu;
	}

	// always remember to free the PDU once done with it
	pdu.free();

	return;
}


void example1ManyOidsAtOnce( SNMPpp::SessionHandle &handle )
{
	std::cout << "Perform a single \"get\" with multiple OIDs at once (#1):" << std::endl;

	// create a request PDU and add to it all the OIDs we want
	SNMPpp::PDU pdu( SNMPpp::PDU::kGet );
	pdu.addNullVar( "1.3.6.1.2.1.1.1.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.2.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.3.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.4.0" );

	pdu = SNMPpp::get( handle, pdu );

	// for debugging, display the OID and the value we retrieved from the SNMP server
	std::cout << pdu;
	
	// always remember to free the PDU once done with it
	pdu.free();

	return;
}


void example2ManyOidsAtOnce( SNMPpp::SessionHandle &handle )
{
	std::cout << "Perform a single \"get\" with multiple OIDs at once (#2):" << std::endl;

	// if there are many OIDs we know are needed, they can all be retrieved at the same time
	SNMPpp::SetOID oids;
	oids.insert( ".1.3.6.1.2.1.1.1.0" );
	oids.insert( ".1.3.6.1.2.1.1.2.0" );
	oids.insert( ".1.3.6.1.2.1.1.3.0" );
	oids.insert( ".1.3.6.1.2.1.1.4.0" );
	oids.insert( ".1.3.6.1.2.1.1.5.0" );
	oids.insert( ".1.3.6.1.2.1.1.6.0" );
	oids.insert( ".1.3.6.1.2.1.1.7.0" );
	oids.insert( ".1.3.6.1.2.1.1.8.0" );

	SNMPpp::PDU pdu = SNMPpp::get( handle, oids );

	// for debugging, display the OID and the value we retrieved from the SNMP server
	std::cout << pdu;

	// always remember to free the PDU once done with it
	pdu.free();

	return;
}


void exampleGetSingleOid( SNMPpp::SessionHandle &handle )
{
	std::cout << "Perform a get with a specific OID:" << std::endl;

	SNMPpp::OID o = "1.3.6.1.2.1.1.1.0";
	SNMPpp::PDU pdu = SNMPpp::get( handle, o );
	
	std::cout 	<< pdu
				<< "\tIf we know the type of an OID, then we can retrieve the value directly from the PDU:" << std::endl
				<< "\t" << o << ":  " << pdu.getString(o) << std::endl;

	// always remember to free the PDU once done with it
	pdu.free();
	
	return;
}


void example1GetBulk( SNMPpp::SessionHandle &handle )
{
	std::cout << "Perform a getbulk (SNMPv2) with a single starting OID:" << std::endl;

	SNMPpp::PDU pdu( SNMPpp::PDU::kGetBulk );
	pdu.addNullVar( ".1" );

	// note the number of bulk requests to try and perform, meaning it will get
	// twenty consecutive values unless it gets to the end of the tree, or the
	// maximum packet size has been exceeded
	pdu = SNMPpp::getBulk( handle, pdu, 20 );

	// for debugging, display the OID and the value we retrieved from the SNMP server
	std::cout << pdu;
	
	// always remember to free the PDU once done with it
	pdu.free();

	return;
}


void example2GetBulk( SNMPpp::SessionHandle &handle )
{
	std::cout << "Perform a getbulk (SNMPv2) with multiple starting OIDs:" << std::endl;

	SNMPpp::SetOID s;
	s.insert( "1.3.6.1.2.1" );
	s.insert( "1.3.6.1.2.1.25.4.2.1.4" );
	s.insert( "1.3.6.12.1.31.1.5" );

	// create a PDU and add the entire set of OIDs to it
	SNMPpp::PDU pdu( SNMPpp::PDU::kGetBulk );
	pdu.addNullVar( s );

	pdu = SNMPpp::getBulk( handle, pdu, 5 ); // the reply PDU should have 5 OIDs for each of the starting locations, for a total of 15 OIDs

	// for debugging, display the OID and the value we retrieved from the SNMP server
	std::cout << pdu;
	
	// always remember to free the PDU once done with it
	pdu.free();
	
	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Test some of the net-snmp GET and GETNEXT functionality." << std::endl;

	SNMPpp::SessionHandle handle;
	SNMPpp::openSession( handle, "udp:localhost:161" );

	exampleGetNext			( handle );
	example1ManyOidsAtOnce	( handle );
	example2ManyOidsAtOnce	( handle );
	exampleGetSingleOid		( handle );
	example1GetBulk			( handle );
	example2GetBulk			( handle );

	SNMPpp::closeSession( handle );
	
	return 0;
}
