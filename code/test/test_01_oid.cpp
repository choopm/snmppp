// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <SNMPpp/OID.hpp>


void checkConstructor( const std::string &name, const SNMPpp::OID &oid1, const SNMPpp::OID &oid2, const int sizeHint=-1 )
{
	// compare the 2 given OIDs to make sure they were constructed the same way
	std::cout << "\t" << std::left << std::setw(11) << name << ": comparing OID \"" << oid1 << "\" and OID  \"" << oid2 << "\"" << std::endl;

	if ( sizeHint != -1 )
	{
		assert( oid1.size()		== (size_t)sizeHint );
		assert( oid2.size()		== (size_t)sizeHint );
	}
	assert( oid1.size()			== oid2.size() );
	assert( oid1.empty()			== oid2.empty() );
	assert( oid1.str()			== oid2.str() );
	assert( oid1.str()			== oid2.operator std::string() );
	assert( oid1.str().empty()	== oid2.str().empty() );
	assert( (bool)oid1			== (bool)oid2 );
	assert( (bool)oid1			== oid2.operator bool() );
	assert( oid1					== oid2 );
	assert( oid2					== oid1 );
	assert( (oid1 < oid2)		== false );
	assert( (oid2 < oid1)		== false );
	assert( (oid1 > oid2)		== false );
	assert( (oid2 > oid1)		== false );
	assert( (oid1 <= oid2)		== true );
	assert( (oid2 <= oid1)		== true );
	assert( (oid1 >= oid2)		== true );
	assert( (oid2 >= oid1)		== true );

	std::cout << std::endl;

	return;
}


void checkConstructor( const std::string &name, const SNMPpp::OID &oid1, const std::string &txt2, const int sizeHint=-1 )
{
	// create an OID with txt1, and compare against the given oid2
	std::cout << "\t" << std::left << std::setw(11) << name << ": comparing OID \"" << oid1 << "\" and text \"" << txt2 << "\"" << std::endl;
	
	const SNMPpp::OID oid2( txt2 );
	checkConstructor( name, oid1, oid2, sizeHint );
	
	return;
}


void checkConstructor( void )
{
	std::cout << "Checking OID constructors:" << std::endl;
	
	const SNMPpp::OID oid1;
	const SNMPpp::OID oid2( SNMPpp::OID::kPrivateEnterprise );
	const SNMPpp::OID oid3( oid2 );
	const SNMPpp::OID oid4( ".1.2.3.4.5.6" );
	
	// create a net-snmp oid, then a SNMPpp::OID from the net-snmp one
	const oid oid5[] = { 1, 2, 3, 4, 5, 6 };
	const size_t len = OID_LENGTH(oid5);
	const SNMPpp::OID oid6( oid5, len );
	
	// create a net-snmp varlist with a single OID in it
	netsnmp_variable_list *vl = NULL;
	snmp_varlist_add_variable( &vl, oid5, len, ASN_NULL, 0, 0 );
	const SNMPpp::OID oid7( vl );
	
	// long OID
	std::vector < unsigned long > v;
	for ( size_t idx = 0; idx < 20; idx ++ )
	{
		v.push_back( idx );
	}
	const SNMPpp::OID oid8( &v[0], v.size() );
	
	checkConstructor( "empty",		oid1, std::string(""), 0 );
	checkConstructor( "jump",			oid1, std::string("junk"), 0 );
	checkConstructor( "enterprise",	oid2, std::string(".1.3.6.1.4"), 5 );
	checkConstructor( "copy",			oid2, oid3, 5 );
	checkConstructor( "first dot",	oid4, std::string( ".1.2.3.4.5.6"					) );
	checkConstructor( "no dot",		oid4, std::string(  "1.2.3.4.5.6"					) );
	checkConstructor( "both dots",	oid4, std::string( ".1.2.3.4.5.6."				) );
	checkConstructor( "last dot",		oid4, std::string(  "1.2.3.4.5.6."				) );
	checkConstructor( "whitespace",	oid4, std::string( ".1.2.3.4.5.6 followed by junk"	) );
	checkConstructor( "more junk",	oid4, std::string( ".1.2.3.4.5.6.followed by junk"	) );
	checkConstructor( "oid array 1",	oid4, SNMPpp::OID( oid5, len ) );
	checkConstructor( "oid array 2",	oid4, oid6 );
	checkConstructor( "varlist",		oid4, oid7 );
	checkConstructor( "long oid",		oid8, std::string("0.1.2.3.4.5.6.7.8.9.10.11.12.13.14.15.16.17.18.19"), 20 );
	
	snmp_free_varbind( vl );
	
	return;
}


void checkConstness( void )
{
	// all of these methods should be "const"

	const SNMPpp::OID o( "1.2.3.4.5.6" );
	std::cout 	<< "Checking compile-time constness using the following const object: " << o	<< std::endl
				<< "\tsize:                  " << o.size()									<< std::endl
				<< "\tlen:                   " << o.len()										<< std::endl
				<< "\tsize_t:                " << o.operator size_t()							<< std::endl
				<< "\tempty:                 " << o.empty()									<< std::endl
				<< "\tbool:                  " << o.operator bool()							<< std::endl
				<< "\tstr:                   " << o.str()										<< std::endl
				<< "\tstd::string:           " << std::string(o)								<< std::endl
				<< "\tisChild:               " << o.isChildOf( "1.2.3.4" )						<< std::endl
				<< "\tisParent:              " << o.isParentOf( "1.2.3.4.5.6.7.8" )				<< std::endl
				<< "\timmediateChild:        " << o.isImmediateChildOf( ".1.2.3.4.5")			<< std::endl
				<< "\timmediateParent:       " << o.isImmediateParentOf( "1.2.3.4.5.6.7")		<< std::endl
				<< "\tconst unsigned long *: " << o.operator const unsigned long *()			<< std::endl
				<< "\tconst unsigned char *: " << (void*)o.operator const unsigned char *()		<< std::endl
				<< "\t==:                    " << (o == ".1.2.3.4.5.6")						<< std::endl
				<< "\t+:                     " << (o + std::string(".7"))						<< std::endl
				<< "\t<:                     " << (o < ".9.8.7")								<< std::endl
				<< "\t>:                     " << (o > ".0.1.2")								<< std::endl
				<< "\t<=:                    " << (o <= ".9.8.7")								<< std::endl
				<< "\t>=:                    " << (o >= ".0.1.2")								<< std::endl;

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
	oid2 += 6;
	std::cout	<< "\toid2: " << oid2 << std::endl
				<< "\tverifying parent<->child relationship between " << oid1 << " and " << oid2 << std::endl;
	assert( oid2.isChildOf(  oid1 ) );
	assert( oid1.isParentOf( oid2 ) );
	assert( oid2.isImmediateChildOf(  oid1 ) );
	assert( oid1.isImmediateParentOf( oid2 ) );

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
	oid3 += 6;
	std::cout << "\toid3: " << oid3 << std::endl;
	assert( oid2 == oid3 );
	oid3 += 7;
	std::cout	<< "\toid3: " << oid3 << std::endl
				<< "\tverifying parent<->child relationship between " << oid1 << ", " << oid2 << ", and " << oid3 << std::endl;
	assert( oid2.isChildOf(  oid1 ) );
	assert( oid1.isParentOf( oid2 ) );
	assert( oid3.isChildOf(  oid1 ) );
	assert( oid1.isParentOf( oid3 ) );
	assert( oid3.isChildOf(  oid2 ) );
	assert( oid2.isParentOf( oid3 ) );
	assert( oid3.isImmediateChildOf(  oid2 ) );
	assert( oid2.isImmediateParentOf( oid3 ) );
	assert( oid3.isImmediateChildOf(  oid1 ) == false );
	assert( oid1.isImmediateParentOf( oid3 ) == false );

	return;
}


int main( int argc, char *argv[] )
{
	std::cout << "Test some of the OID functionality." << std::endl;

	checkConstructor();
	checkConstness();
	checkBasicFunctionality();

	std::cout << "\t...done!" << std::endl;

	return 0;
}
