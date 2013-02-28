// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <iostream>
#include <SNMPpp/Get.hpp>


void exampleGetNext( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Perform several iterations of \"getnext\" to walk through the MIB:" << std::endl;
	SNMPpp::PDU pdu( SNMPpp::PDU::kGetNext );
	pdu.addNullVar( SNMPpp::OID::kInternet );	// start with this OID
	
	for ( size_t idx = 0; idx < 10; idx ++ )
	{
		// keep calling "getNext" using whatever OID we last received
		pdu = SNMPpp::getNext( sessionHandle, pdu );
		
		// for debugging, display the OID and the value we retrieved from the SNMP server
		std::cout << pdu;
	}

	// always remember to free the PDU once done with it
	pdu.free();

	return;
}


void example1ManyOidsAtOnce( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Perform a single \"GET\" with multiple OIDs:" << std::endl;

	// create a request PDU and add to it all the OIDs we want
	SNMPpp::PDU pdu( SNMPpp::PDU::kGet );
	pdu.addNullVar( "1.3.6.1.2.1.1.1.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.2.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.3.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.4.0" );

	pdu = SNMPpp::get( sessionHandle, pdu );

	// for debugging, display the OID and the value we retrieved from the SNMP server
	std::cout << pdu;
	
	// always remember to free the PDU once done with it
	pdu.free();

	return;
}


void example2ManyOidsAtOnce( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Perform a single \"GET\" (using a std::set) with multiple OIDs:" << std::endl;

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

	SNMPpp::PDU pdu = SNMPpp::get( sessionHandle, oids );

	// for debugging, display the OID and the value we retrieved from the SNMP server
	std::cout << pdu;

	// always remember to free the PDU once done with it
	pdu.free();

	return;
}


void exampleGetSingleOid( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Perform a \"GET\" with a specific OID:" << std::endl;

	SNMPpp::OID o = "1.3.6.1.2.1.1.1.0";
	SNMPpp::PDU pdu = SNMPpp::get( sessionHandle, o );

	std::cout 	<< pdu
				<< "\tIf we know the type of an OID, then we can retrieve the value directly from the PDU:" << std::endl
				<< "\t" << o << ":  " << pdu.varlist().getString(o) << std::endl;

	// always remember to free the PDU once done with it
	pdu.free();
	
	return;
}


void example1GetBulk( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Perform a GETBULK (SNMPv2) with a single starting OID:" << std::endl;

	SNMPpp::PDU pdu( SNMPpp::PDU::kGetBulk );
	pdu.addNullVar( ".1" );

	// note the number of bulk requests to try and perform, meaning it will get
	// twenty consecutive values unless it gets to the end of the tree, or the
	// maximum packet size has been reached
	pdu = SNMPpp::getBulk( sessionHandle, pdu, 20 );

	// for debugging, display the OID and the value we retrieved from the SNMP server
	std::cout << pdu;
	
	// always remember to free the PDU once done with it
	pdu.free();

	return;
}


void example2GetBulk( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Perform a GETBULK (SNMPv2) with multiple starting OIDs:" << std::endl;

	SNMPpp::SetOID s;
	s.insert( "1.3.6.1.2.1" );
	s.insert( "1.3.6.1.2.1.25.4.2.1.4" );
	s.insert( "1.3.6.12.1.31.1.5" );

	// create a PDU and add the entire set of OIDs to it
	SNMPpp::PDU pdu( SNMPpp::PDU::kGetBulk );
	pdu.addNullVars( s );

	pdu = SNMPpp::getBulk( sessionHandle, pdu, 5 ); // the reply PDU should have 5 OIDs for each of the starting locations, for a total of 15 OIDs

	// for debugging, display the OID and the value we retrieved from the SNMP server
	std::cout << pdu;
	
	// always remember to free the PDU once done with it
	pdu.free();
	
	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Example code showing several different ways to make GET/GETNEXT/GETBULK requests." << std::endl;

	SNMPpp::SessionHandle sessionHandle;
	SNMPpp::openSession( sessionHandle, "udp:localhost:161" );

	exampleGetNext			( sessionHandle );
	example1ManyOidsAtOnce	( sessionHandle );
	example2ManyOidsAtOnce	( sessionHandle );
	exampleGetSingleOid		( sessionHandle );
	example1GetBulk			( sessionHandle );
	example2GetBulk			( sessionHandle );

	SNMPpp::closeSession( sessionHandle );

	return 0;
}
