// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <SNMPpp/Get.hpp>
#include <stdexcept>
#include <sstream>
#include <stdlib.h>


SNMPpp::PDU SNMPpp::sync( SNMPpp::SessionHandle &session, SNMPpp::PDU &request )
{
	netsnmp_pdu *pdu = request;
	if ( pdu == NULL )
	{
		throw std::invalid_argument( "Request PDU must not be NULL." );
	}
	if ( session == NULL )
	{
		request.free();
		throw std::invalid_argument( "Session handle must not be NULL." );
	}

	// send out this request and wait until we have a reply
	netsnmp_pdu *response = NULL;
	const int status = snmp_sess_synch_response( session, pdu, &response );

	// the request PDU has been freed by net-snmp and no longer exists
	request.clear();

	if (		response				== NULL				||
			response->errstat	!= SNMP_ERR_NOERROR	||
			status				!= STAT_SUCCESS		)
	{
		int error1 = 0;
		int error2 = 0;
		char *msg  = NULL;
		snmp_sess_error( session, &error1, &error2, &msg );
		std::stringstream ss;
		ss	<< "Failed to get. ["
		<< "e1=" << error1 << ", "
		<< "e2=" << error2;
		if ( msg != NULL && msg[0] != '\0' )
		{
			ss << ", " << msg;
		}
		ss << "]";

		free( msg );
		snmp_free_pdu( response );
		throw std::runtime_error( ss.str() );
	}

	return SNMPpp::PDU( response );
}


SNMPpp::PDU SNMPpp::get( SNMPpp::SessionHandle &session, SNMPpp::PDU &pdu )
{
	return sync( session, pdu );
}


SNMPpp::PDU SNMPpp::get( SNMPpp::SessionHandle &session, const SNMPpp::OID &o )
{
	if ( o.empty() )
	{
		throw std::invalid_argument( "OID cannot be empty." );
	}

	SNMPpp::PDU request( SNMPpp::PDU::kGet );
	request.addNullVar( o );

	return sync( session, request );
}

SNMPpp::PDU SNMPpp::getNext( SNMPpp::SessionHandle &session, const SNMPpp::OID &o )
{
	if ( o.empty() )
	{
		throw std::invalid_argument( "OID cannot be empty." );
	}
	
	SNMPpp::PDU request( SNMPpp::PDU::kGetNext );
	request.addNullVar( o );

	return sync( session, request );
}


SNMPpp::PDU SNMPpp::getNext( SNMPpp::SessionHandle &session, SNMPpp::PDU &request )
{
	netsnmp_pdu *pdu = request;
	if ( pdu == NULL )
	{
		throw std::invalid_argument( "Request PDU must not be NULL." );
	}

	if ( request.getType() == SNMPpp::PDU::kGetNext )
	{
		// the PDU is already GETNEXT
		return sync( session, request );
	}

	// otherwise take the first OID in the PDU (which could be a response PDU) and create a new GETNEXT

	SNMPpp::OID o = request.firstOID();
	request.free();
	return getNext( session, o );
}


SNMPpp::PDU SNMPpp::get( SNMPpp::SessionHandle &session, const SetOID &oids )
{
	if ( oids.empty() )
	{
		throw std::invalid_argument( "Cannot get empty set of OIDs." );
	}

	SNMPpp::PDU pdu( SNMPpp::PDU::kGet );
	SetOID::const_iterator iter;
	for (	iter  = oids.begin();
			iter != oids.end();
			iter ++ )
	{
		const SNMPpp::OID &o = *iter;
		pdu.addNullVar( o );
	}

	return sync( session, pdu );
}


SNMPpp::PDU SNMPpp::getBulk( SNMPpp::SessionHandle &session, const SNMPpp::OID &o, const int maxRepetitions, const int nonRepeaters )
{
	SNMPpp::PDU pdu( SNMPpp::PDU::kGetBulk );
	pdu.addNullVar( o );

	return getBulk( session, pdu, maxRepetitions, nonRepeaters );
}


SNMPpp::PDU SNMPpp::getBulk( SNMPpp::SessionHandle &session, SNMPpp::PDU &pdu, const int maxRepetitions, const int nonRepeaters )
{
	netsnmp_pdu *p = pdu;
	if ( p == NULL )
	{
		throw std::invalid_argument( "Request PDU must not be NULL." );
	}

	if ( pdu.getType() != SNMPpp::PDU::kGetBulk )
	{
		// copy the OIDs and build a new PDU to use for getbulk
		SNMPpp::SetOID s;
		pdu.getOids( s );
		pdu.free();

		pdu = SNMPpp::PDU( SNMPpp::PDU::kGetBulk );
		pdu.addNullVar( s );
		p = pdu;
	}

	// net-snmp seems to have re-used these variables to mean something else during bulk requests
	p->errstat  = nonRepeaters;
	p->errindex = maxRepetitions;

	return sync( session, pdu );
}
