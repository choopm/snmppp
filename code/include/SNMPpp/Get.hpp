// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/Session.hpp>
#include <SNMPpp/OID.hpp>
#include <SNMPpp/PDU.hpp>


namespace SNMPpp
{
	// Several C++ helpers for some of the common net-snmp "GET" actions.
	// Remember to free the response PDUs by calling SNMPpp::PDU::free() or
	// netsnmp's snmp_pdu_free().

	// The request PDU is automatically freed.  The response PDU needs to be
	// freed by the caller.  This is the method all other "get" call to
	// perform the underlying request->response sync with the SNMP server.
	// This will throw if an unexpected problem occurs.
	SNMPpp::PDU sync( SNMPpp::SessionHandle &session, SNMPpp::PDU &request );

	// net-snmp uses "sync", but also provide "get" to be consistent with all the other ones below
	SNMPpp::PDU get( SNMPpp::SessionHandle &session, SNMPpp::PDU &pdu );
	
	// Get a single OID.  The response PDU needs to be freed by the caller.
	SNMPpp::PDU get( SNMPpp::SessionHandle &session, const SNMPpp::OID &o );

	// Get the next OID.  The response PDU needs to be freed by the caller.
	SNMPpp::PDU getNext( SNMPpp::SessionHandle &session, const SNMPpp::OID &o );
	SNMPpp::PDU getNext( SNMPpp::SessionHandle &session, SNMPpp::PDU &request );

	// Get several specific OIDs at once.  The response PDU needs to be freed by the caller.
	SNMPpp::PDU get( SNMPpp::SessionHandle &session, const SetOID &oids );

	// bulk requests require SNMPv2 or higher
	SNMPpp::PDU getBulk( SNMPpp::SessionHandle &session, const SNMPpp::OID &o	, const int maxRepetitions = 50, const int nonRepeaters = 0 );
	SNMPpp::PDU getBulk( SNMPpp::SessionHandle &session, SNMPpp::PDU &pdu		, const int maxRepetitions = 50, const int nonRepeaters = 0 );
};
