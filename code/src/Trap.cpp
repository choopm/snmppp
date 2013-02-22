// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <SNMPpp/Trap.hpp>
#include <SNMPpp/PDU.hpp>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdexcept>
#include <sstream>
#include <iostream>


void SNMPpp::initializeNetSnmpAgent( const std::string &name )
{
	// the order in which things are called is very important.  For example,
	// init_agent() must be called before init_snmp().  For more examples,
	// see this:  http://www.net-snmp.org/docs/man/snmp_agent_api.html

	netsnmp_ds_set_boolean(
		NETSNMP_DS_APPLICATION_ID		,
		NETSNMP_DS_AGENT_ROLE,	1	); // 0 if master, 1 if client

	// the "name" determines which snmp .conf file will be parsed
	const int rc = init_agent( name.c_str() );
	if ( rc )
	{
		int error1 = 0;
		int error2 = 0;
		char *msg  = NULL;
		snmp_error( NULL, &error1, &error2, &msg );

		std::stringstream ss;
		ss	<< "Failed to initialize the SNMP agent for \"" << name << "\". ["
			<< "rc=" << rc		<< ", "
			<< "e1=" << error1	<< ", "
			<< "e2=" << error2;
		if ( msg != NULL && msg[0] != '\0' )
		{
			ss << ", " << msg;
		}
		ss << "]";

		free( msg );
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

	return 100L * ( now - t );
}


long SNMPpp::centisecondsPidStarted( pid_t pid )
{
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
		// something went wrong -- we have no idea what time to use
		return centisecondsNetSnmpUptime();
	}

	return centisecondsSince( buf.st_ctime );
}


long SNMPpp::centisecondsNetSnmpUptime( void )
{
	// return the number of centiseconds (1/100s) given by net-snmp's get_uptime()
	return get_uptime();

	// TODO:  also look into 	netsnmp_get_agent_uptime()
}


long SNMPpp::centisecondsUptime( void )
{
	// Return the number of centiseconds (1/100s) since the last reboot.

	struct sysinfo s = {0};
	sysinfo( &s );

	return 100L * s.uptime;
}


void SNMPpp::sendV2Trap( const SNMPpp::OID &o, const long uptime )
{
	if ( o.empty() )
	{
		throw std::invalid_argument( "Cannot send a trap without an OID." );
	}

	// RFC 2576 states:
	//
	//		SNMPv2 notification parameters consist of:
	//		
	//		-  A sysUpTime parameter (TimeTicks).  This appears in the first
	//		   variable-binding in an SNMPv2-Trap-PDU or InformRequest-PDU.
	//
	//		-  An snmpTrapOID parameter (OBJECT IDENTIFIER).  This appears in
	//		   the second variable-binding in an SNMPv2-Trap-PDU or
	//		   InformRequest-PDU.
	//		
	//		-  A list of variable-bindings (VarBindList).  This refers to all
	//		   but the first two variable-bindings in an SNMPv2-Trap-PDU or
	//		   InformRequest-PDU.

	const SNMPpp::OID sysUpTime	( OID::kSysUpTime	); // set this to centiseconds
	const SNMPpp::OID trap		( OID::kTrap			); // set this to the OID you want to send out

	netsnmp_variable_list *vl = NULL;
	snmp_varlist_add_variable( &vl, sysUpTime, sysUpTime, ASN_TIMETICKS, (unsigned char*)&uptime, sizeof(uptime)	);
	snmp_varlist_add_variable( &vl, trap, trap, ASN_OBJECT_ID, o, o.size() * sizeof(unsigned long) );

	const int rc = netsnmp_send_traps( -1, -1, NULL, 0, vl, NULL, 0 );
	if ( rc )
	{
		std::stringstream ss;
		ss	<< "Failed to send trap " << o << " (rc=" << rc << ").";
		throw std::runtime_error( ss.str() );
	}

	return;
}
