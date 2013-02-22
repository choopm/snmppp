# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

PROJECT ( SNMPpp CXX )

# SNMP++ requires net-snmp (e.g., "libsnmp-dev.deb")
FIND_LIBRARY ( NETSNMP_LIBRARY		NAMES netsnmp		)
FIND_LIBRARY ( NETSNMPAGENT_LIBRARY	NAMES netsnmpagent	)
FIND_LIBRARY ( NETSNMPHELPERS_LIBRARY	NAMES netsnmphelpers	)
FIND_LIBRARY ( NETSNMPMIBS_LIBRARY	NAMES netsnmpmibs	)
FIND_LIBRARY ( NETSNMPTRAPD_LIBRARY	NAMES netsnmptrapd	)
FIND_LIBRARY ( SNMP_LIBRARY			NAMES snmp			)

SET ( SNMP_LIBRARIES ${NETSNMP_LIBRARY} ${NETSNMPAGENT_LIBRARY} ${NETSNMPHELPERS_LIBRARY} ${NETSNMPMIBS_LIBRARY} ${NETSNMPTRAPD_LIBRARY} ${SNMP_LIBRARY} )

# DEBUG
ADD_DEFINITIONS( -DDEBUG )
SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_CFLAGS} -O0 -ggdb" )
# NON-DEBUG
#add_definitions( -DNDEBUG )
#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_CFLAGS} -O3" )

INCLUDE_DIRECTORIES ( BEFORE include )
SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option -Wall -Werror -Wshadow -Wformat=2 -Winit-self -Wwrite-strings -fpic" )

SUBDIRS ( src examples include/SNMPpp )
