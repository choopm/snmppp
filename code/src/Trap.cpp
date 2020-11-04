// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <SNMPpp/Trap.hpp>
#include <sys/stat.h>
#include <stdexcept>
#include <sstream>
#include <iostream>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <sys/sysinfo.h>
#include <unistd.h>
#endif


void SNMPpp::initializeNetSnmpAgent( const std::string &name )
{
    // the order in which things are called is very important.  For example,
    // init_agent() must be called before init_snmp().  For more examples,
    // see this:  http://www.net-snmp.org/docs/man/snmp_agent_api.html

    netsnmp_ds_set_boolean(
        NETSNMP_DS_APPLICATION_ID   ,
        NETSNMP_DS_AGENT_ROLE       , 1 ); // 0 if master, 1 if client

    // the "name" determines which snmp .conf file will be parsed
    const int rc = init_agent( name.c_str() );
    if ( rc )
    {
        int error1 = 0;
        int error2 = 0;
        char *msg  = NULL;
        snmp_error( NULL, &error1, &error2, &msg );

        std::stringstream ss;
        ss  << "Failed to initialize the SNMP agent for \"" << name << "\". ["
            << "rc=" << rc      << ", "
            << "cliberrno=" << error1  << ", "
            << "snmperrno=" << error2;
        if ( msg != NULL && msg[0] != '\0' )
        {
            ss << ", " << msg;
        }
        ss << "]";

        free( msg );
        /// @throw std::runtime_error if net-snmp init_agent(name) returned an error.
        throw std::runtime_error( ss.str() );
    }

    init_snmp ( name.c_str() );
    init_traps();

    return;
}


void SNMPpp::shutdownNetSnmpAgent( const std::string &name )
{
    snmp_shutdown( name.c_str() );
    shutdown_agent();

    return;
}


long SNMPpp::centisecondsSince( const time_t t )
{
    // return the number of centiseconds (1/100s) since the given time

    const time_t now = time( NULL );
    if ( t > now )
    {
        // how were we given a time from the future?
        // ignore the request and return zero
        return 0L;
    }

    /** @return the number of centiseconds (1/100s) that have elapsed since the
     * given time.  If the specified time is in the future, then `zero` is
     * returned.
     */
    return long( 100L * ( now - t ) );
}


long SNMPpp::centisecondsPidStarted( pid_t pid )
{
#ifdef WIN32
    /// @todo This is beyond my limited Windows skills, so return something simpler.
    return centisecondsUptime();
#else
    // Return the number of centiseconds (1/100s) since the given PID was
    // started.  If PID==0 or the PID uptime is unavailable, then net-snmp's
    // uptime will be returned instead.

    if ( pid == 0 )
    {
        pid = getpid();
    }

    char path[20];
    sprintf( path, "/proc/%d", pid );

    struct stat buf = {0};
    if ( stat( path, &buf ) != 0 )
    {
        /** @return If the pid uptime is unavailable or cannot be found, then
         * SNMPpp::centisecondsNetSnmpUptime() is returned instead.
         */
        return centisecondsNetSnmpUptime();
    }

    /** @return If the pid uptime was determined, return the number of
     * centiseconds (1/100s) since the given PID was started.
     */
    return centisecondsSince( buf.st_ctime );
#endif
}


long SNMPpp::centisecondsNetSnmpUptime( void )
{
    /** @details
     * The first variable in SNMPv2 traps is 'sysUpTime', which is defined as:
     *
     *      "the time ... since the network management portion of the system was re-initialized"
     *
     * But in all likelihood, this value isn't easily available from code which
     * is using SNMPpp.  So several other possibly useful values have been made
     * available for convenience.  Of those, centisecondsNetSnmpUptime() is
     * most likely the value you need to use.
     */

    /** @todo What does netsnmp_get_agent_uptime(); return?  I tried both
     * get_uptime() and netsnmp_get_agent_uptime() but as far as I can tell the
     * one called netsnmp_get_agent_uptime() returns garbage.
     */

    /** @return net-snmp's get_uptime(), which on linux seems to be the number
     * of centiseconds since the system was rebooted.
     */
    return get_uptime();
}


long SNMPpp::centisecondsUptime( void )
{
    // Return the number of centiseconds (1/100s) since the last reboot.

    /// @todo In Windows, GetTickCount() wraps after 7 weeks.  Look into using something else, such as GetTickCount64().
#ifdef WIN32
    DWORD uptime = GetTickCount();
    // ULONGLONG uptime = GetTickCount64();

    // remember to convert the time from milliseconds to centiseconds
    return uptime/10;
#else
    struct sysinfo s = {0};
    sysinfo( &s ); // uptime is in seconds, so we need to multiply by 100 to get centiseconds
    return 100L * s.uptime;
#endif
}


void SNMPpp::sendV2Trap( const SNMPpp::OID &o, const long uptime )
{
    SNMPpp::Varlist varlist = SNMPpp::createV2TrapVarlist( o, uptime );
    sendV2Trap( varlist );

    return;
}


void SNMPpp::sendV2Trap( SNMPpp::Varlist &varlist )
{
    /// @see SNMPpp::createV2TrapVarlist();

    if ( varlist.empty() )
    {
        /// @throw std::invalid_argument if varlist is empty.
        throw std::invalid_argument( "Cannot send SNMPv2 trap using a NULL variable list." );
    }

    // at the very least, must have the uptime and trap oid, so size should be >= 2
    if ( varlist.size() < 2 )
    {
        varlist.free();
        /// @throw std::invalid_argument if the varlist contains less than 2 OIDs.
        throw std::invalid_argument( "Variable list is too small to be a valid SNMPv2 trap." );
    }

    const int rc = netsnmp_send_traps( -1, -1, NULL, 0, varlist, NULL, 0 );
    varlist.free();

    if ( rc )
    {
        std::stringstream ss;
        ss << "Failed to send trap (rc=" << rc << ").";
        /// @throw std::runtime_error if netsnmp_send_traps(...) returned an error.
        throw std::runtime_error( ss.str() );
    }

    return;
}


void SNMPpp::sendV2Trap( SNMPpp::PDU &pdu )
{
    if ( pdu.empty() )
    {
        pdu.free();
        /// @throw std::invalid_argument if the PDU is empty.
        throw std::invalid_argument( "Cannot send a trap using an empty PDU." );
    }

    // steal the variable list from the PDU, free the gutted PDU, then send
    // out the trap using the variable list that was stolen from the PDU
    netsnmp_pdu *p = pdu;
    SNMPpp::Varlist varlist( p->variables );
    p->variables = NULL;
    pdu.free();

    sendV2Trap( varlist );

    return;
}


SNMPpp::Varlist SNMPpp::createV2TrapVarlist( const SNMPpp::OID &o, const long uptime )
{
    // RFC 2576 states:
    //
    //      SNMPv2 notification parameters consist of:
    //
    //      -  A sysUpTime parameter (TimeTicks).  This appears in the first
    //         variable-binding in an SNMPv2-Trap-PDU or InformRequest-PDU.
    //
    //      -  An snmpTrapOID parameter (OBJECT IDENTIFIER).  This appears in
    //         the second variable-binding in an SNMPv2-Trap-PDU or
    //         InformRequest-PDU.
    //
    //      -  A list of variable-bindings (VarBindList).  This refers to all
    //         but the first two variable-bindings in an SNMPv2-Trap-PDU or
    //         InformRequest-PDU.

    if ( o.empty() )
    {
        /// @throw std::invalid_argument if the OID is empty.
        throw std::invalid_argument( "Cannot create SNMPv2 trap without an OID." );
    }

    const SNMPpp::OID sysUpTime ( OID::kSysUpTime   ); // set this to centiseconds
    const SNMPpp::OID trap      ( OID::kTrap        ); // set this to the OID you want to send out

    SNMPpp::Varlist varlist;
    snmp_varlist_add_variable( varlist, sysUpTime, sysUpTime, ASN_TIMETICKS, (unsigned char*)&uptime, sizeof(uptime) );
    snmp_varlist_add_variable( varlist, trap, trap, ASN_OBJECT_ID, o, o.size() * sizeof(unsigned long) );

    return varlist;
}
