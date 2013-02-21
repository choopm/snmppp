// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <SNMPpp/SNMPpp.hpp>


std::string SNMPpp::version( void )
{
	return std::string( LIBSNMPPPVER );
}
