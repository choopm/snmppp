// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <set>
#include <vector>
#include <string>
#include <ostream>


namespace SNMPpp
{
	class OID
	{
		public:

			enum ECommon
			{
				// several common locations useful as a starting point
				kInvalid				= 0,
				kEmpty				= 1,
				kInternet			= 2,		// .1.3.6
				kPrivateEnterprise	= 3,		// .1.3.6.1.4
				kSysUpTime			= 4,		// .1.3.6.1.2.1.1.3.0
				kTrap				= 5		// .1.3.6.1.6.3.1.1.4.1.0
			};

			virtual ~OID( void );

			// empty OID, does not have any value:
			//		OID oid1;
			OID( void );

			// copy an OID from an existing object
			OID( const OID &oid );

			// initialize using a text string with the familiar numeric value:
			//		OID oid( ".1.3.6.1.4" );
			OID( const char * const s );
			OID( const std::string &s );

			// initialize using one of the common SNMP OID locations
			//		OID oid( SNMPpp::OID::kInternet ); // .1.3.6
			OID( const SNMPpp::OID::ECommon &location );

			// initialize using the net-snmp "oid *" array
			OID( const unsigned long *l, const size_t len );

			// convert the OID to the familiar numeric format, such as ".1.3.6.1.4.x.x.x"
			virtual operator std::string( void ) const;
			virtual std::string str( void ) const { return operator std::string(); }

			// convert the OID to an array of unsigned longs ("oid *") the way net-snmp needs
			virtual operator const unsigned long *( void ) const;

			// some of the net-snmp API calls convert everything to uchar*
			virtual operator const unsigned char *( void ) const { return (unsigned char *)operator const unsigned long *(); }

			// clear the OID value in the object (clear the vector)
			virtual OID &clear( void );

			// return TRUE if the OID object is empty (no OID values specified)
			virtual bool empty( void ) const { return v.empty(); }

			// return the number of values in the OID vector
			//
			// remember this is the number of UNSIGNED LONG, while some of the
			// net-snmp API calls expect the number of bytes, in which case you
			// must multiply by sizeof(ulong)
			virtual operator size_t	( void ) const { return v.size(); }
			virtual size_t size		( void ) const { return v.size(); }
			virtual size_t len		( void ) const { return v.size(); }

			// need the ability to order OIDs so they can be stored in maps and sets
			// (if we do operator<() we may as well include the other ones to be complete)
			virtual bool operator< ( const SNMPpp::OID &rhs ) const { return v <  rhs.v; }
			virtual bool operator<=( const SNMPpp::OID &rhs ) const { return v <= rhs.v; }
			virtual bool operator> ( const SNMPpp::OID &rhs ) const { return v >  rhs.v; }
			virtual bool operator>=( const SNMPpp::OID &rhs ) const { return v >= rhs.v; }
			virtual bool operator==( const SNMPpp::OID &rhs ) const { return v == rhs.v; }
			virtual bool operator!=( const SNMPpp::OID &rhs ) const { return v != rhs.v; }

			// append a single numeric value to the end of the existing OID
			//		OID oid( ".1.3.6" );
			//		oid += 1;				// == .1.3.6.1
			virtual OID  operator+ ( const unsigned long l ) const;
			virtual OID &operator+=( const unsigned long l );

			// append one or more numeric values to the end of the existing OID
			//		OID oid( ".1.3.6" );
			//		oid += ".1.4";			// == .1.3.6.1.4
			virtual OID  operator+ ( const std::string &s ) const;
			virtual OID &operator+=( const std::string &s );

			// reuse an OID by setting the numeric value as indicated
			virtual OID &set( const OID &oid );
			virtual OID &set( const char * const s );
			virtual OID &set( const std::string &s );
			virtual OID &set( const SNMPpp::OID::ECommon &location );
			virtual OID &operator=( const std::string &s ) { return set( s ); }
			virtual OID &operator=( const char * const s ) { return set( s ); }

			// is this object a child of "rhs"?  (note this includes grandchildren, etc)
			//
			//		OID oid( ".1.3.6.4.1" );
			//		...
			//		if ( oid.isChildOf( ".1.3.6" )	// == TRUE
			virtual bool isChildOf(  const SNMPpp::OID &rhs ) const;

			// is this object the parent of "rhs"?  (note this includes grandparents, etc)
			//
			//		OID oid( ".1.3.6" );
			//		if ( oid.isParentOf( "1.3.6.4.1" )	// == TRUE
			virtual bool isParentOf( const SNMPpp::OID &rhs ) const;

			// same as isChildOf() and isParentOf(), but the child<->parent relationship must be exact
			//
			//		OID oid( ".1.3.6.4.1" );
			//		...
			//		if ( oid.isImmediateChildOf( "1.3.6" )	// == FALSE
			//		...
			//		if ( oid.isImmediateChildOf( "1.3.6.4" )	// == TRUE
			virtual bool isImmediateChildOf ( const SNMPpp::OID &rhs ) const;
			virtual bool isImmediateParentOf( const SNMPpp::OID &rhs ) const;

		protected:

			std::vector < unsigned long > v;
	};

	typedef std::set   <OID> SetOID;
	typedef std::vector<OID> VecOID;
};

std::ostream &operator<<( std::ostream &os, const SNMPpp::OID &o );
