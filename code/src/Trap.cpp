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
	// TODO:  what does this one return:  netsnmp_get_agent_uptime();
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
	netsnmp_variable_list *vl = SNMPpp::createV2TrapVarlist( o, uptime );
	sendV2Trap( vl );

	return;
}


void SNMPpp::sendV2Trap( netsnmp_variable_list *vl )
{
	if ( vl == NULL )
	{
		throw std::invalid_argument( "Cannot send SNMPv2 trap using a NULL variable list." );
	}

	// at the very least, must have the uptime and trap oid, so size should be >= 2
	if ( count_varbinds( vl ) < 2 )
	{
		snmp_free_varbind( vl );
		throw std::invalid_argument( "Variable list is too small to be a valid SNMPv2 trap." );
	}

	const int rc = netsnmp_send_traps( -1, -1, NULL, 0, vl, NULL, 0 );
	snmp_free_varbind( vl );

	if ( rc )
	{
		std::stringstream ss;
		ss	<< "Failed to send trap (rc=" << rc << ").";
		throw std::runtime_error( ss.str() );
	}
	
	return;
}


void SNMPpp::sendV2Trap( SNMPpp::PDU &pdu )
{
	if ( pdu.empty() )
	{
		pdu.free();
		throw std::invalid_argument( "Cannot send a trap using an empty PDU." );
	}

	// steal the variable list from the PDU, free the gutted PDU, then send
	// out the trap using the variable list that was stolen from the PDU
	netsnmp_pdu *p = pdu;
	netsnmp_variable_list *vl = p->variables;
	p->variables = NULL;
	pdu.free();

	sendV2Trap( vl );

	return;
}


netsnmp_variable_list *SNMPpp::createV2TrapVarlist( const SNMPpp::OID &o, const long uptime )
{
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

	if ( o.empty() )
	{
		throw std::invalid_argument( "Cannot create SNMPv2 trap without an OID." );
	}

	const SNMPpp::OID sysUpTime	( OID::kSysUpTime	); // set this to centiseconds
	const SNMPpp::OID trap		( OID::kTrap			); // set this to the OID you want to send out
	
	netsnmp_variable_list *vl = NULL;
	snmp_varlist_add_variable( &vl, sysUpTime, sysUpTime, ASN_TIMETICKS, (unsigned char*)&uptime, sizeof(uptime)	);
	snmp_varlist_add_variable( &vl, trap, trap, ASN_OBJECT_ID, o, o.size() * sizeof(unsigned long) );

	return vl;
}
