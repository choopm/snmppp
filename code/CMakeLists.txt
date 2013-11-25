# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

# SNMPpp requires net-snmp (e.g., "libsnmp-dev.deb")
FIND_LIBRARY ( NETSNMP_LIBRARY          NAMES netsnmp           )
FIND_LIBRARY ( NETSNMPAGENT_LIBRARY     NAMES netsnmpagent      )
#FIND_LIBRARY ( NETSNMPHELPERS_LIBRARY   NAMES netsnmphelpers    )
FIND_LIBRARY ( NETSNMPMIBS_LIBRARY      NAMES netsnmpmibs       )
FIND_LIBRARY ( NETSNMPTRAPD_LIBRARY     NAMES netsnmptrapd      )
FIND_LIBRARY ( SNMP_LIBRARY             NAMES snmp              )

SET ( SNMP_LIBRARIES ${NETSNMP_LIBRARY} ${NETSNMPAGENT_LIBRARY} ${NETSNMPHELPERS_LIBRARY} ${NETSNMPMIBS_LIBRARY} ${NETSNMPTRAPD_LIBRARY} ${SNMP_LIBRARY} )

IF ( WIN32 )
    SET ( SNMP_LIBRARIES ${SNMP_LIBRARIES} ws2_32.lib )
ENDIF ()

# DEBUG
#ADD_DEFINITIONS( -DDEBUG )
# END DEBUG

# NON-DEBUG
ADD_DEFINITIONS( -DNDEBUG )
# END NON-DEBUG

IF ( UNIX )
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3"                              ) # optimization level
#   SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb"                        ) # disable optimization, enable debugging
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option"        ) # show command-line option to control errors
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra"                    ) # enable many warnings that gcc can auto-detect
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror"                          ) # all warnings will be considered as errors
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow"                         ) # warn when a local variable hides another at a higher scope
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2"                       ) # check calls such as printf and scanf for parm format
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers"  ) # allow the perfectly valid foo = {0} initializers
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable"             ) # some asserts aren't used in non-debug mode, so don't complain about unused vars
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter"            ) # especially in C++, sometimes parms aren't used in derived classes
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic"                            ) # position-independent code is necessary for a library
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11"                     ) # C++11 so far is just used in a few std::to_string() which should be easy to remove if necessary
ELSE ()
    INCLUDE_DIRECTORIES ( AFTER /usr/include )
    ADD_DEFINITIONS ( -D_CRT_SECURE_NO_WARNINGS )
    SET ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT" )
ENDIF ()

INCLUDE_DIRECTORIES ( BEFORE include )
ADD_SUBDIRECTORY ( include/SNMPpp )
ADD_SUBDIRECTORY ( src )
ADD_SUBDIRECTORY ( examples )
ADD_SUBDIRECTORY ( test )
