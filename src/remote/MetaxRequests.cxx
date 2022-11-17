#include "remote/MetaxRequests.hxx"
#include "curlwrp/CurlWrapper.hxx"
#include "utils/Defines.hxx"

#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <ctime>

Remote::MetaxRequests::MetaxRequests( const std::string& sHostName, int iPort )
    : m_sRealHostName( sHostName )
    , m_iPort( iPort )
    , m_eConnType( (-1 == iPort) ? HOSTNAME : IP_PORT )
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

void Remote::MetaxRequests::addChildToLayout( nlohmann::json& oLayoutNode,
       const std::string& sChildUUID )
{
    nlohmann::json oLayoutChildNode;
    oLayoutChildNode["node"] = nlohmann::json::object();
    oLayoutChildNode["node"]["id"] = sChildUUID;
    oLayoutChildNode["node"]["domain"] = "";
    oLayoutChildNode["view"] = "1:1:1:1:1";
    oLayoutChildNode["title"] = nlohmann::json::object();
    oLayoutChildNode["title"]["view"] = "1:1:1:1:1";
    oLayoutChildNode["img"] = nlohmann::json::object();
    oLayoutChildNode["img"]["view"] = "1:1:1:1:1";
    oLayoutChildNode["txt"] = nlohmann::json::object();
    oLayoutChildNode["txt"]["view"] = "1:1:1:1:1";
    oLayoutChildNode["leaf"] = 0;
    oLayoutNode.push_back(oLayoutChildNode);
}

void Remote::MetaxRequests::addChildToOut( nlohmann::json& oOutNode,
       const std::string& sChildUUID )
{
    nlohmann::json oOutChildNode;
    oOutChildNode["node"] = nlohmann::json::object();
    oOutChildNode["node"]["id"] = sChildUUID;
    oOutChildNode["node"]["domain"] = "";
    oOutChildNode["tag"] = "";
    oOutNode.push_back(oOutChildNode);
}

int Remote::MetaxRequests::findNodeByUUID( const nlohmann::json& oArray,
        const std::string& sUUID )
{
    for ( int i = 0; i < oArray.size(); ++i )
    {
        if ( sUUID == oArray[i]["node"]["id"] )
        {
            return i;
        }
    }
    return -1;
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

bool Remote::MetaxRequests::AddChildNode( const std::string& sParentUUID,
        const std::string& sChildUUID, bool bConnectNode )
{
    std::string sParentResponse;
    std::string sChildResponse;
    if ( !GetNode(sParentUUID, sParentResponse) )
    {
        return false;
    }
    if ( !GetNode(sChildUUID, sChildResponse) )
    {
        return false;
    }
    nlohmann::json oParentNode = nlohmann::json::parse(sParentResponse);
    nlohmann::json oLayoutNode = oParentNode["layout"];
    addChildToLayout(oLayoutNode, sChildUUID);
    if ( bConnectNode )
    {
        nlohmann::json oOutNode = oParentNode["out"];
        addChildToOut(oOutNode, sChildUUID);
        if ( !UpdateNode(sParentUUID, "out", oOutNode) )
        {
            return false;
        }
    }
    return UpdateNode(sParentUUID, "layout", oLayoutNode);
}

bool Remote::MetaxRequests::AddChildNode( const std::string& sParentUUID,
        const Strings& vecChildUUID, bool bConnectNode )
{
    std::string sParentResponse;
    std::string sChildResponse;
    if ( !GetNode(sParentUUID, sParentResponse) )
    {
        return false;
    }
    nlohmann::json oParentNode = nlohmann::json::parse(sParentResponse);
    nlohmann::json oLayoutNode = oParentNode["layout"];
    nlohmann::json oOutNode = oParentNode["out"];
    for ( const auto& sChildUUID : vecChildUUID )
    {
        if ( !GetNode(sChildUUID, sChildResponse) )
        {
            continue;
        }
        addChildToLayout(oLayoutNode, sChildUUID);
        if ( bConnectNode )
        {
            addChildToOut(oOutNode, sChildUUID);
        }
    }
    if ( bConnectNode )
    {
        if ( !UpdateNode(sParentUUID, "out", oOutNode) )
        {
            return false;
        }
    }
    return UpdateNode(sParentUUID, "layout", oLayoutNode);
}

bool Remote::MetaxRequests::ConnectChildNode( const std::string& sParentUUID,
        const std::string& sChildUUID )
{
    std::string sParentResponse;
    std::string sChildResponse;
    if ( !GetNode(sParentUUID, sParentResponse) )
    {
        return false;
    }
    if ( !GetNode(sChildUUID, sChildResponse) )
    {
        return false;
    }
    nlohmann::json oParentNode = nlohmann::json::parse(sParentResponse);
    nlohmann::json oOutNode = oParentNode["out"];
    addChildToOut(oOutNode, sChildUUID);
    return UpdateNode(sParentUUID, "out", oOutNode);
}

bool Remote::MetaxRequests::ConnectChildNode( const std::string& sParentUUID,
        const Strings& vecChildUUID )
{
    std::string sParentResponse;
    std::string sChildResponse;
    if ( !GetNode(sParentUUID, sParentResponse) )
    {
        return false;
    }
    nlohmann::json oParentNode = nlohmann::json::parse(sParentResponse);
    nlohmann::json oOutNode = oParentNode["out"];
    for ( const auto& sChildUUID : vecChildUUID )
    {
        if ( !GetNode(sChildUUID, sChildResponse) )
        {
            return false;
        }
        addChildToOut(oOutNode, sChildUUID);
    }
    return UpdateNode(sParentUUID, "out", oOutNode);
}

bool Remote::MetaxRequests::DisconnectChildNode( const std::string& sParentUUID,
        const std::string& sChildUUID )
{
    std::string sParentResponse;
    if ( !GetNode(sParentUUID, sParentResponse) )
    {
        return false;
    }
    nlohmann::json oParentNode = nlohmann::json::parse(sParentResponse);
    nlohmann::json oOutNode = oParentNode["out"];
    int iChildIdx = findNodeByUUID(oOutNode, sChildUUID);
    if ( -1 == iChildIdx )
    {
        return false;
    }
    oOutNode.erase(iChildIdx);
    return UpdateNode(sParentUUID, "out", oOutNode);
}

bool Remote::MetaxRequests::DisconnectChildNode( const std::string& sParentUUID,
        const Strings& vecChildUUID )
{
    std::string sParentResponse;
    if ( !GetNode(sParentUUID, sParentResponse) )
    {
        return false;
    }
    nlohmann::json oParentNode = nlohmann::json::parse(sParentResponse);
    nlohmann::json oOutNode = oParentNode["out"];
    for ( const auto& sChildUUID : vecChildUUID )
    {
        int iChildIdx = findNodeByUUID(oOutNode, sChildUUID);
        if ( -1 == iChildIdx )
        {
            continue;
        }
        oOutNode.erase(iChildIdx);
    }
    return UpdateNode(sParentUUID, "out", oOutNode);
}

bool Remote::MetaxRequests::DeleteChildNode( const std::string& sParentUUID,
        const std::string& sChildUUID, bool bDisconnectNode )
{
    std::string sParentResponse;
    if ( !GetNode(sParentUUID, sParentResponse) )
    {
        return false;
    }
    nlohmann::json oParentNode = nlohmann::json::parse(sParentResponse);
    nlohmann::json oLayoutNode = oParentNode["layout"];
    int iChildIdx = findNodeByUUID(oLayoutNode, sChildUUID);
    if ( -1 == iChildIdx )
    {
        return false;
    }
    oLayoutNode.erase(iChildIdx);
    if ( bDisconnectNode )
    {
        nlohmann::json oOutNode = oParentNode["out"];
        int iIdx = findNodeByUUID(oOutNode, sChildUUID);
        if ( -1 != iChildIdx )
        {
            oOutNode.erase(iIdx);
        }
        UpdateNode(sParentUUID, "out", oOutNode);
    }
    return UpdateNode(sParentUUID, "layout", oLayoutNode);
}

bool Remote::MetaxRequests::DeleteChildNode( const std::string& sParentUUID,
        const Strings& vecChildUUID, bool bDisconnectNode )
{
    std::string sParentResponse;
    if ( !GetNode(sParentUUID, sParentResponse) )
    {
        return false;
    }
    nlohmann::json oParentNode = nlohmann::json::parse(sParentResponse);
    nlohmann::json oLayoutNode = oParentNode["layout"];
    nlohmann::json oOutNode = oParentNode["out"];
    for ( const auto& sChildUUID : vecChildUUID )
    {
        int iChildIdx = findNodeByUUID(oLayoutNode, sChildUUID);
        if ( -1 == iChildIdx )
        {
            continue;
        }
        oLayoutNode.erase(iChildIdx);
        if ( bDisconnectNode )
        {
            int iIdx = findNodeByUUID(oOutNode, sChildUUID);
            if ( -1 != iChildIdx )
            {
                oOutNode.erase(iIdx);
            }
        }
    }
    if ( bDisconnectNode )
    {
        UpdateNode(sParentUUID, "out", oOutNode);
    }
    return UpdateNode(sParentUUID, "layout", oLayoutNode);
}
