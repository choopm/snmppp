// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <string>
#include <SNMPpp/OID.hpp>
#include <SNMPpp/PDU.hpp>
#include <SNMPpp/net-snmppp.hpp>
#include <sys/types.h>
#include <time.h>


namespace SNMPpp
{
	// The only way to send traps to all configured trap sinks is to first
	// initialize as a net-snmp agent.  If the process you are writing is
	// already an net-snmp agent, then all is fine.  But if the process is
	// NOT already a SNMP agent, then you need to make the following call
	// to initialize a connection to SNMPD prior to sending out a trap.
	//
	// Calling SNMPpp::initializeNetSnmpAgent() will make your application a
	// net-snmp agent by calling init_agent(), init_snmp(), and init_traps().
	//
	// If your application is logging errors of this sort when trying to send
	// a SNMP trap:
	//
	//		Warning: Failed to connect to the agentx master agent ([NIL]):
	//
	// ...then it means there are permission problems.  You either need to run
	// your application as "root", or you need to open up the permissions on
	// AgentX in /etc/snmp/snmpd.conf.  Search for "agentXPerms" for details.
	//
	// If your application is logging errors of this sort when trying to send
	// a SNMP trap:
	//
	//		Cannot rename /var/lib/snmp/snmppp.conf to /var/lib/snmp/snmppp.0.conf
	//		Cannot unlink /var/lib/snmp/snmppp.conf
	//		read_config_store open failure on /var/lib/snmp/snmppp.conf
	//
	// ...then it means you either have to run as "root", or you need to ensure
	// that /var/lib/snmp is writeable by the same user/group as your application.
	//
	// If your application is logging errors of this sort when trying to send
	// a SNMP trap:
	//
	//		truncating integer value > 32 bits
	//		send_trap: no v2 trapOID varbind provided
	//
	// ...then it means net-snmp traps haven't been initialized.  Are you
	// *certain* you've called SNMPpp::initializeNetSnmpAgent()?
	//
	void initializeNetSnmpAgent( const std::string &name = "snmppp" );
	void shutdownNetSnmpAgent(   const std::string &name = "snmppp" );


	// The first variable in SNMPv2 traps is 'sysUpTime', which is defined as:
	//
	//		"the time ... since the network management portion of the system was re-initialized"
	//
	// But in all likelyhood, this value isn't easily available from code
	// which is using SNMPpp.  So several other possibly useful values have
	// been made available for convenience.
	//
	// Return the number of centiseconds (1/100s) since the given time.
	long centisecondsSince( const time_t t );
	//
	// Return the number of centiseconds (1/100s) since the given PID was
	// started.  If PID==0 or the PID uptime is unavailable, then the
	// net-snmp's uptime will be returned instead.
	long centisecondsPidStarted( pid_t pid = 0 );
	//
	// Return the number of centiseconds (1/100s) since the last reboot.
	long centisecondsUptime( void );
	//
	// Return the number of centiseconds (1/100s) given by net-snmp's get_uptime().
	// From what I can tell, this is just a few centiseconds off from the last
	// reboot, and it is unaffected by restarting snmpd.  If you don't know
	// which one of the centiseconds...() call you need to use, this is most
	// likely the safest and best choice since the value comes from net-snmp.
	long centisecondsNetSnmpUptime( void );

	// To send a trap to all configured trapsinks, an application must have
	// made the right calls to be considered a net-snmp agent.  If you want
	// to generate traps from an application which isn't running as an agent,
	// remember to call SNMPpp::initializeNetSnmpAgent() otherwise no trap
	// will be sent.
	void sendV2Trap( const SNMPpp::OID &o, const long uptime = SNMPpp::centisecondsNetSnmpUptime() );

	// If you have a variable list already made up with at least 2 entries
	// describing the uptime and the trap, call this method.  Note you can
	// use this to send as many additional OIDs as you want with the trap.
	// The variable list is freed prior to returning.
	void sendV2Trap( netsnmp_variable_list *vl );

	// Similar to the previous one.  The PDU is freed prior to returning.
	void sendV2Trap( SNMPpp::PDU &pdu );

	// If you'd like to send a trap with multiple variables, you can use this
	// method as a starting point to create a varlist with the first two OIDs
	// filled in correctly.  Once you finish adding variables to the varlist,
	// call sendV2Trap( vl ) to have the trap sent out.
	netsnmp_variable_list *createV2TrapVarlist( const SNMPpp::OID &o, const long uptime = SNMPpp::centisecondsNetSnmpUptime() );
};
