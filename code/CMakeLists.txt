# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

PROJECT ( SNMPpp CXX )

# SNMP++ requires net-snmp (e.g., "libsnmp-dev.deb")
FIND_LIBRARY ( SNMP_LIBRARIES NAMES netsnmp )

# DEBUG
ADD_DEFINITIONS( -DDEBUG )
SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_CFLAGS} -O0 -ggdb" )
# NON-DEBUG
#add_definitions( -DNDEBUG )
#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_CFLAGS} -O3" )

INCLUDE_DIRECTORIES ( BEFORE include )
SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option -Wall -Werror -Wshadow -Wformat=2 -Winit-self -Wwrite-strings" )

SUBDIRS ( src examples include/SNMPpp )
