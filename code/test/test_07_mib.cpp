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

	snmp_set_save_descriptions(1);
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

	const struct tree * tree = o.getTree();

	std::cout	<< std::endl
				<< "Information extracted from the tree or MIB module:" << std::endl
				<< "\tOID=" << o << std::endl
				<< "\ttree pointer=" << (void*)tree << std::endl;
	if ( tree )
	{
		std::cout	<< "\ttree->modid="			<< tree->modid										<< std::endl
					<< "\ttree->label="			<< (tree->label			? tree->label		: "" )	<< std::endl
					<< "\ttree->parent="			<< (tree->parent			? tree->parent		:NULL)	<< std::endl
					<< "\ttree->augments="		<< (tree->augments		? tree->augments		: "" )	<< std::endl
					<< "\ttree->hints="			<< (tree->hint			? tree->hint			: "" )	<< std::endl
					<< "\ttree->units="			<< (tree->units			? tree->units		: "" )	<< std::endl
					<< "\ttree->description="		<< (tree->description	? tree->description	: "" )	<< std::endl
					<< "\ttree->reference="		<< (tree->reference		? tree->reference	: "" )	<< std::endl
					<< "\ttree->defaultValue="	<< (tree->defaultValue	? tree->defaultValue	: "" )	<< std::endl;
	}

	const struct module *module = o.getModule();
	if ( module )
	{
		std::cout	<< "\tmodule->name="			<< module->name			<< std::endl
					<< "\tmodule->file="			<< module->file			<< std::endl
					<< "\tmodule->no_imports="	<< module->no_imports	<< std::endl;
		for ( int idx = 0; idx < module->no_imports; idx ++ )
		{
			std::cout << "\t\t#" << idx << ": module->imports[" << idx << "].label=" << module->imports[idx].label << std::endl;
		}
	}

	std::cout	<< "\to.mibModuleName(false)=" << o.mibModuleName(false)	<< std::endl
				<< "\to.mibModuleFile(false)=" << o.mibModuleFile(false)	<< std::endl
				<< "\to.mibModuleName(true) =" << o.mibModuleName(true)	<< std::endl
				<< "\to.mibModuleFile(true) =" << o.mibModuleFile(true)	<< std::endl;

	std::cout << "\t...done!" << std::endl;

	return 0;
}
