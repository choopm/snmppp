# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

PROJECT ( SNMPpp CXX )

SET ( SNMPPP_SOURCE
	Get.cpp
	net-snmppp.cpp
	OID.cpp
	PDU.cpp
	SNMPpp.cpp )

ADD_LIBRARY ( snmppps STATIC ${SNMPPP_SOURCE} )
ADD_LIBRARY ( snmppp  SHARED ${SNMPPP_SOURCE} )

SET_TARGET_PROPERTIES ( snmppps PROPERTIES OUTPUT_NAME "snmppp" )
SET_TARGET_PROPERTIES ( snmppps PROPERTIES PREFIX "lib" )
TARGET_LINK_LIBRARIES ( snmppps ${SNMP_LIBRARIES} )

INSTALL ( TARGETS snmppp snmppps
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib )
