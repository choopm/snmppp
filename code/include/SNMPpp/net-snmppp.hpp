// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

// All net-snmp includes needed by SNMP++ are included here:
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/config_api.h>
#include <net-snmp/pdu_api.h>
// That's it.  No other SNMP++ source files should have to include any net-snmp headers.

#include <string>


namespace SNMPpp
{
	// net-snmp uses a void * for the "Single API" opaque session pointer, but
	// I prefer to have an actual named type to make it more obvious when
	// passing the session pointer by reference in C++.
	typedef void * SessionHandle; // for example, see the return value of snmp_sess_open()

	// Open a net-snmp session and return a session handle.  The session handle
	// will be needed for other net-snmp calls.  Any previous value in "handle"
	// will be blindly overwritten, so don't re-use handles unless you've
	// remembered to call either snmp_sess_close() or SNMPpp::closeSession().
	//
	// IMPORTANT:
	//		- Handles must be freed when no longer needed.
	//		- Handles are not the same as struct snmp_session!
	//		- This function will throw if a connection cannot be established.
	void openSession( SessionHandle &handle, const std::string &server = "udp:127.0.0.1:161", const std::string &community = "public", const int version = SNMP_VERSION_2c, const int retryAttempts = 3 );

	// Sessions must be closed when no longer needed.  This can be done by
	// calling snmp_sess_close() directly, or SNMPpp::closeSession().
	void closeSession( SessionHandle &handle );
};
