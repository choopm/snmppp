// SNMPpp: https://sourceforge.net/p/snmppp/
// SNMPpp project uses the MIT license. See LICENSE for details.
// Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>

#include <stdexcept>
#include <SNMPpp/Varlist.hpp>


SNMPpp::Varlist::~Varlist( void )
{
    return;
}


SNMPpp::Varlist::Varlist( void ) :
    varlist( NULL )
{
    return;
}


SNMPpp::Varlist::Varlist( netsnmp_variable_list *vl ) :
    varlist( vl )
{
    return;
}


// free up the underlying net-snmp structure: snmp_free_varbind()
void SNMPpp::Varlist::free( void )
{
    if ( varlist != NULL )
    {
        snmp_free_varbind( varlist );
    }
    clear();

    return;
}


const SNMPpp::Varlist &SNMPpp::Varlist::getOids( SNMPpp::SetOID &s ) const
{
    s.clear();

    for ( netsnmp_variable_list *p = varlist; p != NULL; p = p->next_variable )
    {
        s.insert( SNMPpp::OID(p) );
    }

    return *this;
}


const SNMPpp::Varlist &SNMPpp::Varlist::getOids( SNMPpp::VecOID &v ) const
{
    v.clear();

    for ( netsnmp_variable_list *p = varlist; p != NULL; p = p->next_variable )
    {
        v.push_back( SNMPpp::OID(p) );
    }

    return *this;
}


SNMPpp::MapOidVarList SNMPpp::Varlist::getMap( void ) const
{
    MapOidVarList m;

    for ( netsnmp_variable_list *p = varlist; p != NULL; p = p->next_variable )
    {
        m[ SNMPpp::OID(p) ] = p;
    }

    return m;
}


bool SNMPpp::Varlist::contains( const SNMPpp::OID &o ) const
{
    bool result = false;

    for ( netsnmp_variable_list *p = varlist; p != NULL; p  = p->next_variable )
    {
        if ( o == SNMPpp::OID(p) )
        {
            // found it!
            result = true;
            break;
        }
    }

    return result;
}



SNMPpp::Varlist &SNMPpp::Varlist::addNullVar( const SNMPpp::OID &o )
{
    if ( o.empty() )
    {
        /// @throw std::invalid_argument if the OID is empty.
        throw std::invalid_argument( "Cannot add an empty OID." );
    }

    netsnmp_variable_list *p = snmp_varlist_add_variable( &varlist, o, o, ASN_NULL, 0, 0 );
    if ( p == NULL )
    {
        /// @throw std::runtime_error if net-snmp failed to add the OID.
        throw std::runtime_error( "Failed to add " + o.to_str() + " to the variable list." );
    }

    return *this;
}


SNMPpp::Varlist &SNMPpp::Varlist::addNullVars( const SNMPpp::SetOID &s )
{
    SNMPpp::SetOID::const_iterator iter;
    for (   iter  = s.begin();
            iter != s.end();
            iter ++ )
        {
            addNullVar( *iter );
        }

    return *this;
}


SNMPpp::Varlist &SNMPpp::Varlist::addNullVars( const SNMPpp::VecOID &v )
{
    for ( size_t idx = 0; idx < v.size(); idx ++ )
    {
        addNullVar( v[idx] );
    }

    return *this;
}


const netsnmp_variable_list *SNMPpp::Varlist::at( const SNMPpp::OID &o ) const
{
    netsnmp_variable_list *p = NULL;
    for ( p = varlist; p != NULL; p = p->next_variable )
    {
        if ( o == SNMPpp::OID(p) )
        {
            // found it!
            break;
        }
    }

    if ( p == NULL )
    {
        /// @throw std::invalid_argument if the varlist does not contain the requested OID.
        throw std::invalid_argument( "Varlist does not contain OID " + o.to_str() + "." );
    }

    return p;
}


netsnmp_variable_list *SNMPpp::Varlist::operator[]( const size_t idx )
{
    /**
     * @return Returns a NULL pointer if the index is larger than the number
     * of items in the variable list.
     *
     * @details Runs in O(n) since it needs to walk the linked list of
     * netsnmp_variable_list pointers.
     */
    netsnmp_variable_list *p = varlist;
    for ( size_t counter = 0; p != NULL && counter < idx; counter ++ )
    {
        p = p->next_variable;
    }

    return p;
}


SNMPpp::OID SNMPpp::Varlist::firstOID( void ) const
{
    if ( empty() )
    {
        /// @throw std::logic_error if the varlist is empty.
        throw std::logic_error( "Cannot get an OID from an empty variable list." );
    }

    return SNMPpp::OID(varlist);
}


bool SNMPpp::Varlist::getBool( const SNMPpp::OID &o ) const
{
    const netsnmp_variable_list *vl = at( o );

    if ( vl->type != ASN_BOOLEAN )
    {
        /// @throw std::invalid_argument if the requested OID is not `ASN_BOOLEAN`.
        throw std::invalid_argument( "OID " + o.to_str() + " is not a boolean type." );
    }

    return *vl->val.integer == 1; // rfc1212, true=1, false=2
}


long SNMPpp::Varlist::getLong( const SNMPpp::OID &o ) const
{
    const netsnmp_variable_list *vl = at( o );

    if ( vl->type != ASN_INTEGER )
    {
        /// @throw std::invalid_argument if the requested OID is not `ASN_INTEGER`.
        throw std::invalid_argument( "OID " + o.to_str() + " is not a numeric type." );
    }

    return *vl->val.integer;
}


std::string SNMPpp::Varlist::getString( const SNMPpp::OID &o ) const
{
    const netsnmp_variable_list *vl = at( o );

    if ( vl->type != ASN_OCTET_STR )
    {
        /// @throw std::invalid_argument if the requested OID is not `ASN_OCTET_STR`.
        throw std::invalid_argument( "OID " + o.to_str() + " is not a string." );
    }

    return std::string( (const char *)vl->val.string, vl->val_len );
}


SNMPpp::OID SNMPpp::Varlist::getOID( const SNMPpp::OID &o ) const
{
    const netsnmp_variable_list *vl = at( o );

    if ( vl->type != ASN_OBJECT_ID )
    {
        /// @throw std::invalid_argument if the requested OID is not `ASN_OBJECT_ID`.
        throw std::invalid_argument( "OID " + o.to_str() + " is not a OID." );
    }

    return SNMPpp::OID( vl->val.objid, vl->val_len/sizeof(unsigned long) );
}


std::string SNMPpp::Varlist::asString( const SNMPpp::OID &o ) const
{
    const netsnmp_variable_list *vl = at( o );

    u_char *buf = NULL;
    size_t buf_len = 0;
    size_t out_len = 0;
    sprint_realloc_value( &buf, &buf_len, &out_len, 1, o, o, vl );

    // The buffer should look something like one of these examples:
    //
    //      OID: iso.3.6.1.6.3.16.2.2.1
    //      STRING: "The SNMP Management Architecture MIB."
    //
    // We need to extract the two parts -- the type followed by the value.

    std::string asn( (char*) buf );
    std::string txt( (char*) buf );

    ::free( buf );

    size_t pos = asn.find( ": " );
    if ( pos != std::string::npos )
    {
        asn.erase( pos, std::string::npos );
        txt.erase( 0, pos + 2 );

        // strings start and end with double quotes -- get rid of those double quotes
        const size_t len = txt.size();
        if (    asn         == "STRING" &&
                len         >= 2        &&
                txt[0]      == '\"'     &&
                txt[len-1]  == '\"'     )
        {
            txt.erase( len - 1 );   // erase trailing double quote
            txt.erase( 0, 1 );      // erase leading double quote
        }
    }

    return txt;
}


std::ostream &operator<<( std::ostream &os, const SNMPpp::Varlist &varlist )
{
    SNMPpp::VecOID v;
    varlist.getOids( v );

    os << "Number of OIDs in the variable list: " << v.size() << std::endl;

    // list all of the OIDs in the varlist
    for ( size_t idx = 0; idx < v.size(); idx ++ )
    {
        os << "\t" << v[idx] << ": ASN type=" << varlist.asnType( v[idx] ) << ", txt=" << varlist.asString( v[idx] ) << std::endl;
    }

    return os;
}
