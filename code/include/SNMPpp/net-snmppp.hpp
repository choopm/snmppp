// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

/** @file
 * All net-snmp includes needed by SNMPpp can be easily included by using
 * net-snmppp.hpp.  No other SNMPpp source files should have to include
 * additional net-snmp headers to compile SNMPpp.
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/library/snmpv3.h>
#include <net-snmp/library/snmpusm.h>

#include <net-snmp/agent/agent_trap.h>
#include <net-snmp/agent/snmp_agent.h>
#include <net-snmp/agent/snmp_vars.h>
#include <net-snmp/agent/ds_agent.h>

#include <string>


namespace SNMPpp
{
    /// Disable net-snmp logging.  @see net-snmp/library/snmp_logging.h for several more options.
    void netsnmpDisableLogging( void );

    /// Toggle net-snmp logging to STDERR.
    void netsnmpLogStdErr( const bool enabled = true );

    /// Toggle net-snmp logging to syslog.
    void netsnmpLogSyslog( const bool enabled = true );

    /// Toggle net-snmp logging to an external file.
    void netsnmpLogToFile( const bool enabled = true, const std::string &filename = "/var/log/netsnmp.log" );
};
