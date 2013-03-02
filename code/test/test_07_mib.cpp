// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <SNMPpp/SNMPpp.hpp>


void lookupOid( const SNMPpp::OID &o )
{
	std::cout << "Looking up " << o << std::endl;
	std::cout << "\tFULL   = " << o.nameFromMib( SNMPpp::OID::kFull		) << std::endl;
	std::cout << "\tINVERT = " << o.nameFromMib( SNMPpp::OID::kInverted		) << std::endl;
	std::cout << "\tPARTIAL= " << o.nameFromMib( SNMPpp::OID::kPartial		) << std::endl;
	std::cout << "\tLEAF   = " << o.nameFromMib( SNMPpp::OID::kLeafOnly		) << std::endl;

	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Testing MIBs:" << std::endl;

//	snmp_set_save_descriptions(1);
	netsnmp_init_mib();

	lookupOid( ""							);
	lookupOid( ".1"							);
	lookupOid( ".1.3"						);
	lookupOid( ".1.3.6.1.4.1.38322.1.0.1"		);
	lookupOid( ".1.3.6.1.4.1.38322"			);
	lookupOid( ".1.3.6.1.4.1"					);
	lookupOid( ".1.3.6.1.6.3.15.1.2.2.1.3.17.128.0.31.136.128.194.238.107.99.53.234.27.81.0.0.0.0.12.105.110.116.101.114.110.97.108.85.115.101.114" );
	lookupOid( ".1.3.6.1.4.1.8072.1.2.1.1.4.0.10.1.3.6.1.2.1.1.9.1.2.1" );
	lookupOid( ".1.3.6.1.4.1.2021.11.11.0"	);

	SNMPpp::OID o( ".1.3.6.1.4.1.38322" );

	std::cout << std::endl;
	if ( o[2] == 6 ) std::cout << "\to[2]==3 is TRUE for " << o << std::endl;
	if ( o[2] != 7 ) std::cout << "\to[2]==4 is FALSE for " << o << std::endl;
	std::cout << "\to[6]==" << o[6] << std::endl;

	struct tree * tree = get_tree( o, o, get_tree_head() );

	std::cout	<< std::endl
				<< "\tOID=" << o << std::endl
				<< "\tp=" << (void*)tree << std::endl;
	if ( tree )
	{
		std::cout	<< "\tlabel="		<< (tree->label			? tree->label		: "" ) << std::endl
					<< "\tparent="		<< (tree->parent			? tree->parent		:NULL) << std::endl
					<< "\taugments="		<< (tree->augments		? tree->augments		: "" ) << std::endl
					<< "\thints="		<< (tree->hint			? tree->hint			: "" ) << std::endl
					<< "\tunits="		<< (tree->units			? tree->units		: "" ) << std::endl
					<< "\tdescription="	<< (tree->description	? tree->description	: "" ) << std::endl
					<< "\treference="	<< (tree->reference		? tree->reference	: "" ) << std::endl
					<< "\tdefaultValue="	<< (tree->defaultValue	? tree->defaultValue	: "" ) << std::endl;
	}

//	FILE *f = fopen( "output.txt", "w" );
//	tree = read_all_mibs();
//	print_ascii_dump_tree( f, tree, 0 );
//	fclose( f );
//	while ( tree != NULL )
//	{
//		tree = tree->next;
//	}
//	print_description( o, o, 999 );

	std::cout << "\t...done!" << std::endl;

	return 0;
}
