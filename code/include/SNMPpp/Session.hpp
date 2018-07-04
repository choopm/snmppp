// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#pragma once

#include <SNMPpp/net-snmppp.hpp>
#include <mutex>
#include <string>


namespace SNMPpp
{
    /** net-snmp uses a void * for the "Single API" opaque session pointer, but
     * SNMPpp prefers to have an actual named type to make it more obvious when
     * passing the session pointer by reference in C++.
     * For example, see the return value of `snmp_sess_open();`.
     * @see SNMPpp::openSession();
     * @see SNMPpp::closeSession();
     */
    typedef void * SessionHandle;

    // net-snmp itself is thread-safe as long as all initializations and config parsing is synchronized (mainly init_* and snmp_*open calls).
    // One shall use the "Single API" (*_sess_* variants) when using net-snmp in a threaded environment, this is default in SNMPpp.
    // As a result of this we use a std::lock_guard to synchronize calls to openSession/openSessionV3 to protect net-snmp from corruption
    static std::mutex mtxOpenSession;

    /** Open a net-snmp session and return a session handle.  The session
     * handle will be needed for all other net-snmp calls.  Any previous value
     * in "handle" will be blindly overwritten, so don't re-use handles unless
     * you've remembered to call either `snmp_sess_close()` or
     * SNMPpp::closeSession().
     * @note
     * - Handles must be freed when no longer needed.
     * - Handles are not the same as `struct snmp_session`!
     * - This will throw if something goes wrong when setting up the session.
     */
    void openSession( SessionHandle &sessionHandle, const std::string &server = "udp:127.0.0.1:161", const std::string &community = "public", const int version = SNMP_VERSION_2c, const int retryAttempts = 3 );
    void openSessionV3( SessionHandle &sessionHandle, const std::string &server = "udp:127.0.0.1:161", const std::string &authUser = "guest", const std::string &authPassword = "", const std::string &privPassword = "", const std::string &secLevel = "authPriv", const std::string &authProtocol = "SHA1", const std::string &privProtocol = "AES", const int retryAttempts = 3 );

    /** Sessions must be closed when no longer needed.  This can be done by
     * calling `snmp_sess_close()` directly, or SNMPpp::closeSession().
     */
    void closeSession( SessionHandle &sessionHandle );
};
