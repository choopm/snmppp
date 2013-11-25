// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <stdexcept>
#include <SNMPpp/net-snmppp.hpp>


void SNMPpp::netsnmpDisableLogging( void )
{
    snmp_disable_log();

    return;
}


void SNMPpp::netsnmpLogStdErr( const bool enabled )
{
    if ( enabled )
    {
        /// @todo Why is init_snmp_logging() an unresolved external in Windows?
#ifndef WIN32
        init_snmp_logging();
#endif
        snmp_enable_stderrlog();
    }
    else
    {
        snmp_disable_stderrlog();
    }

    return;
}


void SNMPpp::netsnmpLogSyslog( const bool enabled )
{
    if ( enabled )
    {
        /// @todo Why is init_snmp_logging() an unresolved external in Windows?
#ifndef WIN32
        init_snmp_logging();
#endif
        snmp_enable_syslog();
    }
    else
    {
        snmp_disable_syslog();
    }

    return;
}


void SNMPpp::netsnmpLogToFile( const bool enabled, const std::string &filename )
{
    /** @note net-snmp may not be running as "root", and may not have access
     * to the file it is being told to use to log.  Check your directory and
     * file write permissions if the log file you expect wasn't created.
     */

    if ( enabled && ! filename.empty() )
    {
        /// @todo Why is init_snmp_logging() an unresolved external in Windows?
#ifndef WIN32
        init_snmp_logging();
#endif
        snmp_enable_filelog( filename.c_str(), 1 ); // 0 overwrites, 1 appends
    }
    else
    {
        /// @todo Why is snmp_disable_filelog() an unresolved external in Windows?
#ifndef WIN32
        snmp_disable_filelog();
#endif
    }

    return;
}
