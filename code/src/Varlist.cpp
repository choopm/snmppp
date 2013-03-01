// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <SNMPpp/Varlist.hpp>
#include <stdexcept>
#include <sstream>


SNMPpp::Varlist::~Varlist( void )
{
	return;
}


SNMPpp::Varlist::Varlist( void ) :
	varlist( NULL )
{
	return;
}


SNMPpp::Varlist::Varlist( netsnmp_variable_list *vl ) :
	varlist( vl )
{
	return;
}


// free up the underlying net-snmp structure: snmp_free_varbind()
void SNMPpp::Varlist::free( void )
{
	if ( varlist != NULL )
	{
		snmp_free_varbind( varlist );
	}
	clear();

	return;
}


const SNMPpp::Varlist &SNMPpp::Varlist::getOids( SNMPpp::SetOID &s ) const
{
	s.clear();

	for ( netsnmp_variable_list *p = varlist; p != NULL; p = p->next_variable )
	{
		s.insert( SNMPpp::OID(p) );
	}

	return *this;
}


const SNMPpp::Varlist &SNMPpp::Varlist::getOids( SNMPpp::VecOID &v ) const
{
	v.clear();

	for ( netsnmp_variable_list *p = varlist; p != NULL; p = p->next_variable )
	{
		v.push_back( SNMPpp::OID(p) );
	}

	return *this;
}


SNMPpp::MapOidVarList SNMPpp::Varlist::getMap( void ) const
{
	MapOidVarList m;

	for ( netsnmp_variable_list *p = varlist; p != NULL; p = p->next_variable )
	{
		m[ SNMPpp::OID(p) ] = p;
	}

	return m;
}


bool SNMPpp::Varlist::contains( const SNMPpp::OID &o ) const
{
	bool result = false;

	for ( netsnmp_variable_list *p = varlist; p != NULL; p  = p->next_variable )
	{
		if ( o == SNMPpp::OID(p) )
		{
			// found it!
			result = true;
			break;
		}
	}

	return result;
}



SNMPpp::Varlist &SNMPpp::Varlist::addNullVar( const SNMPpp::OID &o )
{
	if ( o.empty() )
	{
		/// @throw std::invalid_argument if the OID is empty.
		throw std::invalid_argument( "Cannot add an empty OID." );
	}

	netsnmp_variable_list *p = snmp_varlist_add_variable( &varlist, o, o, ASN_NULL, 0, 0 );
	if ( p == NULL )
	{
		/// @throw std::runtime_error if net-snmp failed to add the OID.
		throw std::runtime_error( "Failed to add " + o.str() + " to the variable list." );
	}

	return *this;
}


SNMPpp::Varlist &SNMPpp::Varlist::addNullVars( const SNMPpp::SetOID &s )
{
	SNMPpp::SetOID::const_iterator iter;
	for (	iter  = s.begin();
			iter != s.end();
			iter ++ )
	 {
		 addNullVar( *iter );
	 }

	 return *this;
}


SNMPpp::Varlist &SNMPpp::Varlist::addNullVars( const SNMPpp::VecOID &v )
{
	for ( size_t idx = 0; idx < v.size(); idx ++ )
	{
		addNullVar( v[idx] );
	}

	return *this;
}


const netsnmp_variable_list *SNMPpp::Varlist::at( const SNMPpp::OID &o ) const
{
	netsnmp_variable_list *p = NULL;
	for ( p = varlist; p != NULL; p = p->next_variable )
	{
		if ( o == SNMPpp::OID(p) )
		{
			// found it!
			break;
		}
	}

	if ( p == NULL )
	{
		/// @throw std::invalid_argument if the varlist does not contain the requested OID.
		throw std::invalid_argument( "Varlist does not contain OID " + o.str() + "." );
	}

	return p;
}


const netsnmp_variable_list *SNMPpp::Varlist::operator[]( const size_t idx ) const
{
	if ( idx >= size() )
	{
		/// @throw std::invalid_argument if the index is larger than the variable list.
		throw std::invalid_argument( "The index is larger than the number of variable list pointers." );
	}

	/** @details Runs in O(2n) since it needs to walk the linked list of
	 * netsnmp_variable_list pointers twice every time it is called.
	 * (The first is due to a call to size().)
	 */
	netsnmp_variable_list *p = varlist;
	for ( size_t counter = 0; counter < idx; counter ++ )
	{
		p = p->next_variable;
	}

	return p;
}


SNMPpp::OID SNMPpp::Varlist::firstOID( void ) const
{
	if ( empty() )
	{
		/// @throw std::logic_error if the varlist is empty.
		throw std::logic_error( "Cannot get an OID from an empty variable list." );
	}

	return SNMPpp::OID(varlist);
}


bool	 SNMPpp::Varlist::getBool( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list *vl = at( o );

	if ( vl->type != ASN_BOOLEAN )
	{
		/// @throw std::invalid_argument if the requested OID is not `ASN_BOOLEAN`.
		throw std::invalid_argument( "OID " + o.str() + " is not a boolean type." );
	}
	
	return *vl->val.integer == 1; // rfc1212, true=1, false=2
}


long	 SNMPpp::Varlist::getLong( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list *vl = at( o );

	if ( vl->type != ASN_INTEGER )
	{
		/// @throw std::invalid_argument if the requested OID is not `ASN_INTEGER`.
		throw std::invalid_argument( "OID " + o.str() + " is not a numeric type." );
	}

	return *vl->val.integer;
}


std::string SNMPpp::Varlist::getString( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list *vl = at( o );

	if ( vl->type != ASN_OCTET_STR )
	{
		/// @throw std::invalid_argument if the requested OID is not `ASN_OCTET_STR`.
		throw std::invalid_argument( "OID " + o.str() + " is not a string." );
	}

	return std::string( (const char *)vl->val.string, vl->val_len );
}


SNMPpp::OID SNMPpp::Varlist::getOID( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list *vl = at( o );

	if ( vl->type != ASN_OBJECT_ID )
	{
		/// @throw std::invalid_argument if the requested OID is not `ASN_OBJECT_ID`.
		throw std::invalid_argument( "OID " + o.str() + " is not a OID." );
	}
	
	return SNMPpp::OID( vl->val.objid, vl->val_len/sizeof(unsigned long) );
}


std::string SNMPpp::Varlist::asString( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list *vl = at( o );
	std::stringstream ss;
	switch( vl->type )
	{
		case ASN_BOOLEAN:
			ss << ( *vl->val.integer == 1 ? "true" : "false" );
			break;
		case ASN_INTEGER:
			ss << vl->val.integer;
			break;
		case ASN_OCTET_STR:
			ss << std::string( (const char *)vl->val.string, vl->val_len );
			break;
		case ASN_NULL:
			break;
		case ASN_OBJECT_ID:
			ss << SNMPpp::OID( vl->val.objid, vl->val_len/sizeof(unsigned long) );
			break;
		case ASN_TIMETICKS:
			ss << vl->val.integer << " timeticks (1/100 seconds)";
			break;
		default:
			// if you hit this exception, please go ahead and add whichever case you triggered
			/// @throw std::invalid_argument if the requested OID is not one of the few simple types recognized by SNMPpp.
			throw std::invalid_argument( "OID " + o.str() + " has no string representation." );
	}
	
	return ss.str();
}


std::ostream &operator<<( std::ostream &os, const SNMPpp::Varlist &varlist )
{
	SNMPpp::VecOID v;
	varlist.getOids( v );

	os << "Number of OIDs in the variable list: " << v.size() << std::endl;

	// list all of the OIDs in the varlist
	for ( size_t idx = 0; idx < v.size(); idx ++ )
	{
		os << "\t" << v[idx] << ": ";
		const int type = varlist.asnType( v[idx] );
		os << " asn type=" << type;
		switch ( type )
		{
			case ASN_BOOLEAN:
			case ASN_INTEGER:
			case ASN_OCTET_STR:
			case ASN_NULL:
			case ASN_OBJECT_ID:
			case ASN_TIMETICKS:
				os << ", txt=" << varlist.asString( v[idx] );
				break;
		}
		os << std::endl;
	}

	return os;
}
