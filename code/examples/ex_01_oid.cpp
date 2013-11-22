// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <iostream>
#include <SNMPpp/OID.hpp>


int main( int argc, char *argv[] )
{
	std::cout << "Example code showing how to use SNMPpp::OIDs." << std::endl;

	SNMPpp::OID oid1( "1.2.3.4.5.6" );
	SNMPpp::OID oid2( oid1 );
	oid2 += "7.8.9";

	std::cout	<< "OID1 == " << oid1 << std::endl
				<< "OID2 == " << oid2 << std::endl
				<< "OID2 is " << ( oid2.isChildOf(oid1) ? "": "not " ) << "a child of OID1." << std::endl
				<< "OID1 is " << ( oid1.isImmediateParentOf(oid2) ? "" : "not " ) << "the immediate parent of OID2." << std::endl
				<< "The length of OID1 is " << oid1.size() << "." << std::endl
				<< "The length of OID2 is " << oid2.size() << "." << std::endl;

	return 0;
}
