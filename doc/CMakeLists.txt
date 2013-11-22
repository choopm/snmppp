# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

FIND_PACKAGE ( Doxygen )
CONFIGURE_FILE ( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )
ADD_CUSTOM_TARGET (
    doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )

FILE ( GLOB DOCS *.txt )
LIST ( REMOVE_ITEM DOCS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt )

FILE ( GLOB_RECURSE DOXY ${CMAKE_CURRENT_BINARY_DIR}/doxygen/html/* )

INSTALL ( FILES ${DOCS} DESTINATION doc/SNMPpp )
INSTALL ( FILES ${DOXY} DESTINATION doc/SNMPpp/doxygen )
