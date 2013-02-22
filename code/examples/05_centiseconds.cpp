// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <unistd.h>
#include <iostream>
#include <SNMPpp/Trap.hpp>


void checkCentiseconds( const time_t t )
{
	std::cout	<< "Uptime:   "					<< SNMPpp::centisecondsUptime()			<< std::endl
				<< "Since:    "					<< SNMPpp::centisecondsSince( t )			<< std::endl
				<< "Net-snmp: "					<< SNMPpp::centisecondsNetSnmpUptime()	<< std::endl
				<< "Pid " << getpid() << ": "		<< SNMPpp::centisecondsPidStarted()		<< std::endl;

	return;
}


int main( int argc, char *argv[] )
{
	std::cout	<< "Test the centiseconds API." << std::endl;

	const time_t t = time(NULL);

	checkCentiseconds( t );
	std::cout << "...pause 1 second..." << std::endl;
	sleep( 1 );
	checkCentiseconds( t );

	return 0;
}
