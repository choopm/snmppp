// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/OID.hpp>


namespace SNMPpp
{
	// C++ helper for some of the common net-snmp "GET" actions,
	// such as SNMP_MSG_GET, SNMP_MSG_GETNEXT, SNMP_MSG_GETBULK
	void get( snmp_session *session, const OID &oid );
};
