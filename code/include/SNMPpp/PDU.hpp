// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <map>
#include <string>
#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/OID.hpp>
#include <SNMPpp/Varlist.hpp>


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
				kInform		= SNMP_MSG_INFORM	,	// traps with replies
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
			virtual void clear( void );

			// returns TRUE if the PDU or the varlist is NULL
			virtual bool empty( void ) const;

			// make a copy of this PDU: snmp_clone_pdu()
			virtual PDU clone( void ) const;

			// easily convert the PDU to the base net-snmp type for passing into net-snmp API
			virtual operator netsnmp_pdu*( void ) { return pdu; }

			// get access to the varlist for this PDU (these will throw if the PDU doesn't exist)
			virtual const SNMPpp::Varlist varlist( void ) const;
			virtual SNMPpp::Varlist varlist( void );
			virtual operator netsnmp_variable_list *( void );

			// free the existing varlist and use this one instead
			virtual PDU &setVarlist( Varlist &vl );
			virtual PDU &setVarlist( netsnmp_variable_list *vl );

			// for convenience, expose a few of the Varlist members we're likely to use in simple cases

			virtual size_t size		( void )					const;
			virtual bool contains	( const SNMPpp::OID &o )	const;
			virtual PDU &addNullVar	( const SNMPpp::OID &o );
			virtual PDU &addNullVar	( const SNMPpp::SetOID &s );
			virtual PDU &addNullVar	( const SNMPpp::VecOID &v );

		protected:

			EType type;
			netsnmp_pdu *pdu; // beware -- this pointer *can* be null if a PDU hasn't been defined or if it has been clear()
	};
};

std::ostream &operator<<( std::ostream &os, const SNMPpp::PDU &pdu );
