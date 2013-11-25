// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <SNMPpp/net-snmppp.hpp>
#include <SNMPpp/Session.hpp>
#include <SNMPpp/OID.hpp>
#include <SNMPpp/PDU.hpp>


namespace SNMPpp
{
    /** @file
     * Several C++ helpers for some of the common net-snmp "GET" actions.
     * Remember to free the response PDUs by calling SNMPpp::PDU::free() or
     * `netsnmp's snmp_pdu_free()`.
     */

    /** Send a PDU using the given SNMPpp::SessionHandle, and wait for a reply.
     * This is the method all other "get" functions call to perform the
     * underlying request->response sync with the SNMP server.
     * @note
     * - The *request* PDU is automatically freed before returning to the
     *   caller, even when an exception is thrown.
     * - The *response* PDU needs to be freed using SNMPpp::PDU::free().
     * - This will throw if an unexpected problem occurs.
     */
    SNMPpp::PDU sync( SNMPpp::SessionHandle &session, SNMPpp::PDU &request );

    /** Alias to SNMPpp::sync() for convenience, and to be consistent with the
     * various other SNMPpp::get...() calls.
     * @see SNMPpp::sync() to see additional exceptions this may throw.
     */
    SNMPpp::PDU get( SNMPpp::SessionHandle &session, SNMPpp::PDU &pdu );

    /** Get a single OID
     * @note
     * - The response PDU needs to be freed using SNMPpp::PDU::free().
     * - This will throw if an unexpected problem occurs.
     * @see SNMPpp::sync() to see additional exceptions this may throw.
     */
    SNMPpp::PDU get( SNMPpp::SessionHandle &session, const SNMPpp::OID &o );

    /** Get the next OID.
     * @note
     * - The response PDU needs to be freed using SNMPpp::PDU::free().
     * - This will throw if an unexpected problem occurs.
     * @see SNMPpp::sync() to see additional exceptions this may throw.
     */
    SNMPpp::PDU getNext( SNMPpp::SessionHandle &session, const SNMPpp::OID &o );

    /** Get the next OID.
     * @note
     * - The *request* PDU is automatically freed before returning to the
     *   caller, even when an exception is thrown.
     * - The *response* PDU needs to be freed using SNMPpp::PDU::free().
     * - This will throw if an unexpected problem occurs.
     * @see SNMPpp::sync() to see additional exceptions this may throw.
     */
    SNMPpp::PDU getNext( SNMPpp::SessionHandle &session, SNMPpp::PDU &request );

    /** Get several specific OIDs at once.
     * @note
     * - The response PDU needs to be freed using SNMPpp::PDU::free().
     * - This will throw if an unexpected problem occurs.
     * @see SNMPpp::sync() to see additional exceptions this may throw.
     */
    SNMPpp::PDU get( SNMPpp::SessionHandle &session, const SetOID &oids );

    /** Getbulk request, starting with the given OID.
     * @note
     * - Getbulk requests require SNMPv2 or higher on the server.
     * - The response PDU needs to be freed using SNMPpp::PDU::free().
     * - This will throw if an unexpected problem occurs.
     * @see SNMPpp::sync() to see additional exceptions this may throw.
     */
    SNMPpp::PDU getBulk( SNMPpp::SessionHandle &session, const SNMPpp::OID &o, const int maxRepetitions = 50, const int nonRepeaters = 0 );

    /** Getbulk request, possibly with multiple starting points.
     * @note
     * - Getbulk requests require SNMPv2 or higher on the server.
     * - The *request* PDU is automatically freed before returning to the
     *   caller, even when an exception is thrown.
     * - The *response* PDU needs to be freed using SNMPpp::PDU::free().
     * - This will throw if an unexpected problem occurs.
     * @see SNMPpp::sync() to see additional exceptions this may throw.
     */
    SNMPpp::PDU getBulk( SNMPpp::SessionHandle &session, SNMPpp::PDU &pdu, const int maxRepetitions = 50, const int nonRepeaters = 0 );
};
