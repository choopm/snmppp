// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <stdlib.h>
#include <sstream>
#include <SNMPpp/OID.hpp>


SNMPpp::OID::~OID( void )
{
	clear();
	return;
}


SNMPpp::OID::OID( void )
{
	return;
}


SNMPpp::OID::OID( const SNMPpp::OID &o )
{
	set( o );

	return;
}


SNMPpp::OID::OID( const char * const s )
{
	set( s );

	return;
}


SNMPpp::OID::OID( const std::string &s )
{
	set( s );

	return;
}


SNMPpp::OID::OID( const SNMPpp::OID::ECommon &location )
{
	set( location );

	return;
}


SNMPpp::OID::OID( const unsigned long *array, const size_t length )
{
	v.reserve( length );
	for ( size_t idx = 0; idx < length; idx ++ )
	{
		v.push_back( array[idx] );
	}

	return;
}


SNMPpp::OID::OID( const netsnmp_variable_list *vl )
{
	if ( vl != NULL )
	{
		v.reserve( vl->name_length );
		for ( size_t idx = 0; idx < vl->name_length; idx ++ )
		{
			v.push_back( vl->name[idx] );
		}
	}

	return;
}


SNMPpp::OID::operator std::string( void ) const
{
	// convert the OID to a text string, such as:  .0.1.2.3.4

	std::stringstream ss;
	for ( size_t idx = 0; idx < v.size(); idx ++ )
	{
		ss << "." << v[ idx ];
	}

	return ss.str();
}


SNMPpp::OID::operator const unsigned long *( void ) const
{
	// net-snmp uses "unsigned long *" as OIDs, so this operator makes it easy
	// and convenient to convert a C++ "OID" to a net-snmp "oid *".

	if ( empty() )
	{
		/// @return NULL if the OID is empty.
		return NULL;
	}

	/// @return The address of the first value in the vector.
	return &v[0];
}


SNMPpp::OID &SNMPpp::OID::clear( void )
{
	v.clear();

	return *this;
}


SNMPpp::OID SNMPpp::OID::operator+( const unsigned long l ) const
{
	// For example:
	//
	//		SNMPpp::OID oid1( ".1.2.3.4" );
	//		SNMPpp::OID oid2 = oid1 + 5;

	OID newOid( *this );
	newOid.v.push_back( l );

	return newOid;
}


SNMPpp::OID &SNMPpp::OID::operator+=( const unsigned long l )
{
	// For example:
	//
	//		SNMPpp::OID oid( ".1.2.3.4" );
	//		oid += 5;

	v.push_back( l );

	return *this;
}


SNMPpp::OID SNMPpp::OID::operator+( const std::string &s ) const
{
	// For example:
	//
	//		SNMPpp::OID oid1( ".1.2.3.4" );
	//		SNMPpp::OID oid2 = oid1 + ".5.6";

	OID newOid( *this );

	size_t pos = 0;
	while ( pos < std::string::npos )
	{
		// move past the next "." character
		if ( s[ pos ] == '.' )
		{
			pos ++;
		}

		if ( pos >= s.size() )
		{
			break;
		}

		const char * const p = &s[pos];

		// if the next char is not numeric, then we're done
		if ( isdigit( p[0] ) == false )
		{
			break;
		}

		newOid.v.push_back( atoi( p ) );

		// move our position pointer past the numeric value we just processed
		pos = s.find_first_not_of( "0123456789", pos );
	}

	return newOid;
}


SNMPpp::OID &SNMPpp::OID::operator+=( const std::string &s )
{
	// For example:
	//
	//		SNMPpp::OID oid( ".1.2.3.4" );
	//		oid += ".5.6";

	OID newOid = operator+( s );
	v.swap( newOid.v );

	return *this;
}


SNMPpp::OID &SNMPpp::OID::set( const OID &o )
{
	if ( &o != this )
	{
		clear();
		v = o.v;
	}

	return *this;
}


SNMPpp::OID &SNMPpp::OID::set( const char * const s )
{
	clear();
	if ( s != NULL )
	{
		operator+=( std::string( s ) );
	}

	return *this;
}


SNMPpp::OID &SNMPpp::OID::set( const std::string &s )
{
	clear();
	operator+=( s );

	return *this;
}


SNMPpp::OID &SNMPpp::OID::set( const SNMPpp::OID::ECommon &location )
{
	switch ( location )
	{
		case kInvalid:			set( ""						); break;
		case kEmpty:				set( ""						); break;
		case kInternet:			set( "1.3.6.1"				); break;
		case kPrivateEnterprise:	set( "1.3.6.1.4"				); break;
		case kSysUpTime:			set( "1.3.6.1.2.1.1.3.0"		); break;
		case kTrap:				set( "1.3.6.1.6.3.1.1.4.1.0"	); break;
	}

	return *this;
}


bool SNMPpp::OID::isChildOf( const SNMPpp::OID &rhs ) const
{
	// For example:
	//
	//		THIS						RHS
	//		".1.2.3.4.5" isChildOf( ".1.2.3" ) == TRUE
	//		".1.2.9.9.9" isChildOf( ".1.2.3" ) == FALSE
	//		".1.2.3.4.5" isChildOf( ""       ) == FALSE
	//		""           isChildOf( ".1.2.3" ) == FALSE

	bool isChild = false;

	const size_t thisSize = this->size();
	const size_t rhsSize  = rhs  .size();

	// both THIS and RHS must have a size for there to be any kind of relationship between THIS and RHS
	if ( rhsSize > 0 && thisSize > 0 )
	{
		// the only way THIS can be a child of RHS is if RHS < THIS
		if ( rhsSize < thisSize )
		{
			// at this point, assume they are indeed child<->parent, and look for any mismatches
			isChild = true;
			for ( size_t idx = 0; idx < rhsSize; idx ++ )
			{
				if ( v[idx] != rhs.v[idx] )
				{
					// we found a mismatch, so THIS cannot be a child of RHS
					isChild = false;
					break;
				}
			}
		}
	}

	return isChild;
}


bool SNMPpp::OID::isParentOf( const SNMPpp::OID &rhs ) const
{
	// asking if THIS is a parent of RHS is the same as asking if RHS is a child of THIS
	return rhs.isChildOf( *this );
}


bool SNMPpp::OID::isImmediateChildOf( const SNMPpp::OID &rhs ) const
{
	const bool isChild = ( size() == rhs.size() + 1 );
	return isChild && isChildOf( rhs );
}


bool SNMPpp::OID::isImmediateParentOf( const SNMPpp::OID &rhs ) const
{
	return rhs.isImmediateChildOf( *this );
}


std::ostream &operator<<( std::ostream &os, const SNMPpp::OID &o )
{
	os << o.str();

	return os;
}
