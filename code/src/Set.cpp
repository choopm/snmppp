// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>
// Copyright (C) 2020 Christoph Hoopmann <choopm@0pointer.org>

#include <stdexcept>
#include <sstream>
#include <stdlib.h>
#include <SNMPpp/Set.hpp>

SNMPpp::PDU SNMPpp::set( SNMPpp::SessionHandle &session, SNMPpp::PDU &pdu )
{
    return sync( session, pdu );
}

SNMPpp::PDU SNMPpp::setBooleanVar( SNMPpp::SessionHandle &session, const SNMPpp::OID &o, bool v )
{
    if ( o.empty() )
    {
        /// @throw std::invalid_argument if the OID is empty.
        throw std::invalid_argument( "OID cannot be empty." );
    }

    SNMPpp::PDU request( SNMPpp::PDU::kSet );
    request.addBooleanVar( o, v );

    return sync( session, request );
}

SNMPpp::PDU SNMPpp::setIntegerVar( SNMPpp::SessionHandle &session, const SNMPpp::OID &o, int v )
{
    if ( o.empty() )
    {
        /// @throw std::invalid_argument if the OID is empty.
        throw std::invalid_argument( "OID cannot be empty." );
    }

    SNMPpp::PDU request( SNMPpp::PDU::kSet );
    request.addIntegerVar( o, v );

    return sync( session, request );
}

SNMPpp::PDU SNMPpp::setInteger64Var( SNMPpp::SessionHandle &session, const SNMPpp::OID &o, long v )
{
    if ( o.empty() )
    {
        /// @throw std::invalid_argument if the OID is empty.
        throw std::invalid_argument( "OID cannot be empty." );
    }

    SNMPpp::PDU request( SNMPpp::PDU::kSet );
    request.addInteger64Var( o, v );

    return sync( session, request );
}

SNMPpp::PDU SNMPpp::setGaugeVar( SNMPpp::SessionHandle &session, const SNMPpp::OID &o, unsigned int v )
{
    if ( o.empty() )
    {
        /// @throw std::invalid_argument if the OID is empty.
        throw std::invalid_argument( "OID cannot be empty." );
    }

    SNMPpp::PDU request( SNMPpp::PDU::kSet );
    request.addGaugeVar( o, v );

    return sync( session, request );
}

SNMPpp::PDU SNMPpp::setOctetStringVar( SNMPpp::SessionHandle &session, const SNMPpp::OID &o, unsigned char * v, long unsigned int size )
{
    if ( o.empty() )
    {
        /// @throw std::invalid_argument if the OID is empty.
        throw std::invalid_argument( "OID cannot be empty." );
    }

    SNMPpp::PDU request( SNMPpp::PDU::kSet );
    request.addOctetStringVar( o, v, size );

    return sync( session, request );
}
