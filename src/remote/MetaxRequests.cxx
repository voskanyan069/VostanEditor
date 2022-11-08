#include "remote/MetaxRequests.hxx"
#include "curlwrp/CurlWrapper.hxx"
#include "utils/Defines.hxx"

#include <nlohmann/json.hpp>

#include <iostream>

Remote::MetaxRequests::MetaxRequests( const std::string& sHostName )
    : m_sRealHostName( sHostName )
    , m_iPort( 0 )
    , m_eConnType( HOSTNAME )
{
    //std::size_t iIdx = sHostName.find(":");
    //if ( std::string::npos != iIdx )
    //{
    //    m_sRealHostName = sHostName.substr(0, iIdx);
    //    m_iPort = atoi(sHostName.substr(iIdx + 1));
    //    m_eConnType = IP_PORT;
    //}
    esablishConnection();
}

Remote::MetaxRequests::MetaxRequests( const std::string& sHostName, int iPort )
    : m_sRealHostName( sHostName )
    , m_iPort( iPort )
    , m_eConnType( IP_PORT )
{
    esablishConnection();
}

Remote::MetaxRequests::~MetaxRequests()
{
}

void Remote::MetaxRequests::esablishConnection()
{
    CurlWrapper::PingServer(GetHostname());
}

std::string Remote::MetaxRequests::GetHostname()
{
    if ( ConnType_t::IP_PORT == m_eConnType )
    {
        return m_sRealHostName + ":" + std::to_string(m_iPort);
    }
    return m_sRealHostName;
}

void Remote::MetaxRequests::CreateNode( /* ... */ )
{
    std::string sUrl = GetHostname() + CREATE_NODE_PATH;
    nlohmann::json oData;
    oData["test"] = "value";
    Strings vHeaders = {HEADER_CONTENT_TYPE(application/json)};
    std::string sResponse;
    CurlWrapper::PostRequest(sUrl, oData, vHeaders, sResponse);
    std::cout << "CreateNode response: " << sResponse << std::endl;
}

void Remote::MetaxRequests::DeleteNode( const std::string& sUUID )
{
}

void Remote::MetaxRequests::GetNode( const std::string& sUUID,
        std::string& sResonseData )
{
}
