// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <SNMPpp/OID.hpp>


void checkConstructors( void )
{
	std::cout << "Checking constructors:" << std::endl;

	SNMPpp::OID oid1;
	std::cout << "\tEmpty OID:             \"" << oid1 << "\"" << std::endl;
	assert( oid1.size() == 0 );
	assert( oid1.empty() == true );
	assert( std::string(oid1) == "" );

	SNMPpp::OID oid2( SNMPpp::OID::kPrivateEnterprise );
	std::cout << "\tSNMPpp::OID::ECommon:  " << oid2 << std::endl;
	assert( oid2.size() == 5 );
	assert( oid2.empty() == false );
	assert( oid2 == SNMPpp::OID(".1.3.6.1.4") );

	SNMPpp::OID oid3( oid2 );
	std::cout << "\tSNMPpp::OID copy:      " << oid3 << std::endl;
	assert( oid3 == oid2 );

	SNMPpp::OID oid4( ".1.2.3.4.5.6" );
	SNMPpp::OID oid5( "1.2.3.4.5.6"  );
	std::cout << "\tSNMPpp::OID str1:      " << oid4 << std::endl;
	std::cout << "\tSNMPpp::OID str2:      " << oid5 << std::endl;
	assert( oid4.size() == 6 );
	assert( oid4 == oid5 );

	SNMPpp::OID oid6( ".1.2.3.4.5.6 followed by junk" );
	std::cout << "\tSNMPpp::OID str3:      " << oid6 << std::endl;
	assert( oid6.size() == 6 );
	assert( oid4 == oid6 );

	return;
}


void checkConstness( void )
{
	// all of these methods should be "const"

	const SNMPpp::OID o( "1.2.3.4.5.6" );
	std::cout << "Checking compile-time constness using the following const object: " << o << std::endl;

	std::cout	<< "\tSize:            " << o.size() << std::endl
				<< "\tEmpty:           " << o.empty() << std::endl
				<< "\tstd::string:     " << std::string(o) << std::endl
				<< "\tisChild:         " << o.isChildOf( "1.2.3.4" ) << std::endl
				<< "\tisParent:        " << o.isParentOf( "1.2.3.4.5.6.7.8" ) << std::endl
				<< "\timmediateChild:  " << o.isImmediateChildOf( ".1.2.3.4.5") << std::endl
				<< "\timmediateParent: " << o.isImmediateParentOf( "1.2.3.4.5.6.7") << std::endl;

	return;
}


void checkBasicFunctionality( void )
{
	std::cout << "Checking basic functionality:" << std::endl;

	SNMPpp::OID oid1;
	oid1 = "1.2.3.4.5";
	std::cout << "\toid1: " << oid1 << std::endl;
	assert( oid1.size() == 5 );
	assert( oid1.empty() == false );
	assert( oid1 == SNMPpp::OID(".1.2.3.4.5") );

	// build an OID using operator+=()
	SNMPpp::OID oid2;
	for ( size_t idx = 1; idx < 6; idx ++ )
	{
		oid2 += idx;
	}
	std::cout << "\toid2: " << oid2 << std::endl;
	assert( oid1 == oid2 );

	// build an OID using operator+=( string )
	SNMPpp::OID oid3;
	for ( size_t idx = 1; idx < 6; idx ++ )
	{
		char str[3] = ". ";
		str[1] = '0' + idx;
		oid3 += str;
	}
	std::cout << "\toid3: " << oid3 << std::endl;
	assert( oid1 == oid3 );

	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Test some of the OID functionality." << std::endl;

	checkConstructors();
	checkConstness();
	checkBasicFunctionality();

	return 0;
}
