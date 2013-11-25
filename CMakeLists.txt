# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

PROJECT ( SNMPpp C CXX )

CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

IF ( NOT CMAKE_BUILD_TYPE )
    SET ( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
ENDIF ()

EXECUTE_PROCESS ( COMMAND git describe WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE GITVER OUTPUT_STRIP_TRAILING_WHITESPACE )
MESSAGE ( "git describe: ${GITVER}" )
IF ( NOT ${GITVER} MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+-[0-9]+-g[0-9a-f]+$" )
    MESSAGE ( FATAL_ERROR "Failed to parse git revision number." )
ENDIF ()
STRING ( REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)-g([0-9a-f]+)$" "\\1" SNMPPP_VER_MAJOR  ${GITVER} )
STRING ( REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)-g([0-9a-f]+)$" "\\2" SNMPPP_VER_MINOR  ${GITVER} )
STRING ( REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)-g([0-9a-f]+)$" "\\3" SNMPPP_VER_PATCH  ${GITVER} )
STRING ( REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)-g([0-9a-f]+)$" "\\4" SNMPPP_VER_COMMIT ${GITVER} )
STRING ( REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)-g([0-9a-f]+)$" "\\5" SNMPPP_VER_HASH   ${GITVER} )

SET ( SNMPPP_VERSION "${SNMPPP_VER_MAJOR}.${SNMPPP_VER_MINOR}.${SNMPPP_VER_PATCH}-${SNMPPP_VER_COMMIT}-${SNMPPP_VER_HASH}" )
MESSAGE ( "Building ver: ${SNMPPP_VERSION}" )

ENABLE_TESTING ()

ADD_SUBDIRECTORY ( code )
ADD_SUBDIRECTORY ( doc )

INSTALL ( FILES LICENSE DESTINATION doc/SNMPpp )

SET ( CPACK_PACKAGE_VENDOR              "Stephane Charette" )
SET ( CPACK_PACKAGE_CONTACT             "stephanecharette@gmail.com" )
SET ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "SNMPpp C++ wrapper for net-snmp" )
SET ( CPACK_PACKAGE_DESCRIPTION         "SNMPpp C++ wrapper for net-snmp http://snmppp.sourceforge.net/" )
SET ( CPACK_PACKAGE_NAME                "snmppp-dev" )
SET ( CPACK_PACKAGE_VERSION             ${SNMPPP_VERSION} )
SET ( CPACK_PACKAGE_VERSION_MAJOR       ${SNMPPP_VER_MAJOR} )
SET ( CPACK_PACKAGE_VERSION_MINOR       ${SNMPPP_VER_MINOR} )
SET ( CPACK_PACKAGE_VERSION_PATCH       ${SNMPPP_VER_PATCH} )
#SET ( CPACK_PACKAGE_VERSION_PATCH       "${SNMPPP_VER_PATCH}-${SNMPPP_VER_COMMIT}-${SNMPPP_VER_HASH}" )
SET ( CPACK_RESOURCE_FILE_LICENSE       ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE )

IF ( UNIX )
    # dependencies were found by running this comand:  objdump --private-headers libsnmppp.so | grep NEEDED
    SET ( CPACK_DEBIAN_PACKAGE_DEPENDS      "libstdc++6, libgcc1, libsnmp15, libsnmp-dev" )
    SET ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
    SET ( CPACK_DEBIAN_PACKAGE_SECTION      "libs" )
    SET ( CPACK_DEBIAN_PACKAGE_PRIORITY     "optional" )
    SET ( CPACK_DEBIAN_PACKAGE_MAINTAINER   "Stephane Charette <stephanecharette@gmail.com>" )
    SET ( CPACK_PACKAGE_FILE_NAME           "snmppp-dev-${SNMPPP_VER_MAJOR}.${SNMPPP_VER_MINOR}.${SNMPPP_VER_PATCH}-${SNMPPP_VER_COMMIT}_Linux-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}" )
    SET ( CPACK_GENERATOR                   "DEB;RPM" )
ELSE ()
    SET ( CPACK_PACKAGE_FILE_NAME           "snmppp-dev-${SNMPPP_VER_MAJOR}.${SNMPPP_VER_MINOR}.${SNMPPP_VER_PATCH}-${SNMPPP_VER_COMMIT}_Win32" )
    SET ( CPACK_STRIP_FILES                 "TRUE" )
    SET ( CPACK_PACKAGE_INSTALL_DIRECTORY   "SNMPpp" )
    SET ( CPACK_NSIS_PACKAGE_NAME           "SNMPpp" )
    SET ( CPACK_NSIS_DISPLAY_NAME           "SNMPpp" )
    SET ( CPACK_NSIS_MUI_ICON               "${CMAKE_CURRENT_SOURCE_DIR}\\\\doc\\\\images\\\\snmppp.ico" )
    SET ( CPACK_NSIS_MUI_UNIICON            "${CMAKE_CURRENT_SOURCE_DIR}\\\\doc\\\\images\\\\snmppp.ico" )
    SET ( CPACK_PACKAGE_ICON                "${CMAKE_CURRENT_SOURCE_DIR}\\\\doc\\\\images\\\\snmppp.ico" )
    SET ( CPACK_NSIS_CONTACT                "stephanecharette@gmail.com" )
    SET ( CPACK_NSIS_MODIFY_PATH            "OFF" )
    SET ( CPACK_GENERATOR                   "NSIS" )
ENDIF ()


SET ( CPACK_SOURCE_IGNORE_FILES         ".git" ".kdev4" "SNMPpp.kdev4" "build" "snmppp.xcf" "snmppp.png" )
SET ( CPACK_SOURCE_GENERATOR            "TGZ" )

INCLUDE( CPack )
