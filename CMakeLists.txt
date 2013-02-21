# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

PROJECT ( SNMPpp CXX )

CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

IF ( NOT CMAKE_BUILD_TYPE )
	SET ( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
ENDIF ()

SET ( SNMPPP_VER_MAJOR 0 )
SET ( SNMPPP_VER_MINOR 0 )
SET ( SNMPPP_VER_PATCH 1 )
SET ( SNMPPP_VERSION "${SNMPPP_VER_MAJOR}.${SNMPPP_VER_MINOR}.${SNMPPP_VER_PATCH}" )

SUBDIRS ( code )

# dependencies were found by running this comand:  objdump --private-headers libsnmppp.so | grep NEEDED
SET ( CPACK_DEBIAN_PACKAGE_DEPENDS		"libstdc++6, libgcc1, libsnmp15, libsnmp-dev" )
SET ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE	"amd64" )
SET ( CPACK_DEBIAN_PACKAGE_SECTION		"libs" )
SET ( CPACK_DEBIAN_PACKAGE_PRIORITY		"optional" )
SET ( CPACK_DEBIAN_PACKAGE_MAINTAINER		"Stephane Charette <stephanecharette@gmail.com>" )
SET ( CPACK_PACKAGE_VENDOR				"Stephane Charette" )
SET ( CPACK_PACKAGE_CONTACT				"stephanecharette@gmail.com" )
SET ( CPACK_PACKAGE_DESCRIPTION_SUMMARY	"SNMP C++ wrapper for net-snmp" )
SET ( CPACK_PACKAGE_DESCRIPTION			"SNMP C++ wrapper for net-snmp" )
SET ( CPACK_PACKAGE_NAME					"snmppp-dev" )
SET ( CPACK_PACKAGE_VERSION				${SNMPPPP_VERSION} )
SET ( CPACK_PACKAGE_VERSION_MAJOR			${SNMPPP_VER_MAJOR} )
SET ( CPACK_PACKAGE_VERSION_MINOR			${SNMPPP_VER_MINOR} )
SET ( CPACK_PACKAGE_VERSION_PATCH			${SNMPPP_VER_PATCH} )
SET ( CPACK_RESOURCE_FILE_LICENSE			${CMAKE_CURRENT_SOURCE_DIR}/LICENSE )
SET ( CPACK_GENERATOR					"TGZ;DEB;RPM" )

INCLUDE( CPack )
