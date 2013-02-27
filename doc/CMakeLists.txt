# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

PROJECT ( SNMPpp CXX )

FILE ( GLOB DOCS *.txt )
LIST ( REMOVE_ITEM DOCS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt )

INSTALL ( FILES ${DOCS} DESTINATION doc/SNMPpp )

