// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <stdlib.h>
#include <sstream>
#include <stdexcept>
#include <algorithm>
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


SNMPpp::OID::OID( const oid * o, const size_t length )
{
	v.reserve( length );
	for ( size_t idx = 0; idx < length; idx ++ )
	{
		v.push_back( o[idx] );
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


SNMPpp::OID::operator const oid *( void ) const
{
	// CONST version:
	//
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


SNMPpp::OID::operator oid *( void )
{
	// NON-CONST version:
	//
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


SNMPpp::OID SNMPpp::OID::operator+( const oid o ) const
{
	// For example:
	//
	//		SNMPpp::OID oid1( ".1.2.3.4" );
	//		SNMPpp::OID oid2 = oid1 + 5;

	OID newOid( *this );
	newOid.v.push_back( o );

	return newOid;
}


SNMPpp::OID &SNMPpp::OID::operator+=( const oid o )
{
	// For example:
	//
	//		SNMPpp::OID oid( ".1.2.3.4" );
	//		oid += 5;

	v.push_back( o );

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


oid SNMPpp::OID::operator[]( const size_t idx ) const
{
	if ( idx >= v.size() )
	{
		/// @throw std::invalid_argument if the index is larger than the OID.
		throw std::invalid_argument( "The index is beyond the end of OID \"" + str() + "\"." );
	}

	return v[idx];
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
		case kInvalid:				set( ""							); break;
		case kEmpty:				set( ""							); break;
		case kInternet:				set( "1.3.6.1"					); break;
		case kPrivateEnterprise:	set( "1.3.6.1.4"				); break;
		case kSysUpTime:			set( "1.3.6.1.2.1.1.3.0"		); break;
		case kTrap:					set( "1.3.6.1.6.3.1.1.4.1.0"	); break;
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


SNMPpp::OID SNMPpp::OID::parent( const size_t level ) const
{
	/** @details
	 * For example, if we have this:
	 *
	 *		SNMPpp::OID oid1( ".1.2.3.4.5" );
	 *		SNMPpp::OID oid2 = oid1.parent();
	 *		SNMPpp::OID oid3 = oid1.parent(3);
	 *
	 * ...then the value of oid2 will be `.1.2.3.4`,
	 * and the value of oid3 will be `.1.2`.
	 */

	SNMPpp::OID o( *this );
	if ( level >= o.size() )
	{
		/// @return If the level is greater than or equal to the number of values in the OID, then an empty OID is returned to the caller.
		o.clear();
	}
	else
	{
		/// @return If the level is smaller than the number of values in the OID, then the vector is truncated by the appropriate amount and a new OID is returned to the caller.
		o.v.resize( o.size() - level );
	}

	return o;
}


std::string SNMPpp::OID::nameFromMib( const SNMPpp::OID::ENameLookup lookup ) const
{
	/** @note If net-snmp hasn't yet been initialized, you must call
	 *`netsnmp_init_mib()` prior to nameFromMib().  Otherwise the MIB files
	 * haven't been loaded, and the OID wont map to a known name.
	 *
	 * The different lookup types determine how the name is shown:
	 *
	 * Value			| Meaning										| Example
	 * -----			| -------										| -------
	 * kLeafOnly		| Only the very last name is returned.		| .1.3.6.1 -> "internet"
	 * kPartial			| Start of OID combined with the last name.	| .1.3.6.1 -> ".1.3.6.internet"
	 * kInverted		| The opposite of kPartial.					| .1.3.6.1 -> ".iso.org.dod.1"
	 * kFull			| All names are looked up.						| .1.3.6.1 -> ".iso.org.dod.internet"
	 *
	 * If the MIB files haven't been installed or initialized, the same
	 * example would look like this:
	 *
	 * Value			| Meaning										| Example
	 * -----			| -------										| -------
	 * kLeafOnly		| Only the very last name is returned.		| .1.3.6.1 -> "1"
	 * kPartial			| Start of OID combined with the last name.	| .1.3.6.1 -> ".1.3.6.1"
	 * kInverted		| The opposite of kPartial.					| .1.3.6.1 -> ".1.3.6.1"
	 * kFull			| All names are looked up.						| .1.3.6.1 -> ".1.3.6.1"
	 */

	if ( empty() )
	{
		// nothing for us to do
		return "";
	}

	switch ( lookup )
	{
		case SNMPpp::OID::kLeafOnly:
		case SNMPpp::OID::kPartial:
		case SNMPpp::OID::kInverted:
		case SNMPpp::OID::kFull:
			break;
		default:
			/// @throw std::invalid_argument if the lookup type is not one of the 4 values described above.
			throw std::invalid_argument( "Invalid lookup type for OID " + str() + "." );
	}

	std::vector<std::string> vstr, tmp;
	vstr.reserve( v.size() );

	// start by copying to vstr all the numeric values we have
	for ( size_t idx = 0; idx < v.size(); idx ++ )
	{
		char buffer[10] = {0};
		snprintf( buffer, sizeof(buffer), "%ld", v[idx] );
		vstr.push_back( buffer );
	}

	// Start with the longest OID we have, and look it up in the MIBs to see
	// if we can find a name.  If not, then take the parent OID and start the
	// process over until we eventually find a match.

	// get the OID which interests us and which needs to be looked up
	const SNMPpp::OID o = parent( lookup == SNMPpp::OID::kInverted ? 1 : 0 );

	if ( ! o.empty() )
	{
		// BEWARE:  get_tree() will return the closest match when it cannot find
		// an exact match!  Meaning that if the mibs describe .1.2.3.4 and you
		// ask for .1.2.3.4.5.6 you'll still get a valid "t" pointer back.  It
		// is up to us afterwards to determine just how close of a match we've
		// been given.
		struct tree * t = get_tree( o, o, get_tree_head() );

		while ( t != NULL && t->label != NULL )
		{
			// note how "tmp" is reversed -- it starts with the last entry, and then builds up to the root
			tmp.push_back( t->label );
			t = t->parent;
		}
	}

	// now take the relevant names from tmp and insert them into the right location in vstr
	for ( size_t idx = 0; idx < tmp.size(); idx ++ )
	{
		// for kLeakOnly and kPartial, we only want the very last name in the
		// OID (meaning the first name in "tmp") so exit out of this loop if
		// we're about to copy something other than the last name
		if (		lookup == SNMPpp::OID::kLeafOnly	||
					lookup == SNMPpp::OID::kPartial	)
		{
			if ( tmp.size() != vstr.size() || idx > 0 )
			{
				break;
			}
		}
		vstr[ tmp.size() - idx - 1 ] = tmp[idx];
	}

	// now build up the string to represent what the caller wants
	std::string name;
	size_t idx = 0;
	if ( lookup == kLeafOnly )
	{
		// skip to the end of the vector
		idx = v.size() - 1;
	}
	else
	{
		name = ".";
	}

	while( true )
	{
		name += vstr[ idx ];
		idx ++;
		if ( idx >= v.size() )
		{
			// we're done!
			break;
		}
		// otherwise we put a "." at the end of the string prior to adding a new member
		name += ".";
	}

	return name;
}


std::string SNMPpp::OID::mibModuleName( const bool exact ) const
{
	/// @param [in] exact see SNMPpp::OID::getTree() for details.

	std::string s;
	const struct module *m = getModule( exact );
	if ( m )
	{
		s = m->name;
	}

	return s;
}


std::string SNMPpp::OID::mibModuleFile( const bool exact ) const
{
	/// @param [in] exact see SNMPpp::OID::getTree() for details.

	std::string s;
	const struct module *m = getModule( exact );
	if ( m )
	{
		s = m->file;
	}

	return s;
}


#include <iostream>
const struct tree *SNMPpp::OID::getTree( const bool exact ) const
{
	struct tree *t = NULL;

	if ( ! empty() )
	{
		t = get_tree( *this, *this, get_tree_head() );

		if ( exact )
		{
			/** @param [in] exact When set to `true` the tree returned must
			 * exactly match the OID.  By default when called with `false`, if
			 * an exact match cannot be made, then a parent or grandparent of
			 * the OID may be returned instead.  Usually, this means the
			 * required MIB is not installed or hasn't been loaded.
			 * For example, if the OID is .1.3.6.1.4.1.38322.1.1.1.3.0 but that
			 * specific device's custom MIB isn't available, then the closest
			 * grandparent likely to be returned is .1.3.6.1.4.1.  If `exact`
			 * is set to `true` then in this scenario getTree() will return
			 * a `NULL` pointer rather than the pointer representing
			 * .1.3.6.1.4.1.
			 */

			// make sure the tree returned is an exact match
			std::vector<oid> tmpOid;
			struct tree *p = t;
			while ( p != NULL )
			{
				// note how we're starting with the last value and working
				// backwards towards the root, so this OID will be reversed
				tmpOid.push_back( p->subid );
				p = p->parent;
			}

			// put the OID back into the order we need
			std::reverse( tmpOid.begin(), tmpOid.end() );

			if ( v != tmpOid )
			{
				// this isn't an exact match, so return nothing
				t = NULL;
			}
		}
	}

	return t;
}


const struct module *SNMPpp::OID::getModule( const bool exact ) const
{
	/// @param [in] exact see SNMPpp::OID::getTree() for details.

	const struct module	*m = NULL;
	const struct tree	*t = getTree( exact );
	if ( t )
	{
		m = find_module( t->modid );
	}

	return m;
}


std::ostream &operator<<( std::ostream &os, const SNMPpp::OID &o )
{
	os << o.str();

	return os;
}
