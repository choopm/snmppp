// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <vector>
#include <string>
#include <iostream>


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
				kPrivateEnterprise	= 3		// .1.3.6.1.4
			};

			virtual ~OID( void );

			// empty OID, does not have any value:
			//		OID oid1;
			OID( void );

			// copy an OID from an existing object
			OID( const OID &oid );

			// initialize using a text string with the familiar numeric value:
			//		OID oid( ".1.3.6.1.4" );
			OID( const char * const str );
			OID( const std::string &str );

			// initialize using one of the common SNMP OID locations
			//		OID oid( SNMPpp::OID::kInternet ); // .1.3.6
			OID( const SNMPpp::OID::ECommon &location );

			// convert the OID to the familiar numeric format, such as ".1.3.6.1.4.x.x.x"
			virtual operator std::string( void ) const;

			// clear the OID value in the object (clear the vector)
			virtual OID &clear( void );

			// return TRUE if the OID object is empty (no OID specified)
			virtual bool empty( void ) const { return v.empty(); }

			// return the number of values in the OID
			virtual size_t size( void ) const { return v.size(); }

			virtual bool operator==( const SNMPpp::OID &rhs ) const { return v == rhs.v; }
			virtual bool operator!=( const SNMPpp::OID &rhs ) const { return v != rhs.v; }

			// append a single numeric value to the end of the existing OID
			//		OID oid( ".1.3.6" );
			//		oid += 1;				// == .1.3.6.1
			virtual OID  operator+ ( const unsigned int i ) const;
			virtual OID &operator+=( const unsigned int i );

			// append one or more numeric values to the end of the existing OID
			//		OID oid( ".1.3.6" );
			//		oid += ".1.4";			// == .1.3.6.1.4
			virtual OID  operator+ ( const std::string &str ) const;
			virtual OID &operator+=( const std::string &str );

			// reuse an OID by setting the numeric value as indicated
			virtual OID &set( const OID &oid );
			virtual OID &set( const char * const str );
			virtual OID &set( const std::string &str );
			virtual OID &set( const SNMPpp::OID::ECommon &location );
			virtual OID &operator=( const std::string &str ) { return set( str ); }
			virtual OID &operator=( const char * const str ) { return set( str ); }

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

			std::vector < unsigned int > v;
	};
};

std::ostream &operator<<( std::ostream &os, const SNMPpp::OID &oid );
