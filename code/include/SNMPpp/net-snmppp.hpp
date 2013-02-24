// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

// All net-snmp includes needed by SNMPpp are included here:
#include <stdlib.h> // needed by net-snmp/utilities.h
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/config_api.h>
#include <net-snmp/output_api.h>
#include <net-snmp/session_api.h>
#include <net-snmp/varbind_api.h>
#include <net-snmp/pdu_api.h>
#include <net-snmp/utilities.h>
#include <net-snmp/agent/agent_trap.h>
#include <net-snmp/agent/snmp_agent.h>
#include <net-snmp/agent/snmp_vars.h>
#include <net-snmp/agent/ds_agent.h>
//#include <net-snmp/library/snmp_api.h>
// That's it.  No other SNMPpp source files should have to include additional net-snmp headers.

#include <string>


namespace SNMPpp
{
	// several simple tweaks for net-snmp logging
	// (more options available in <net-snmp/library/snmp_logging.h)
	void netsnmpDisableLogging( void );
	void netsnmpLogStdErr( const bool enabled = true );
	void netsnmpLogSyslog( const bool enabled = true );
	void netsnmpLogToFile( const bool enabled = true, const std::string &filename = "/var/log/netsnmp.log" );
	// Important note if using LogToFile:
	//		net-snmp may not be running as "root".  Check your directory and
	//		file write permissions if the log file you expect wasn't created.
};
