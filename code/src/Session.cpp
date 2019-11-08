// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <stdlib.h>
#include <sstream>
#include <stdexcept>
#include <SNMPpp/Session.hpp>


void SNMPpp::openSession( SNMPpp::SessionHandle &sessionHandle, const std::string &server, const std::string &community, const int version, const int retryAttempts, const int timeout )
{
    std::lock_guard<std::mutex> lock(mtxOpenSession);
    // make sure you call closeSession() to free up the handle before calling
    // openSession() because we're about to overwrite any previous handles
    sessionHandle = NULL;

    netsnmp_session session = {0};
    snmp_sess_init( &session );

    session.version         = version;
    session.retries         = retryAttempts;
    session.timeout         = timeout;
    session.peername        = (char*)server.c_str();
    session.community       = (unsigned char*)community.c_str();
    session.community_len   = community.size();

    sessionHandle = snmp_sess_open( &session );
    if ( sessionHandle == NULL || snmp_sess_session( sessionHandle ) == NULL )
    {
        // Don't use snmp_sess_error() if the problem is with snmp_sess_open()!
        // Instead, fall back to the traditional snmp_error() and pass in the
        // original netsnmp_session structure.  See the man pages for
        // snmp_sess_error() and snmp_error() for details.
        int error1 = 0;
        int error2 = 0;
        char *msg  = NULL;
        snmp_error( &session, &error1, &error2, &msg );

        std::stringstream ss;
        ss  << "Failed to open SNMP session to \"" << session.peername << "\". ["
            << "e1=" << error1 << ", "
            << "e2=" << error2;
        if ( msg != NULL && msg[0] != '\0' )
        {
            ss << ", " << msg;
        }
        ss << "]";

        free( msg );
        /// @throw std::runtime_error if snmp_sess_open() fails to return a valid new session.
        throw std::runtime_error( ss.str() );
    }

    return;
}

 void SNMPpp::openSessionV3( SessionHandle &sessionHandle, const std::string &server, const std::string &authUser, const std::string &authPassword, const std::string &privPassword, const std::string &secLevel, const std::string &authProtocol, const std::string &privProtocol, const int retryAttempts, const int timeout )
{
    std::lock_guard<std::mutex> lock(mtxOpenSession);
    // make sure you call closeSession() to free up the handle before calling
    // openSession() because we're about to overwrite any previous handles

     // For debugging purposes:
    /* snmp_debug_init();
    debug_register_tokens("all");
    snmp_set_do_debugging(1);
    netsnmp_ds_set_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_DUMP_PACKET, 1); */

    sessionHandle = NULL;

    init_snmp("SNMPpp"); // Needed, otherwise net-snmp internal states/structures regarding SNMPv3 secmods aren't initialized properly (no such security module available). You will also experience various issues with automatic engineID probing (RFC5343). Trying to initialize the security modules and engineID ourself (init_usm(), snmpv3_engineID_probe(..)) will result in a segfault triggered by strchr() after callbacks to internal_register_config_handler(..).

    netsnmp_session session = {0};
    snmp_sess_init( &session );

    session.version = SNMP_VERSION_3;
    session.retries = retryAttempts;
    session.timeout = timeout;
    session.peername = (char *) server.c_str();

//     session.contextName = strdup("");
//     session.contextNameLen = strlen(session.contextName);

    session.engineBoots = 0;
    session.engineTime = 0;

    session.securityName = (char *) authUser.c_str();
    session.securityNameLen = authUser.size();

    session.securityModel = USM_SEC_MODEL_NUMBER; // de-facto standard

    if (secLevel == "authPriv") {
        session.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;
    } else if (secLevel == "authNoPriv") {
        session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;
    } else if (secLevel == "noAuthNoPriv") {
        session.securityLevel = SNMP_SEC_LEVEL_NOAUTH;
    } else {
        throw std::runtime_error("Unsupported secLevel, valid: authPriv, authNoPriv, noAuthNoPriv");
    }

    if (authProtocol == "MD5") {
        session.securityAuthProto = usmHMACMD5AuthProtocol;
        session.securityAuthProtoLen = USM_AUTH_PROTO_MD5_LEN;
    } else if (authProtocol == "SHA1") {
        session.securityAuthProto = usmHMACSHA1AuthProtocol;
        session.securityAuthProtoLen = USM_AUTH_PROTO_SHA_LEN;
    } else {
        throw std::runtime_error("Unsupported authProtocol, valid: MD5, SHA1");
    }
    session.securityAuthKeyLen   = USM_AUTH_KU_LEN;

    if (privProtocol == "DES") {
        session.securityPrivProto = usmDESPrivProtocol;
        session.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
    } else if (privProtocol == "AES") {
        session.securityPrivProto = usmAESPrivProtocol;
        session.securityPrivProtoLen = USM_PRIV_PROTO_AES_LEN;
    } else {
        throw std::runtime_error("Unsupported privProtocol, valid: DES, AES");
    }
    session.securityPrivKeyLen = USM_PRIV_KU_LEN;

    if ( (secLevel == "authPriv" || secLevel == "authNoPriv") &&
        generate_Ku(session.securityAuthProto,
                    session.securityAuthProtoLen,
                    (u_char *) authPassword.c_str(),
                    authPassword.size(),
                    session.securityAuthKey,
                    &session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
        throw std::runtime_error("Error generating Ku from authentication pass phrase");
    }

    if (secLevel == "authPriv" &&
        generate_Ku(session.securityAuthProto,
                    session.securityAuthProtoLen,
                    (u_char *) privPassword.c_str(),
                    privPassword.size(),
                    session.securityPrivKey,
                    &session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
        throw std::runtime_error("Error generating Ku from privacy pass phrase");
    }

//     setup_engineID(NULL, NULL);
//     session.contextEngineID = snmpv3_generate_engineID(&session.contextEngineIDLen);
//     size_t ebuf_len= 32;
//     size_t eout_len= 0;
//     u_char* ebuf= (u_char *) malloc(ebuf_len);
//     char buf[] = "0x8000000001";
//     if (!snmp_hex_to_binary(&ebuf, &ebuf_len, &eout_len, 1, buf))
//         throw std::runtime_error("Bad engineID");
//     if ((eout_len < 5) || (eout_len > 32))
//         throw std::runtime_error("Invalid engineID");
//     session.securityEngineID = ebuf;
//     session.securityEngineIDLen = eout_len;

    sessionHandle = snmp_sess_open( &session );
    if ( sessionHandle == NULL || snmp_sess_session( sessionHandle ) == NULL )
    {
        // Don't use snmp_sess_error() if the problem is with snmp_sess_open()!
        // Instead, fall back to the traditional snmp_error() and pass in the
        // original netsnmp_session structure.  See the man pages for
        // snmp_sess_error() and snmp_error() for details.
        int error1 = 0;
        int error2 = 0;
        char *msg  = NULL;
        snmp_error( &session, &error1, &error2, &msg );

        std::stringstream ss;
        ss  << "Failed to open SNMP session to \"" << session.peername << "\". ["
            << "e1=" << error1 << ", "
            << "e2=" << error2;
        if ( msg != NULL && msg[0] != '\0' )
        {
            ss << ", " << msg;
        }
        ss << "]";

        free( msg );
        /// @throw std::runtime_error if snmp_sess_open() fails to return a valid new session.
        throw std::runtime_error( ss.str() );
    }

    return;
 }

void SNMPpp::closeSession( SNMPpp::SessionHandle &sessionHandle )
{
    if ( sessionHandle )
    {
        snmp_sess_close( sessionHandle );
        sessionHandle = NULL;
    }

    return;
}
