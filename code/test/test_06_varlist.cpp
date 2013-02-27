// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <SNMPpp/Varlist.hpp>
#include <SNMPpp/Trap.hpp>


void checkVarlist( const std::string &name, const SNMPpp::Varlist &varlist, const int sizeHint=-1 )
{
	std::cout << "\t" << std::left << std::setw(15) << name << ": ";

	// sneaky...get to the same net-snmp pointer even though our Varlist object is const
	SNMPpp::Varlist nonconstVarlist( varlist );
	netsnmp_variable_list  *p  = nonconstVarlist;
	netsnmp_variable_list **pp = nonconstVarlist;
	assert( *pp == p );

	if ( sizeHint != -1 )
	{
		assert( varlist.size()	== (size_t)	sizeHint );
		assert( count_varbinds(p)	==			sizeHint );
	}
	if ( varlist.empty() )
	{
		// this basically means we actually have no varlist
		std::cout << "verifying empty varlist" << std::endl;

		assert( p == NULL );
		assert( varlist.size() == 0 );

		SNMPpp::VecOID v;
		varlist.getOids( v );
		assert( v.size() == 0 );
		assert( v.empty() == true );

		SNMPpp::SetOID s;
		varlist.getOids( s );
		assert( s.size() == 0 );
		assert( s.empty() == true );

		SNMPpp::MapOidVarList m = varlist.getMap();
		assert( m.size() == 0 );
		assert( m.empty() == true );
	}
	else
	{
		// if we have at least 1 OID in the varlist:
		std::cout << "verifying non-empty varlist" << std::endl;
		
		assert( p != NULL );
		assert( varlist.size() != 0 );

		SNMPpp::VecOID v;
		varlist.getOids( v );
		assert( v.size() > 0 );
		assert( v.empty() == false );

		SNMPpp::SetOID s;
		varlist.getOids( s );
		assert( s.size() > 0 );
		assert( s.empty() == false );
		assert( s.size() <= v.size() );

		SNMPpp::MapOidVarList m = varlist.getMap();
		assert( m.size() > 0 );
		assert( m.empty() == false );
		assert( m.size() == s.size() );

		const SNMPpp::OID o1 = varlist.firstOID();
		const SNMPpp::OID o2( p );
		assert( o1.empty() == false );
		assert( o2.empty() == false );
		assert( o1 == o2 );
		assert( o1 == v[0] );
		assert( o2 == v[0] );
		assert( varlist.contains(o1) == true );

		const netsnmp_variable_list	*vl = varlist.at(o1);
		const netsnmp_vardata		&vd = varlist.valueAt(o1);
		assert( vl != NULL );
		assert( vl == p );
		assert( vl->type == varlist.asnType(o1) );
		assert( &vd == &vl->val );
	}

	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Testing Varlist:" << std::endl;

	SNMPpp::Varlist varlist;
	checkVarlist( "empty", varlist, 0 );

	varlist.addNullVar( "1.2.3.4.5.6.7" );
	checkVarlist( "single oid", varlist, 1 );

	varlist.free();
	checkVarlist( "freed varlist", varlist, 0 );

	varlist = SNMPpp::createV2TrapVarlist( "1.2.3.4.5.6.7" );
	checkVarlist( "trap varlist", varlist, 2 );

	// add a few more OIDs
	varlist.addNullVar( "1.2.3" );
	varlist.addNullVar( "1.2.3.4" );
	varlist.addNullVar( "1.2.3.4.5" );
	varlist.addNullVar( "1.2.3.4.5.6" );
	varlist.addNullVar( "1.2.3.4.5.6.7" );
	checkVarlist( "multiple OIDs", varlist, 7 );

	std::cout << "\t...done!" << std::endl;

	return 0;
}
