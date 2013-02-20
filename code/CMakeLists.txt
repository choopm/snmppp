# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

project ( SNMPpp CXX )

cmake_minimum_required ( VERSION 2.8 )

# SNMP++ requires net-snmp (e.g., "libsnmp-dev.deb")
find_library ( SNMP_LIBRARIES NAMES netsnmp )

# DEBUG
add_definitions( -DDEBUG )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_CFLAGS} -O0 -ggdb" )

# NON-DEBUG
#add_definitions( -DNDEBUG )
#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_CFLAGS} -O3" )

include_directories ( BEFORE include )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option -Wall -Werror -Wshadow -Wformat=2 -Winit-self -Wwrite-strings" )
set ( SNMPP_VERSION_NUMBER "0.0.1" )
configure_file (	"${CMAKE_HOME_DIRECTORY}/include/SNMPpp/Version.hpp.in" "${CMAKE_HOME_DIRECTORY}/include/SNMPpp/Version.hpp" @ONLY )

subdirs ( src examples )
