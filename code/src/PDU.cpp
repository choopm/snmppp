// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <SNMPpp/PDU.hpp>
#include <SNMPpp/net-snmppp.hpp>
#include <stdexcept>
#include <sstream>


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
		/// @throw std::runtime_error if snmp_pdu_create() cannot allocate the PDU.
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


void SNMPpp::PDU::clear( void )
{
	type = SNMPpp::PDU::kInvalid;
	pdu  = NULL;

	return;
}


bool SNMPpp::PDU::empty( void ) const
{
	if ( pdu == NULL || pdu->variables == NULL )
	{
		return true;
	}
	
	return false;
}


SNMPpp::PDU SNMPpp::PDU::clone( void ) const
{
	netsnmp_pdu *newpdu = snmp_clone_pdu( pdu );

	return PDU( newpdu );
}


const SNMPpp::Varlist SNMPpp::PDU::varlist( void ) const
{
	if ( pdu == NULL )
	{
		/// @throw std::logic_error if the PDU is NULL.
		throw std::logic_error( "Cannot reference a NULL PDU." );
	}
	if ( pdu->variables == NULL )
	{
		/// @throw std::logic_error if the PDU does not yet have a variable list.
		throw std::logic_error( "The PDU does not contain a variable list." );
	}

	return SNMPpp::Varlist( pdu->variables );
}


SNMPpp::Varlist SNMPpp::PDU::varlist( void )
{
	if ( pdu == NULL )
	{
		/// @throw std::logic_error if the PDU is NULL.
		throw std::logic_error( "Cannot reference a NULL PDU." );
	}
	if ( pdu->variables == NULL )
	{
		/// @throw std::logic_error if the PDU does not yet have a variable list.
		throw std::logic_error( "The PDU does not contain a variable list." );
	}

	return SNMPpp::Varlist( pdu->variables );
}


SNMPpp::PDU::operator netsnmp_variable_list *( void )
{
	if ( pdu == NULL )
	{
		/// @throw std::logic_error if the PDU is NULL.
		throw std::logic_error( "Cannot reference a NULL PDU." );
	}

	return pdu->variables;
}


SNMPpp::PDU &SNMPpp::PDU::setVarlist( Varlist &vl )
{
	netsnmp_variable_list *p = vl;
	return setVarlist( p );
}


SNMPpp::PDU &SNMPpp::PDU::setVarlist( netsnmp_variable_list *vl )
{
	if ( pdu == NULL )
	{
		/// @throw std::logic_error if the PDU is NULL.
		throw std::logic_error( "Cannot set variable list on a NULL PDU." );
	}

	if ( vl != pdu->variables )
	{
		snmp_free_varbind( pdu->variables );
		pdu->variables = vl;
	}

	return *this;
}


size_t SNMPpp::PDU::size( void ) const
{
	if ( pdu == NULL || pdu->variables == NULL )
	{
		return 0;
	}

	return varlist().size();
}


bool SNMPpp::PDU::contains( const SNMPpp::OID &o )	const
{
	bool found = false;
	if ( ! empty() )
	{
		found = varlist().contains( o );
	}

	return found;
}


SNMPpp::PDU &SNMPpp::PDU::addNullVar( const SNMPpp::OID &o )
{
	if ( pdu == NULL )
	{
		/// @throw std::logic_error if the PDU is NULL.
		throw std::logic_error( "Cannot reference a NULL PDU." );
	}

	if ( pdu->variables == NULL )
	{
		// if the PDU doesn't have a varlist, create a new one
		SNMPpp::Varlist vl;
		vl.addNullVar( o );
		setVarlist( vl );
	}
	else
	{
		// ...otherwise, add to the existing varlist
		varlist().addNullVar( o );
	}

	return *this;
}


SNMPpp::PDU &SNMPpp::PDU::addNullVars( const SNMPpp::SetOID &s )
{
	if ( pdu == NULL )
	{
		/// @throw std::logic_error if the PDU is NULL.
		throw std::logic_error( "Cannot reference a NULL PDU." );
	}

	if ( pdu->variables == NULL )
	{
		// if the PDU doesn't have a varlist, create a new one
		SNMPpp::Varlist vl;
		vl.addNullVars( s );
		setVarlist( vl );
	}
	else
	{
		// ...otherwise, add to the existing varlist
		varlist().addNullVars( s );
	}

	return *this;
}


SNMPpp::PDU &SNMPpp::PDU::addNullVars( const SNMPpp::VecOID &v )
{
	if ( pdu == NULL )
	{
		/// @throw std::logic_error if the PDU is NULL.
		throw std::logic_error( "Cannot reference a NULL PDU." );
	}
	
	if ( pdu->variables == NULL )
	{
		// if the PDU doesn't have a varlist, create a new one
		SNMPpp::Varlist vl;
		vl.addNullVars( v );
		setVarlist( vl );
	}
	else
	{
		// ...otherwise, add to the existing varlist
		varlist().addNullVars( v );
	}

	return *this;
}


std::ostream &operator<<( std::ostream &os, const SNMPpp::PDU &pdu )
{
	if ( pdu.empty() )
	{
		os << "PDU is empty." << std::endl;
	}
	else
	{
		os << pdu.varlist();
	}

	return os;
}
