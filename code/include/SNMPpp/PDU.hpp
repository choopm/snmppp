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
	/** Wrapper for net-snmp's PDU structures.
	 * 
	 * These objects are extremely small (just a pointer) and can easily
	 * be created on the stack or as a member of another class.
	 *
	 * Many other parts of SNMPpp can accept PDU objects by reference, and
	 * PDU includes the necessary operators so they can be used wherever
	 * net-snmp `netsnmp_pdu *` pointers are typically used.
	 *
	 * @note Request PDUs are automatically freed by net-snmp when requests
	 * are handled, but responses typically need to be manually freed by the
	 * caller to prevent memory leaks.
	 *
	 * This C++ class doesn't solve this problem.  In the C++ PDU destructor
	 * don't expect the underlying pdu to be freed.  You still need to ensure
	 * response PDUs are freed, either by calling SNMPpp::PDU::free() or
	 * net-snmp's `snmp_free_pdu( pdu )`.
	 */
	class PDU
	{
		public:

			/** When creating a new PDU, the type of PDU (how it will be used
			 * needs to be declared so it can be created correctly.
			 */
			enum EType
			{
				kInvalid        = 0                 ,
				kUnknown        = 0                 ,   // alias
				// SNMPv1 and higher:
				kGet            = SNMP_MSG_GET      ,   // get the specified OIDs
				kGetNext        = SNMP_MSG_GETNEXT  ,   // get the OID that comes after the one specified
				kResponse       = SNMP_MSG_RESPONSE ,   // ???
				kSet            = SNMP_MSG_SET      ,   // set the specified OID
				// SNMPv2 and higher:
				kGetBulk        = SNMP_MSG_GETBULK  ,   // get many OIDs at once
				kInform         = SNMP_MSG_INFORM   ,   // traps with replies
				kTrap2          = SNMP_MSG_TRAP2    ,   // traps
				kReport         = SNMP_MSG_REPORT       // ???
			};

			/** Destructor.
			 */
			virtual ~PDU( void );

			/** Create a PDU of the given type.
			 */
			PDU( const EType t );

			/** Inherit a PDU from net-snmp.  It is valid for `p` to be NULL.
			 */
			PDU( netsnmp_pdu *p );

			/** Free up the underlying net-snmp structure using
			 * `snmp_free_pdu()`.
			 */
			virtual void free( void );

			/** Get the PDU type.
			 */
			virtual EType getType( void ) const { return type; }

			/** If a net-snmp function has been called which we know has
			 * already freed the PDU pointer, call clear() to ensure this C++
			 * wrapper object doesn't keep the pointer to the old memory
			 * location.
			 * @note Don't miss the difference between clear() and free().
			 * This one will leak memory if `snmp_free_pdu()` hasn't yet
			 * been called.
			 * @see free()
			 */
			virtual void clear( void );

			/** Returns `TRUE` if the PDU or the varlist is `NULL`.
			 */
			virtual bool empty( void ) const;

			/** Make a copy of this PDU using `snmp_clone_pdu()`.  Both of the
			 * PDU objects will eventually need to be freed.
			 */
			virtual PDU clone( void ) const;

			/** Convert the PDU to a net-snmp pointer for passing into the
			 * net-snmp API.
			 */
			virtual operator netsnmp_pdu*( void ) { return pdu; }

			/** Get access to the varlist for this PDU.  This will throw if
			 * the PDU is empty.
			 */
			virtual const SNMPpp::Varlist varlist( void ) const;

			/** Get access to the varlist for this PDU.  This will throw if
			 * the PDU is empty.
			 */
			virtual SNMPpp::Varlist varlist( void );

			/** Get access to the varlist for this PDU.  This will throw if
			 * the PDU is empty.
			 */
			virtual operator netsnmp_variable_list *( void );

			/** Get access to a specific varlist for this PDU.
			 */
			virtual netsnmp_variable_list * operator[]( const size_t idx );

			/** Free the existing variable list and use this one instead.
			 */
			virtual PDU &setVarlist( Varlist &vl );

			/** Free the existing variable list and use this one instead.
			 */
			virtual PDU &setVarlist( netsnmp_variable_list *vl );

			// for convenience, expose a few of the Varlist members we're likely to use in simple cases

			/** Return the size of the variable list or zero if the PDU is empty.
			 */
			virtual size_t size( void ) const;

			/** Return `TRUE` if the variable list contains the given OID.
			 */
			virtual bool contains( const SNMPpp::OID &o ) const;

			/** Return the first OID object in the variable list.
			 */
			virtual SNMPpp::OID firstOID( void ) const;

			/** Add an OID as ASN_NULL to the variable list.
			 */
			virtual PDU &addNullVar( const SNMPpp::OID &o );

			/** Add numerous OIDs to the variable list.
			 * @see SNMPpp::PDU::addNullVar( const SNMPpp::OID &o )
			 * @see SNMPpp::PDU::addNullVars( const SNMPpp::VecOID &v )
			 */
			virtual PDU &addNullVars( const SNMPpp::SetOID &s );

			/** Add numerous OIDs to the variable list.
			 * @see SNMPpp::PDU::addNullVar( const SNMPpp::OID &o )
			 * @see SNMPpp::PDU::addNullVars( const SNMPpp::SetOID &s )
			 */
			virtual PDU &addNullVars( const SNMPpp::VecOID &v );

		protected:

			EType type;
			netsnmp_pdu *pdu; // beware -- this pointer *can* be null if a PDU hasn't been defined or if it has been clear()
	};
};


/** Can be used to log or display some debugging information on the PDU,
 * including all OIDs contained within the Varlist.
 */
std::ostream &operator<<( std::ostream &os, const SNMPpp::PDU &pdu );
