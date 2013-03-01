// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <map>
#include <set>
#include <vector>
#include <string>
#include <ostream>
#include <SNMPpp/net-snmppp.hpp>


namespace SNMPpp
{
	/** Wrapper for net-snmp's OID arrays.
	 * 
	 * These objects are extremely small (just a std::vector) and can easily
	 * be created on the stack or as a member of another class.
	 *
	 * Many other parts of SNMPpp can accept OID objects by reference, and
	 * OID includes the necessary operators so they can be used wherever
	 * net-snmp `oid` arrays are typically used.
	 */
	class OID
	{
		public:

			/** An enum to represent some of the common OIDs applications may need.
			 *
			 * Value					| Oid
			 * -----					| ---
			 * kEmpty				| n/a
			 * kInternet				| .1.3.6.1
			 * kPrivateEnterprise	| .1.3.6.1.4
			 * kSysUpTime			| .1.3.6.1.2.1.1.3.0
			 * kTrap					| .1.3.6.1.6.3.1.1.4.1.0
			 */
			enum ECommon
			{
				// several common locations useful as a starting point
				kInvalid				= 0,
				kEmpty				= 1,
				kInternet			= 2,
				kPrivateEnterprise	= 3,
				kSysUpTime			= 4,
				kTrap				= 5
			};

			/** An enum to indicate what kind of name lookup is required when
			 * calling SNMPpp::OID::nameFromMib().
			 *
			 * Value			| Meaning									| Example
			 * -----			| -------									| -------
			 * kLeafOnly		| Only the very last name is returned.		| .1.3.6.1 -> "internet"
			 * kPartial		| Start of OID combined with the last name.	| .1.3.6.1 -> ".1.3.6.internet"
			 * kInverted		| The opposite of kPartial.					| .1.3.6.1 -> ".iso.org.dod.1"
			 * kFull         | All names are looked up.					| .1.3.6.1 -> ".iso.org.dod.internet"
			 */
			enum ENameLookup
			{
				kUnknown				= 0,
				kLeafOnly			= 1,
				kPartial				= 2,
				kInverted			= 3,
				kFull				= 4
			};

			/** Destructor.
			 */
			virtual ~OID( void );

			/** Empty OID, does not have any value.
			 * For example:
			 *
			 *		OID oid1;
			 */
			OID( void );

			/** Copy an OID from an existing object.
			 */
			OID( const OID &oid );

			/** Initialize using a text string with the familiar numeric value.
			 * For example:
			 *
			 *		OID oid1( ".1.3.6.1.4" );
			 */
			OID( const char * const s );

			/** Similar to OID( const char * const s ).
			 */
			OID( const std::string &s );

			/** Construct an OID from one of the few common locations described by SNMPpp::OID::ECommon.
			 * For example:
			 *
			 *		OID oid1( SNMPpp::OID::kInternet ); // .1.3.6
			 */
			OID( const SNMPpp::OID::ECommon &location );

			/** Construct an OID using a net-snmp `oid` array.
			 */
			OID( const oid * o, const size_t len );

			/** Construct an OID from the first OID in the given variable list
			 * pointer.  It is perfectly valid to use a NULL pointer.
			 *
			 * This constructor is `explicit` to prevent accidentally
			 * converting an entire SNMPpp::Varlist to a single OID.  It
			 * requires and explicit netsnmp_variable_list * pointer.
			 */
			explicit OID( const netsnmp_variable_list *vl );

			/** Convert the OID to the familiar numeric format, such as `.1.3.6.1.4.x.x.x`.
			 */
			virtual operator std::string( void ) const;

			/** Alias to operator std::string() for convenience.  Sometimes it
			 * is cleaner to call str() than the wordier `operator...()`.
			 */
			virtual std::string str( void ) const { return operator std::string(); }

			/** Convert the const OID to an array of unsigned longs
			 * (aka `oid *`) the way net-snmp needs for most API calls.
			 */
			virtual operator const oid *( void ) const;

			/** Convert the OID to an array of unsigned longs (aka `oid *`)
			 * the way net-snmp needs for most API calls.
			 */
			virtual operator oid *( void );

			/** Convert the OID to a `uchar*` the way a few of the net-snmp API
			 * requires.
			 */
			virtual operator const unsigned char *( void ) const { return (unsigned char *)operator const oid *(); }

			/** Clear the OID value in the object (clears the vector).
			 */
			virtual OID &clear( void );

			/** Return `TRUE` if the OID object is completely empty.
			 */
			virtual bool empty( void ) const { return v.empty(); }

			/** Return `TRUE` if the OID object is not empty.
			 */
			virtual operator bool( void ) const { return ! empty(); }

			/** Return the number of values in the OID vector.
			 * @note Remember this is the number of `unsigned long`, while some
			 * of the net-snmp API calls expect the number of bytes, in which
			 * case you must multiply by `sizeof(ulong)`.
			 */
			virtual operator size_t	( void ) const { return v.size(); }

			/** Alias for operator size_t().
			 */
			virtual size_t size		( void ) const { return v.size(); }

			/** Alias for operator size_t().
			 */
			virtual size_t len		( void ) const { return v.size(); }

			/** Comparison operator for OIDs.
			 */
			virtual bool operator< ( const SNMPpp::OID &rhs ) const { return v <  rhs.v; }

			/** Comparison operator for OIDs.
			 */
			virtual bool operator<=( const SNMPpp::OID &rhs ) const { return v <= rhs.v; }

			/** Comparison operator for OIDs.
			 */
			virtual bool operator> ( const SNMPpp::OID &rhs ) const { return v >  rhs.v; }

			/** Comparison operator for OIDs.
			 */
			virtual bool operator>=( const SNMPpp::OID &rhs ) const { return v >= rhs.v; }

			/** Comparison operator for OIDs.
			 */
			virtual bool operator==( const SNMPpp::OID &rhs ) const { return v == rhs.v; }

			/** Comparison operator for OIDs.
			 */
			virtual bool operator!=( const SNMPpp::OID &rhs ) const { return v != rhs.v; }

			/** Append a single numeric value and return a new OID.
			 * For example:
			 *
			 *		OID oid1( ".1.3.6" );
			 *		OID oid2 = oid1 + 1;		// == .1.3.6.1
			 */
			virtual OID operator+ ( const oid o ) const;

			/** Append a single numeric value to the current OID.
			 * For example:
			 *
			 *		OID oid1( ".1.3.6" );
			 *		oid1 += 1;				// == .1.3.6.1
			 */
			virtual OID &operator+=( const oid o );

			/** Append one or more values and return a new OID.
			 * For example:
			 *
			 *		OID oid1( ".1.3.6" );
			 *		OID oid2 = oid1 + ".1.4";		// == .1.3.6.1.4
			 */
			virtual OID  operator+ ( const std::string &s ) const;

			/** Append one or more values to the current OID.
			 * For example:
			 *
			 *		OID oid1( ".1.3.6" );
			 *		oid1 += ".1.4";			// == .1.3.6.1.4
			 */
			virtual OID &operator+=( const std::string &s );

			/** Return the value at the specified index into the OID vector.
			 * For example:
			 *
			 *		OID o( ".1.3.6" );
			 *		if ( o[0] == 1 ) ...	// TRUE
			 *		if ( o[2] != 6 ) ...	// FALSE
			 */
			virtual oid operator[]( const size_t idx ) const;

			/** Reuse an OID object by setting the numeric values as indicated.
			 */
			virtual OID &set( const OID &oid );

			/** Reuse an OID object by setting the numeric values as indicated.
			 */
			virtual OID &set( const char * const s );

			/** Reuse an OID object by setting the numeric values as indicated.
			 */
			virtual OID &set( const std::string &s );

			/** Reuse an OID object by setting the numeric values as indicated.
			 */
			virtual OID &set( const SNMPpp::OID::ECommon &location );

			/** Alias for SNMPpp::OID::set().
			 */
			virtual OID &operator=( const std::string &s ) { return set( s ); }

			/** Alias for SNMPpp::OID::set().
			 */
			virtual OID &operator=( const char * const s ) { return set( s ); }

			/** Similar to SNMPpp::OID::isImmediateChildOf().  But this
			 * includes grandchildren, etc.  For example:
			 *
			 * 		OID oid( ".1.3.6.4.1" );
			 * 		...
			 * 		if ( oid.isChildOf( ".1.3.6" )  // == TRUE
			 *
			 * @see SNMPpp::OID::isImmediateChildOf()
			 * @see SNMPpp::OID::isImmediateParentOf()
			 * @see SNMPpp::OID::isParentOf()
			 */
			virtual bool isChildOf(  const SNMPpp::OID &rhs ) const;

			/** Similar to SNMPpp::OID::isImmediateParentOf().  But this
			 * includes grandparents, etc.  For example:
			 *
			 * 		OID oid( ".1.3.6" );
			 * 		if ( oid.isParentOf( "1.3.6.4.1" )  // == TRUE
			 *
			 * @see SNMPpp::OID::isImmediateChildOf()
			 * @see SNMPpp::OID::isImmediateParentOf()
			 * @see SNMPpp::OID::isChildOf()
			 */
			virtual bool isParentOf( const SNMPpp::OID &rhs ) const;

			/** Similar to SNMPpp::OID::isChildOf().  The child<->parent
			 * relationship must be exact.  Grandparents and grandchildren
			 * will return `FALSE`.  For example:
			 *
			 * 		OID oid( ".1.3.6.4.1" );
			 * 		...
			 * 		if ( oid.isImmediateChildOf( "1.3.6" )    // == FALSE
			 * 		...
			 * 		if ( oid.isImmediateChildOf( "1.3.6.4" )  // == TRUE
			 *
			 * @see SNMPpp::OID::isImmediateParentOf()
			 * @see SNMPpp::OID::isChildOf()
			 * @see SNMPpp::OID::isParentOf()
			 */

			virtual bool isImmediateChildOf ( const SNMPpp::OID &rhs ) const;

			/** Similar to SNMPpp::OID::isParentOf().  The child<->parent
			 * relationship must be exact.  Grandparents and grandchildren
			 * will return `FALSE`.  For example:
			 *
			 * 		OID oid( ".1.3.6" );
			 * 		...
			 * 		if ( oid.isImmediateParentOf( "1.3.6.4.1" )   // == FALSE
			 * 		...
			 * 		if ( oid.isImmediateParentOf( "1.3.6.4" )     // == TRUE
			 *
			 * @see SNMPpp::OID::isImmediateChildOf()
			 * @see SNMPpp::OID::isChildOf()
			 * @see SNMPpp::OID::isParentOf()
			 */
			virtual bool isImmediateParentOf( const SNMPpp::OID &rhs ) const;

			/** Get the parent OID.
			 */
			virtual OID parent( const size_t level=1 ) const;

			/** Get the OID's name from MIB files (if possible).
			 * @see SNMPpp::OID::ENameLookup
			 */
			virtual std::string nameFromMib( const SNMPpp::OID::ENameLookup lookup=SNMPpp::OID::kFull ) const;

	protected:

			std::vector < oid > v;
	};

	/** A std::set of OIDs.
	 */
	typedef std::set   <OID> SetOID;

	/** A std::vector of OIDs.
	 */
	typedef std::vector<OID> VecOID;

	/** A std::map where the OID is the key and the value is a net-snmp
	 * variable list.  This is used by SNMPpp::PDU and SNMPpp::Varlist.
	 */
	typedef std::map < SNMPpp::OID, netsnmp_variable_list *> MapOidVarList;
};


/** Can be used to log or display an OID.  This does the exact same thing
 * as calling SNMPpp::OID::str().
 */
std::ostream &operator<<( std::ostream &os, const SNMPpp::OID &o );
