// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <map>
#include <string>
#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/OID.hpp>
#include <SNMPpp/PDU.hpp>


namespace SNMPpp
{
	// net-snmp handles PDUs in a strange manner.  Request PDUs are
	// automatically freed by net-snmp when a requests are handled, but
	// responses need to be manually freed by the caller.
	//
	// This C++ class wont try to somehow solve this issue.  So in the C++
	// destructor ~PDU() don't expect the underlying pdu to be freed.  You
	// still need to ensure request PDUs are not freed, and remember to free
	// responses.  To free responses or unused request PDUs, call PDU::free()
	// or net-snmp's snmp_free_pdu( pdu );

	typedef std::map < SNMPpp::OID, netsnmp_variable_list > MapOidVarList;

	class PDU
	{
		public:

			enum EType
			{
				kInvalid		= 0					,
				kUnknown		= 0					,	// alias
				// SNMPv1 and higher:
				kGet			= SNMP_MSG_GET		,	// get the specified OIDs
				kGetNext		= SNMP_MSG_GETNEXT	,	// get the OID that comes after the one specified
				kResponse	= SNMP_MSG_RESPONSE	,	// ???
				kSet			= SNMP_MSG_SET		,	// set the specified OID
				// SNMPv2 and higher:
				kGetBulk		= SNMP_MSG_GETBULK	,	// get many OIDs at once
				kInform		= SNMP_MSG_INFORM	,	// ???
				kTrap2		= SNMP_MSG_TRAP2		,	// traps
				kReport		= SNMP_MSG_REPORT		// ???
			};

			virtual ~PDU( void );
			PDU( const EType t );
			PDU( netsnmp_pdu *p ); // inherit a PDU constructed elsewhere

			// free up the underlying net-snmp structure: snmp_free_pdu(...)
			virtual void free( void );

			// get the underlying PDU type
			virtual EType getType( void ) const { return type; }

			// if a net-snmp function has been called which we know has already
			// freed the netsmp_pdu pointer, we call clear() to ensure the C++
			// wrapper object doesn't keep the old pointer
			virtual void clear( void ) { type = SNMPpp::PDU::kInvalid; pdu = NULL; }

			// make a copy of this PDU: snmp_clone_pdu()
			virtual PDU clone( void ) const;

			// easily convert it to the base net-snmp type for passing into net-snmp API
			virtual operator netsnmp_pdu*( void ) { return pdu; }

			virtual size_t size( void ) const;	// O(n)
			virtual bool empty( void ) const;		// O(1)

			// Equivalent to snmp_add_null_var() which adds a placeholder in
			// the PDU for this OID.  The "null" refers to how a type and
			// value aren't specified, just the OID.  This is typically used
			// when retrieving values from a SNMP server.
			virtual PDU &addNullVar( const SNMPpp::OID &o );
			virtual PDU &addNullVar( const SNMPpp::SetOID &s ); // add many vars at once
			virtual PDU &addNullVar( const SNMPpp::VecOID &v ); // add many vars at once
			
			// Get the OIDs that have been added to this PDU.  This only
			// returns a container of OIDs, not any values for those OIDs.
			// Use valueAt() or one of the getXXX() methods if you want the
			// value associated with an OID.
			virtual const PDU &getOids( SNMPpp::SetOID &s ) const; // automatically ordered by std::set, ignores duplicates
			virtual const PDU &getOids( SNMPpp::VecOID &v ) const; // in the exact same order as the PDU, may contain duplicates

			// get a map of OID -> net-snmp's varlist for every OID in this PDU
			virtual MapOidVarList getMap( void ) const;

			// see if the given OID is in the PDU
			virtual bool contains( const SNMPpp::OID &o ) const;

			// The net-snmp type "netsnmp_variable_list" represents a single
			// variable, and netsnmp_variable_list contains everything the PDU
			// knows about the variable in question.  This will throw if the
			// requested OID does not exist in the PDU.
			virtual const netsnmp_variable_list &at( const SNMPpp::OID &o ) const;

			// Return the first OID in the PDU.  This will throw if the PDU is empty.
			virtual SNMPpp::OID firstOID( void ) const;

			// Return the first net-snmp variable list in the PDU.  This will throw if the PDU is empty.
			virtual const netsnmp_variable_list &firstVL( void ) const;

			// see net-snmp/library/asn1.h with types such as:
			//
			//		ASN_BOOLEAN		-> 1
			//		ASN_INTEGER		-> 2
			//		ASN_BIT_STR		-> 3
			//		ASN_OCTET_STR	-> 4
			//		ASN_NULL			-> 5
			//		ASN_OBJECT_ID	-> 6
			//		ASN_SEQUENCE		-> 16
			//		ASN_SET			-> 17
			//
			virtual int asnType( const SNMPpp::OID &o ) const { return at(o).type; }

			// retrieve the net-snmp value structure associated with an OID
			// (also see at(o).val_len which may be required to properly
			// interpret some values, such as strings and OIDs)
			virtual const netsnmp_vardata &valueAt( const SNMPpp::OID &o ) const { return at(o).val; }

			// If the type is known, then easily retrieve the value from the
			// PDU.  This will throw if the OID is not in the PDU, or if the
			// type doesn't match.  For example, a string OID cannot be
			// retrieved using getBool().
			// Also see asString() which does automatically converts and
			// interprets many of the simple ASN values as a text string.
			virtual bool			getBool		( const SNMPpp::OID &o ) const;
			virtual long			getLong		( const SNMPpp::OID &o ) const;
			virtual std::string	getString	( const SNMPpp::OID &o ) const;
			virtual SNMPpp::OID	getOID		( const SNMPpp::OID &o ) const;

			// convert any of the known basic ASN types to a easy-to-use text string
			// (e.g., booleans are converted to "true" and "false", etc.)
			virtual std::string	asString		( const SNMPpp::OID &o ) const;

		protected:

			EType type;
			netsnmp_pdu *pdu;
	};
};

std::ostream &operator<<( std::ostream &os, const SNMPpp::PDU &pdu );
