// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/OID.hpp>


namespace SNMPpp
{
	class Varlist
	{
		public:

			virtual ~Varlist( void );
			Varlist( void );
			Varlist( netsnmp_variable_list *vl );

			// free up the underlying net-snmp structure: snmp_free_varbind()
			virtual void free( void );

			// if a net-snmp function has been called which we know has already
			// freed the netsmp_varlist pointer, we call clear() to ensure the
			// C++ wrapper object doesn't keep the old pointer
			virtual void clear( void ) { varlist = NULL; }

			// easily convert it to the base net-snmp type for passing into net-snmp API
			virtual operator netsnmp_variable_list*( void ) { return varlist; }

			virtual size_t size( void ) const { return count_varbinds( varlist ); } // O(n)
			virtual bool  empty( void ) const { return varlist == NULL; }

			// get a vector or a set of all the OIDs in the varlist
			// (note that vector will retain positon and duplicates)
			virtual const Varlist &getOids( SNMPpp::VecOID &v ) const;
			virtual const Varlist &getOids( SNMPpp::SetOID &s ) const;

			// get a map of OID -> net-snmp's varlist for every OID in this varlist
			virtual MapOidVarList getMap( void ) const;

			// see if the given OID is in the varlist
			virtual bool contains( const SNMPpp::OID &o ) const;

			// Equivalent to snmp_add_null_var() which adds a placeholder in
			// the PDU for this OID.  The "null" refers to how a type and
			// value aren't specified, just the OID.  This is typically used
			// when retrieving values from a SNMP server.
			virtual Varlist &addNullVar( const SNMPpp::OID &o );
			virtual Varlist &addNullVar( const SNMPpp::SetOID &s ); // add many vars at once
			virtual Varlist &addNullVar( const SNMPpp::VecOID &v ); // add many vars at once

			// The net-snmp type "netsnmp_variable_list" actually represents a
			// single variable (and a pointer to the next variable).  This will
			// throw if the requested OID does not exist in the varlist.
			virtual const netsnmp_variable_list *at( const SNMPpp::OID &o ) const;

			// Return the first OID in the varlist.  This will throw if the varlist is empty.
			virtual SNMPpp::OID firstOID( void ) const;

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
			virtual int asnType( const SNMPpp::OID &o ) const { return at(o)->type; }

			// retrieve the net-snmp value structure associated with an OID
			// (also see at(o).val_len which may be required to properly
			// interpret some values, such as strings and OIDs)
			virtual const netsnmp_vardata &valueAt( const SNMPpp::OID &o ) const { return at(o)->val; }

			// If the type is known, then easily retrieve the value from the
			// varlist.  This will throw if the OID is not in the varlist, or
			// if the type doesn't match.  For example, a string OID cannot be
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

			netsnmp_variable_list *varlist; // beware -- this pointer can be NULL
	};
};


std::ostream &operator<<( std::ostream &os, const SNMPpp::Varlist &varlist );
