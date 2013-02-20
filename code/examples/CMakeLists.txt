# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

project ( SNMPpp )

file ( GLOB EXAMPLES *.cpp )
foreach ( EXAMPLE_CPP ${EXAMPLES} )

	get_filename_component( EXAMPLE ${EXAMPLE_CPP} NAME_WE )
	add_executable ( ${EXAMPLE} ${EXAMPLE_CPP} )
	target_link_libraries ( ${EXAMPLE} snmppps ${SNMP_LIBRARIES} )

endforeach()

