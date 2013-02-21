# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

PROJECT ( SNMPpp CXX )

CONFIGURE_FILE (	Version.hpp.in "${CMAKE_CURRENT_SOURCE_DIR}/Version.hpp" @ONLY )

FILE ( GLOB SNMPPPINCLUDES *.hpp )

INSTALL ( FILES ${SNMPPPINCLUDES} DESTINATION include/SNMPpp/ )
