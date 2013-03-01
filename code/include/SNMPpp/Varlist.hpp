// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// @copyright Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/OID.hpp>


namespace SNMPpp
{
	/** Wrapper for net-snmp's snmp_variable_list pointer.
	 *
	 * These objects are extremely small (just a pointer) and can easily be
	 * created on the stack or as a member of another class.
	 *
	 * Many other parts of SNMPpp can accept Varlist objects by reference, and
	 * Varlist includes the necessary operators so they can be used wherever
	 * snmp_variable_list* pointers are typically used.
	 */
	class Varlist
	{
		public:

			/** Destructor.
			 * @note The destructor does *not* free up the underlying
			 * netsnmp_variable_list pointer!  This isn't done automatically
			 * since many of the net-snmp APIs that act on variable lists
			 * already free up or take ownership of varlists.
			 *
			 * If you aren't calling a net-snmp function that frees up the
			 * varlist pointer, remember to call free() prior to destructing
			 * the object.
			 * @see clear();
			 * @see free();
			 */
			virtual ~Varlist( void );

			/** Create a new object. The underlying netsnmp_variable_list
			 * pointer will be NULL until an OID is added to the Varlist.
			 */
			Varlist( void );

			/** Create a new object with the given netsnmp_variable_list
			 * pointer.  Using a NULL pointer is valid, it creates an empty
			 * object just like the other Varlist() constructor.
			 */
			Varlist( netsnmp_variable_list *vl );

			/** Free up the net-snmp structure by calling snmp_free_varbind().
			 * @see clear();
			 */
			virtual void free( void );

			/** If a net-snmp function has been called which we know has
			 * already freed the netsmp_varlist pointer, call clear() to
			 * ensure this C++ wrapper object doesn't keep the pointer to the
			 * old memory location.
			 * @note Don't miss the difference between clear() and free().
			 * This one will leak memory if snmp_free_varbind() hasn't yet
			 * been called.
			 * @see free();
			 */
			virtual void clear( void ) { varlist = NULL; }

			/** Easily convert Varlist to the base net-snmp type for passing into net-snmp API.
			 * Will return NULL if the varlist is empty.
			 */
			virtual operator netsnmp_variable_list* ( void ) { return  varlist; }
			
			/** Easily convert Varlist to the base net-snmp type for passing into net-snmp API.
			 */
			virtual operator netsnmp_variable_list**( void ) { return &varlist; }

			/** Return the number of variables in this object.  Runs in O(n) as
			 * it needs to traverse the entire linked list every time it is
			 * called.  (The size is not cached anywhere in Varlist.)
			 */
			virtual size_t size( void ) const { return count_varbinds( varlist ); }

			/** Return `TRUE` if the netsnmp_variable_list pointer is NULL.
			 * This normally means no OIDs have been added, or free() has
			 * been called.
			 */
			virtual bool empty( void ) const { return varlist == NULL; }

			/** Get a std::vector of all the OIDs in the varlist.
			 * @note The vector will retain the same position and duplicates
			 * as `netsnmp_variable_list`.
			 * @see SNMPpp::Varlist::getOids( SNMPpp::SetOID &s ) const;
			 */
			virtual const Varlist &getOids( SNMPpp::VecOID &v ) const;

			/** Get a std::set of all the OIDs in the varlist.
			 * @note Unlike `snmp_variable_list` and std::vector, the std::set
			 * will not contain duplicate entries.
			 * @see SNMPpp::Varlist::getOids( SNMPpp::VecOID &v ) const;
			 */
			virtual const Varlist &getOids( SNMPpp::SetOID &s ) const;

			/** Get a map of OID -> net-snmp's varlist for every OID in this
			 * varlist.
			 */
			virtual MapOidVarList getMap( void ) const;

			/** See if the given OID is in the varlist.
			 */
			virtual bool contains( const SNMPpp::OID &o ) const;

			/** Equivalent to snmp_add_null_var() which adds a placeholder in
			 * the PDU for this OID.  The "null" refers to how a type and value
			 * value aren't specified, just the OID.  This is typically used
			 * when retrieving values from a SNMP server.
			 */
			virtual Varlist &addNullVar( const SNMPpp::OID &o );

			/** Same as addNullVar(), but allows multiple OIDs to be added at
			 * once by using a std::set of OID objects.  Calling with an empty
			 * set is perfectly valid.
			 */
			virtual Varlist &addNullVars( const SNMPpp::SetOID &s );

			/** Same as addNullVar(), but allows multiple OIDs to be added at
			 * at once by using a std::vector of OID objects.  Calling with an
			 * empty vector is perfectly valid.
			 */
			virtual Varlist &addNullVars( const SNMPpp::VecOID &v );

			/** Return the underlying netsnmp_variable_list pointer which
			 * describes the given OID object.  The net-snmp type contains
			 * everything net-snmp currently knows about an OID, including
			 * a pointer to the next variable which is how net-snmp uses this
			 * structure to describe both specific OIDs and vectors of OIDs.
			 * This method will throw if the requested OID does not exist
			 * in the varlist.
			 */
			virtual const netsnmp_variable_list *at( const SNMPpp::OID &o ) const;

			/** Return the [N]th netsnmp_variable_list pointer.
			 */
			virtual const netsnmp_variable_list *operator[]( const size_t idx ) const;
			
			/** Return the first OID object in the varlist.  This will throw if
			 * the varlist is empty.
			 */
			virtual SNMPpp::OID firstOID( void ) const;

			/** Return the ASN type for the given OID.  This will throw if the
			 * varlist object does not contain the specified OID.  The ASN
			 * types are defined in net-snmp.  For example, see net-snmp/library/asn1.h:
			 *
			 * NET-SNMP TYPE NAME	|	NET-SNMP TYPE VALUE
			 * :-----------------	|	------------------:
			 * ASN_BOOLEAN			|	1
			 * ASN_INTEGER			|	2
			 * ASN_BIT_STR			|	3
			 * ASN_OCTET_STR			|	4
			 * ASN_NULL				|	5
			 * ASN_OBJECT_ID			|	6
			 * ASN_SEQUENCE			|	16
			 * ASN_SET				|	17
			 * ASN_IPADDRESS			|	64
			 * ASN_COUNTER			|	65
			 * ASN_GAUGE				|	66
			 * ASN_TIMETICKS			|	67
			 */
			virtual int asnType( const SNMPpp::OID &o ) const { return at(o)->type; }

			/** Retrieve the net-snmp value structure associated with an OID.
			 * Also see at().val_len which may be required to properly
			 * interpret some values, such as strings and OIDs.
			 */
			virtual const netsnmp_vardata &valueAt( const SNMPpp::OID &o ) const { return at(o)->val; }

			/** Retrieve the value of the given boolean OID.
			 * @see SNMPpp::Varlist::asString() which automatically converts and interprets
			 * known types to a text string.
			 */
			virtual bool			getBool		( const SNMPpp::OID &o ) const;

			/** Retrieve the value of the given long OID.
			 * @see SNMPpp::Varlist::asString() which automatically converts and interprets
			 * known types to a text string.
			 */
			virtual long			getLong		( const SNMPpp::OID &o ) const;

			/** Retrieve the value of the given string OID.
			 * @see SNMPpp::Varlist::asString() which automatically converts and interprets
			 * known types to a text string.
			 */
			virtual std::string	getString	( const SNMPpp::OID &o ) const;

			/** Retrieve the value of the given object OID.
			 * @see SNMPpp::Varlist::asString() which automatically converts and interprets
			 * known types to a text string.
			 */
			virtual SNMPpp::OID	getOID		( const SNMPpp::OID &o ) const;

			/** Convert any of the known basic ASN types to a easy-to-use text
			 * string.  E.g., booleans are converted to the text strings
			 * "true" and "false", etc.
			 */
			virtual std::string	asString		( const SNMPpp::OID &o ) const;

		protected:

			/** This is the basic varlist pointer from net-snmp.  Beware, this
			 * pointer will be NULL when a varlist is empty.
			 */
			netsnmp_variable_list *varlist;
	};
};


/** Can be used to log or display some debugging information on the varlist,
 * including all OIDs contained within the varlist.
 */
std::ostream &operator<<( std::ostream &os, const SNMPpp::Varlist &varlist );
