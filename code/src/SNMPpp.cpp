// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <stdexcept>
#include <SNMPpp/SNMPpp.hpp>


std::string SNMPpp::version( void )
{
	/// @see LIBSNMPPPVER.
	return std::string( LIBSNMPPPVER );
}


/** @mainpage
 * The official SNMPpp web site is <https://sourceforge.net/projects/snmppp/>.
 *
 * @section gettingStarted Getting Started
 * If you're unfamiliar with SNMPpp, here are several quick suggestions on where to start looking:
 *
 * Type | Link
 * ---- | ----
 * class | SNMPpp::OID
 * class | SNMPpp::PDU
 * class | SNMPpp::Varlist
 * typedef | SNMPpp::SessionHandle
 * function | SNMPpp::sendV2Trap()
 * function | SNMPpp::get()
 * function | SNMPpp::getNext()
 * function | SNMPpp::getBulk()
 *
 * @section license License
 * The SNMPpp project uses the MIT license.  A copy of the license is included
 * in the project root directory when you clone the git repository.
 *
 * @author St&eacute;phane Charette <stephanecharette@gmail.com>
 */