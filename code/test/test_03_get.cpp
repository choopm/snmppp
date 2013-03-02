// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <assert.h>
#include <iostream>
#include <SNMPpp/Get.hpp>


void testGetNext( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Test GETNEXT:" << std::endl;

	SNMPpp::OID newOid;
	SNMPpp::OID oldOid( SNMPpp::OID::kInternet );
	assert( newOid.empty() == true );
	assert( oldOid.empty() == false );

	for ( size_t idx = 0; idx < 10; idx ++ )
	{
		SNMPpp::PDU pdu( SNMPpp::PDU::kGetNext );
		pdu.addNullVar( oldOid );
		assert( pdu.size() == 1 );
		assert( pdu.varlist().firstOID() == oldOid );

		pdu = SNMPpp::getNext( sessionHandle, pdu );

		std::cout << pdu; // debug output

		assert( pdu.empty() == false );
		assert( pdu.size() == 1 );
		newOid = pdu.varlist().firstOID();
		assert( oldOid < newOid );
		assert( newOid > oldOid );

		std::cout << "Reading " << pdu.firstOID() << ": " << pdu.varlist().asString() << std::endl;

		pdu.free();
		assert( pdu.empty() == true );
		assert( pdu.size() == 0 );

		oldOid = newOid;
	}

	return;
}


void testManyOidsAtOnce1( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Test GET with multiple OIDs:" << std::endl;

	SNMPpp::PDU pdu( SNMPpp::PDU::kGet );
	pdu.addNullVar( "1.3.6.1.2.1.1.1.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.2.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.3.0" );
	pdu.addNullVar( "1.3.6.1.2.1.1.4.0" );
	const size_t len = pdu.size();
	assert( len == 4 );

	pdu = SNMPpp::get( sessionHandle, pdu );
	std::cout << pdu;
	assert( pdu.empty() == false );
	assert( pdu.size() == len );

	// always remember to free the PDU once done with it
	pdu.free();
	assert( pdu.empty() == true );
	assert( pdu.size() == 0 );

	return;
}


void testManyOidsAtOnce2( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Test GET with a std::set of OIDs:" << std::endl;

	SNMPpp::SetOID oids;
	oids.insert( ".1.3.6.1.2.1.1.1.0" );
	oids.insert( ".1.3.6.1.2.1.1.2.0" );
	oids.insert( ".1.3.6.1.2.1.1.3.0" );
	oids.insert( ".1.3.6.1.2.1.1.4.0" );
	oids.insert( ".1.3.6.1.2.1.1.5.0" );
	oids.insert( ".1.3.6.1.2.1.1.6.0" );
	oids.insert( ".1.3.6.1.2.1.1.7.0" );
	oids.insert( ".1.3.6.1.2.1.1.8.0" );
	const size_t len = oids.size();
	assert( oids.empty() == false );
	assert( len == 8 );

	SNMPpp::PDU pdu = SNMPpp::get( sessionHandle, oids );
	std::cout << pdu;
	assert( pdu.empty() == false );
	assert( pdu.size() == len );
	const SNMPpp::OID o = pdu.varlist().firstOID();
	assert( o.empty() == false );
	assert( pdu.varlist().asnType(o) != ASN_NULL );

	// always remember to free the PDU once done with it
	pdu.free();
	assert( pdu.empty() == true );
	assert( pdu.size() == 0 );

	return;
}


void testGetSingleOid( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Test GET with a specific OID:" << std::endl;

	SNMPpp::OID o = "1.3.6.1.2.1.1.1.0";
	SNMPpp::PDU pdu = SNMPpp::get( sessionHandle, o );

	assert( pdu.empty() == false );
	assert( pdu.size() == 1 );
	assert( pdu.varlist().asnType(o) != ASN_NULL );
	
	std::cout 	<< pdu
				<< "\tIf we know the type of an OID, then we can retrieve the value directly from the PDU:" << std::endl
				<< "\t" << o << ":  " << pdu.varlist().getString(o) << std::endl;

	// always remember to free the PDU once done with it
	pdu.free();
	assert( pdu.empty() == true );
	assert( pdu.size() == 0 );

	return;
}


void testGetBulk( SNMPpp::SessionHandle &sessionHandle )
{
	std::cout << "Test GETBULK (SNMPv2) with a single starting OID:" << std::endl;

	SNMPpp::PDU pdu( SNMPpp::PDU::kGetBulk );
	pdu.addNullVar( ".1" );
	pdu = SNMPpp::getBulk( sessionHandle, pdu, 20 );
	std::cout << pdu;
	
	const SNMPpp::OID o = pdu.varlist().firstOID();
	assert( o.empty() == false );
	assert( pdu.varlist().asnType(o) != ASN_NULL );
	assert( pdu.empty() == false );
	assert( pdu.size() == 20 );

	// always remember to free the PDU once done with it
	pdu.free();
	assert( pdu.empty() == true );
	assert( pdu.size() == 0 );

	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Test some of the net-snmp GET/GETNEXT/GETBULK functionality." << std::endl;

	SNMPpp::SessionHandle sessionHandle = NULL;
	SNMPpp::openSession( sessionHandle, "udp:localhost:161" );
	assert( sessionHandle != NULL );

	testGetNext			( sessionHandle );
	testManyOidsAtOnce1	( sessionHandle );
	testManyOidsAtOnce2	( sessionHandle );
	testGetSingleOid		( sessionHandle );
	testGetBulk			( sessionHandle );
	SNMPpp::closeSession	( sessionHandle );

	assert( sessionHandle == NULL );

	return 0;
}
