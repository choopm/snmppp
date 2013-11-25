# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

FILE ( GLOB SNMPPP_SOURCE *.cpp )
LIST ( SORT SNMPPP_SOURCE )

ADD_LIBRARY ( snmppp STATIC ${SNMPPP_SOURCE} )
TARGET_LINK_LIBRARIES ( snmppp ${SNMP_LIBRARIES} )
INSTALL ( TARGETS snmppp ARCHIVE DESTINATION lib LIBRARY DESTINATION lib )

IF ( UNIX )

    ADD_LIBRARY ( snmppps SHARED ${SNMPPP_SOURCE} )
    SET_TARGET_PROPERTIES ( snmppps PROPERTIES OUTPUT_NAME "snmppp" )
    SET_TARGET_PROPERTIES ( snmppps PROPERTIES PREFIX "lib" )
    INSTALL ( TARGETS snmppps ARCHIVE DESTINATION lib LIBRARY DESTINATION lib )

ENDIF ()
