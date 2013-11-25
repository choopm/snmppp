// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

/** @file
 * Including SNMPpp.hpp will then include all of the other SNMPpp files in
 * the correct order so cyclic dependencies are avoided.  If you want to keep
 * things simple, you can include just this one header file, and you'll have
 * access to everything in SNMPpp.
 */
#include <SNMPpp/Version.hpp>
#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/Session.hpp>
#include <SNMPpp/OID.hpp>
#include <SNMPpp/Varlist.hpp>
#include <SNMPpp/PDU.hpp>
#include <SNMPpp/Get.hpp>
#include <SNMPpp/Trap.hpp>


namespace SNMPpp
{
    /// Return the SNMPpp version number.
    std::string version( void );
};
