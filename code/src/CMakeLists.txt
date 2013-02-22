# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

PROJECT ( SNMPpp CXX )

FILE ( GLOB SNMPPP_SOURCE *.cpp )

ADD_LIBRARY ( snmppps STATIC ${SNMPPP_SOURCE} )
ADD_LIBRARY ( snmppp  SHARED ${SNMPPP_SOURCE} )

SET_TARGET_PROPERTIES ( snmppps PROPERTIES OUTPUT_NAME "snmppp" )
SET_TARGET_PROPERTIES ( snmppps PROPERTIES PREFIX "lib" )
TARGET_LINK_LIBRARIES ( snmppps ${SNMP_LIBRARIES} )

INSTALL ( TARGETS snmppp  LIBRARY DESTINATION lib )
INSTALL ( TARGETS snmppps ARCHIVE DESTINATION lib )
