# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

FILE ( GLOB EXAMPLES *.cpp )
LIST ( SORT EXAMPLES )

FOREACH ( EXAMPLE_CPP ${EXAMPLES} )

    GET_FILENAME_COMPONENT( EXAMPLE ${EXAMPLE_CPP} NAME_WE )
    ADD_EXECUTABLE ( ${EXAMPLE} ${EXAMPLE_CPP} )
    TARGET_LINK_LIBRARIES ( ${EXAMPLE} snmppps ${SNMP_LIBRARIES} )
    ADD_TEST ( ${EXAMPLE} ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE} )

ENDFOREACH ()
