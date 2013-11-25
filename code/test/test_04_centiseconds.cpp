// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <time.h>
#include <iostream>
#include <assert.h>
#include <SNMPpp/Trap.hpp>
#ifndef WIN32
#include <unistd.h>
#endif

void checkCentiseconds( const time_t t )
{
    std::cout
        << "Uptime:   " << SNMPpp::centisecondsUptime()        << std::endl
        << "Since:    " << SNMPpp::centisecondsSince( t )      << std::endl
        << "Net-snmp: " << SNMPpp::centisecondsNetSnmpUptime() << std::endl
        << "Pid:      " << SNMPpp::centisecondsPidStarted()    << std::endl;

	return;
}


int main( int argc, char *argv[] )
{
	std::cout	<< "Test the centiseconds API." << std::endl;

	const time_t t = time(NULL);
	assert( t > 1234567890 );

	checkCentiseconds( t );

	const long l1 = SNMPpp::centisecondsUptime();
	const long l2 = SNMPpp::centisecondsSince( t );
	const long l3 = SNMPpp::centisecondsNetSnmpUptime();
	const long l4 = SNMPpp::centisecondsPidStarted();

	assert( l1 > 0 );
	assert( l2 < 2 );
	assert( l3 > 0 );
	assert( l4 < 2 );

	std::cout << "...pause 1 second..." << std::endl;
#ifdef WIN32
    Sleep( 1000 ); // milliseconds
#else
    sleep( 1 ); // seconds
#endif

	const long l5 = SNMPpp::centisecondsUptime();
	const long l6 = SNMPpp::centisecondsSince( t );
	const long l7 = SNMPpp::centisecondsNetSnmpUptime();
	const long l8 = SNMPpp::centisecondsPidStarted();

	assert( l5 > l1 );
	assert( l6 > l2 );
	assert( l7 > l3 );
	assert( l8 > l4 );

	checkCentiseconds( t );

	return 0;
}
