# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

project ( SNMPpp )

set ( SNMPPP_SOURCE
	Get.cpp
	net-snmppp.cpp
	OID.cpp
	PDU.cpp
	SNMPpp.cpp )

add_library( snmppps STATIC ${SNMPPP_SOURCE} )
add_library( snmppp  SHARED ${SNMPPP_SOURCE} )

target_link_libraries( snmppps ${SNMP_LIBRARIES} )

install ( TARGETS
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib )
