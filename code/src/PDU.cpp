// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <SNMPpp/PDU.hpp>
#include <stdexcept>
#include <sstream>
#include <net-snmp/library/snmp_api.h>


SNMPpp::PDU::~PDU( void )
{
	// remember -- DO NOT delete the pdu here!
	// (see PDU.hpp for details)
	return;
}


SNMPpp::PDU::PDU( const SNMPpp::PDU::EType t ) :
	type( t ),
	pdu( NULL )
{
	pdu = snmp_pdu_create( type );
	if ( pdu == NULL )
	{
		// throwing in a constructor is a bad idea
		throw std::runtime_error( "Failed to create a PDU." );
	}

	return;
}


SNMPpp::PDU::PDU( netsnmp_pdu * p ) :
	type( SNMPpp::PDU::kUnknown ),
	pdu( NULL )
{
	// This constructor can be used if a pdu is obtained from another source
	// such as net-snmp, and we'd like to interact with it as a C++ object.

	if ( p != NULL )
	{
		pdu = p;
		type = static_cast<SNMPpp::PDU::EType>( p->command );
	}

	return;
}


void SNMPpp::PDU::free( void )
{
	if ( pdu != NULL )
	{
		snmp_free_pdu( pdu );
	}
	clear();

	return;
}


SNMPpp::PDU SNMPpp::PDU::clone( void ) const
{
	if ( pdu == NULL )
	{
		throw std::logic_error( "Cannot reference a NULL PDU." );
	}

	netsnmp_pdu *newpdu = snmp_clone_pdu( pdu );

	return PDU( newpdu );
}


size_t SNMPpp::PDU::size( void ) const
{
	size_t s = 0;

	if ( pdu )
	{
		netsnmp_variable_list *vl = NULL;

		for (	vl  = pdu->variables;
				vl != NULL;
				vl  = vl->next_variable )
		{
			s ++;
		}
	}

	return s;
}


bool SNMPpp::PDU::empty( void ) const
{
	if ( pdu == NULL || pdu->variables == NULL )
	{
		return true;
	}

	return false;
}


SNMPpp::PDU &SNMPpp::PDU::addNullVar( const SNMPpp::OID &o )
{
	if ( pdu == NULL )
	{
		throw std::logic_error( "Cannot dereference a NULL PDU." );
	}

	netsnmp_variable_list *vl = snmp_add_null_var( pdu, o, o );
	if ( vl == NULL )
	{
		throw std::runtime_error( "Failed to add " + o.str() + " to the PDU." );
	}

	return *this;
}


SNMPpp::PDU &SNMPpp::PDU::addNullVar( const SNMPpp::SetOID &s )
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


SNMPpp::PDU &SNMPpp::PDU::addNullVar( const SNMPpp::VecOID &v )
{
	for ( size_t idx = 0; idx < v.size(); idx ++ )
	{
		addNullVar( v[idx] );
	}

	return *this;
}


const SNMPpp::PDU &SNMPpp::PDU::getOids( SNMPpp::SetOID &s ) const
{
	s.clear();

	if ( pdu )
	{
		for ( netsnmp_variable_list *vl = pdu->variables; vl != NULL; vl = vl->next_variable )
		{
			s.insert( SNMPpp::OID(vl->name, vl->name_length) );
		}
	}

	return *this;
}


const SNMPpp::PDU &SNMPpp::PDU::getOids( SNMPpp::VecOID &v ) const
{
	v.clear();

	if ( pdu )
	{
		for ( netsnmp_variable_list *vl = pdu->variables; vl != NULL; vl = vl->next_variable )
		{
			v.push_back( SNMPpp::OID(vl->name, vl->name_length) );
		}
	}

	return *this;
}


SNMPpp::MapOidVarList SNMPpp::PDU::getMap( void ) const
{
	MapOidVarList m;

	if ( pdu )
	{
		for ( netsnmp_variable_list *vl = pdu->variables; vl != NULL; vl = vl->next_variable )
		{
			m[ SNMPpp::OID(vl->name, vl->name_length) ] = *vl;
		}
	}

	return m;
}


bool SNMPpp::PDU::contains( const SNMPpp::OID &o ) const
{
	bool result = false;

	if ( pdu )
	{
		 for ( netsnmp_variable_list *vl = pdu->variables; vl != NULL; vl  = vl->next_variable )
		 {
			if ( o == SNMPpp::OID(vl->name, vl->name_length) )
			{
				// found it!
				result = true;
				break;
			}
		 }
	}
	
	return result;
}


const netsnmp_variable_list &SNMPpp::PDU::at( const SNMPpp::OID &o ) const
{
	if ( pdu == NULL )
	{
		throw std::logic_error( "Cannot get the variable list entry for OID " + o.str() + " from a NULL PDU." );
	}

	netsnmp_variable_list *vl = NULL;
	for ( vl = pdu->variables; vl != NULL; vl  = vl->next_variable )
	{
		if ( o == SNMPpp::OID(vl->name, vl->name_length) )
		{
			// found it!
			break;
		}
	}
	
	if ( vl == NULL )
	{
		throw std::invalid_argument( "PDU does not contain OID " + o.str() + "." );
	}

	return *vl;
}


SNMPpp::OID SNMPpp::PDU::firstOID( void ) const
{
	if ( pdu == NULL )
	{
		throw std::logic_error( "Cannot get an OID from a NULL PDU." );
	}
	if ( pdu->variables == NULL )
	{
		throw std::logic_error( "Cannot get an OID from an empty PDU." );
	}

	return SNMPpp::OID( pdu->variables->name, pdu->variables->name_length );
}


const netsnmp_variable_list &SNMPpp::PDU::firstVL( void ) const
{
	if ( pdu == NULL )
	{
		throw std::logic_error( "Cannot get a variable list from a NULL PDU." );
	}
	if ( pdu->variables == NULL )
	{
		throw std::logic_error( "Cannot get a variable list from an empty PDU." );
	}
	
	return *pdu->variables;
}


bool	 SNMPpp::PDU::getBool( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list &vl = at( o );

	if ( vl.type != ASN_BOOLEAN )
	{
		throw std::invalid_argument( "OID " + o.str() + " is not a boolean type." );
	}

	return *vl.val.integer == 1; // rfc1212, true=1, false=2
}


long	 SNMPpp::PDU::getLong( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list &vl = at( o );
	
	if ( vl.type != ASN_INTEGER )
	{
		throw std::invalid_argument( "OID " + o.str() + " is not a numeric type." );
	}
	
	return *vl.val.integer;
}


std::string SNMPpp::PDU::getString( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list &vl = at( o );
	
	if ( vl.type != ASN_OCTET_STR )
	{
		throw std::invalid_argument( "OID " + o.str() + " is not a string." );
	}

	return std::string( (const char *)vl.val.string, vl.val_len );
}


SNMPpp::OID SNMPpp::PDU::getOID( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list &vl = at( o );
	
	if ( vl.type != ASN_OBJECT_ID )
	{
		throw std::invalid_argument( "OID " + o.str() + " is not a OID." );
	}
	
	return SNMPpp::OID( vl.val.objid, vl.val_len/sizeof(unsigned long) );
}


std::string SNMPpp::PDU::asString( const SNMPpp::OID &o ) const
{
	const netsnmp_variable_list &vl = at( o );
	std::stringstream ss;
	switch( vl.type )
	{
		case ASN_BOOLEAN:
			ss << ( *vl.val.integer == 1 ? "true" : "false" );
			break;
		case ASN_INTEGER:
			ss << *vl.val.integer;
			break;
		case ASN_OCTET_STR:
			ss << std::string( (const char *)vl.val.string, vl.val_len );
			break;
		case ASN_NULL:
			break;
		case ASN_OBJECT_ID:
			ss << SNMPpp::OID( vl.val.objid, vl.val_len/sizeof(unsigned long) );
			break;
		case ASN_TIMETICKS:
			ss << *vl.val.integer << " timeticks (1/100 seconds)";
			break;
		default:
			// if you hit this exception, please go ahead and add whichever case you triggered
			throw std::invalid_argument( "OID " + o.str() + " has no string representation." );
	}

	return ss.str();
}


std::ostream &operator<<( std::ostream &os, const SNMPpp::PDU &pdu )
{
	SNMPpp::VecOID v;
	pdu.getOids( v );

	os << "Number of OIDs in PDU: " << v.size() << std::endl;

	// list all of the OIDs in the PDU
	for ( size_t idx = 0; idx < v.size(); idx ++ )
	{
		os << "\t" << v[idx] << ": ";
		const int type = pdu.asnType( v[idx] );
		os << " asn type=" << type;
		switch ( type )
		{
			case ASN_BOOLEAN:
			case ASN_INTEGER:
			case ASN_OCTET_STR:
			case ASN_NULL:
			case ASN_OBJECT_ID:
			case ASN_TIMETICKS:
				os << ", txt=" << pdu.asString( v[idx] );
				break;
		}
		os << std::endl;
	}

	return os;
}
