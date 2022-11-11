#include "remote/MetaxRequests.hxx"
#include "curlwrp/CurlWrapper.hxx"
#include "utils/Defines.hxx"

#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <ctime>

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

std::string Remote::MetaxRequests::CreateNode( const std::string& sNodeTitle )
{
    std::string sUrl = GetHostname() + CREATE_NODE_PATH;
    nlohmann::json oData = nlohmann::json::parse(NODE_JSON);
    long int iTime = std::time(0);
    oData["created"] = iTime;
    oData["updated"] = iTime;
    oData["title"]["value"] = sNodeTitle;
    Strings vHeaders = {HEADER_CONTENT_TYPE(application/json)};
    std::string sResponse;
    std::string sUUID;
    if ( Remote::CurlWrapper::PostRequest(sUrl, oData, vHeaders, sResponse) )
    {
        nlohmann::json oResponse = nlohmann::json::parse(sResponse);
        sUUID = oResponse["uuid"];
        std::map<std::string, std::string> mapUpdates = {
            {"id", sUUID},
            {"uuid", sUUID}
        };
        if ( UpdateNode(sUUID, mapUpdates) )
        {
            return sUUID;
        }
        else
        {
            DeleteNode(sUUID);
        }
    }
    return std::string();
}

bool Remote::MetaxRequests::UpdateNode( const std::string& sUUID,
        const std::map<std::string, std::string>& mapPairs )
{
    std::string sUrl = GetHostname() + UPDATE_NODE_PATH + sUUID;
    std::string sResponse;
    GetNode(sUUID, sResponse);
    nlohmann::json oData = nlohmann::json::parse(sResponse);
    for ( const auto& it : mapPairs )
    {
        oData[it.first] = it.second;
    }
    Strings vHeaders = {HEADER_CONTENT_TYPE(application/json)};
    return Remote::CurlWrapper::PostRequest(sUrl, oData, vHeaders, sResponse);
}

bool Remote::MetaxRequests::UpdateNode( const std::string& sUUID,
        const nlohmann::json& oData )
{
    std::string sUrl = GetHostname() + UPDATE_NODE_PATH + sUUID;
    std::string sResponse;
    Strings vHeaders = {HEADER_CONTENT_TYPE(application/json)};
    return Remote::CurlWrapper::PostRequest(sUrl, oData, vHeaders, sResponse);
}

bool Remote::MetaxRequests::UpdateNode( const std::string& sUUID,
        const std::string& sKey, const nlohmann::json& oData )
{
    std::string sUrl = GetHostname() + UPDATE_NODE_PATH + sUUID;
    std::string sResponse;
    GetNode(sUUID, sResponse);
    nlohmann::json oPData = nlohmann::json::parse(sResponse);
    oPData[sKey] = oData;
    Strings vHeaders = {HEADER_CONTENT_TYPE(application/json)};
    return Remote::CurlWrapper::PostRequest(sUrl, oPData, vHeaders, sResponse);
}

bool Remote::MetaxRequests::GetNode( const std::string& sUUID,
        std::string& sResponse )
{
    std::string sUrl = GetHostname() + GET_NODE_PATH + sUUID;
    return Remote::CurlWrapper::GetRequest(sUrl, sResponse);
}

bool Remote::MetaxRequests::DeleteNode( const std::string& sUUID )
{
    std::string sUrl = GetHostname() + DELETE_NODE_PATH + sUUID;
    return Remote::CurlWrapper::PostRequest(sUrl);
}
