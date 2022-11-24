#include "remote/MetaxRequests.hxx"
#include "curlwrp/CurlWrapper.hxx"
#include "utils/Defines.hxx"

#include <nlohmann/json.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <regex>

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
    if ( !CurlWrapper::PingServer(GetHostname()) )
    {
        std::exit(-1);
    }
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

void Remote::MetaxRequests::str2dim( const std::string& sStr,
        Dimensions_t& tDim )
{
    std::vector<int> vTokens;
    std::string sToken;
    std::string sDim(sStr);
    boost::replace_all(sDim, "undefined", "-1");
    std::istringstream sTokenStream(sDim);
    while ( std::getline(sTokenStream, sToken, ':') )
    {
        vTokens.push_back(stoi(sToken));
    }
    tDim.show = vTokens[0];
    tDim.top = vTokens[1];
    tDim.left = vTokens[2];
    tDim.width = vTokens[3];
    tDim.height = vTokens[4];
}

Dimensions_t Remote::MetaxRequests::getPreviousDim(
        const nlohmann::json& oPrevDim )
{
    Dimensions_t tDim = DEFAULT_DIM;
    std::string sDim = oPrevDim["view"];
    str2dim(sDim, tDim);
    return tDim;
}

void Remote::MetaxRequests::restoreDimIfNeeded( const Dimensions_t& tDim,
        const Dimensions_t& tPrevDim, Dimensions_t& tNewDim )
{
    if ( tDim.show == -2 )
    {
        tNewDim.show = tPrevDim.show;
    }
    if ( tDim.top == -2 )
    {
       tNewDim.top = tPrevDim.top;
    } 
    if ( tDim.left == -2 )
    {
       tNewDim.left = tPrevDim.left;
    } 
    if ( tDim.width == -2 )
    {
       tNewDim.width = tPrevDim.width;
    } 
    if ( tDim.height == -2 )
    {
       tNewDim.height = tPrevDim.height;
    } 
}

bool Remote::MetaxRequests::changeVisibility( const std::string& sUUID,
        const std::string& sKey, bool bValue,
        const UpdateContentFunc& fCallback )
{
    Dimensions_t tDim = RESTORE_DIM;
    tDim.show = bValue;
    return changeDimensions(sUUID, sKey, tDim, fCallback);
}

bool Remote::MetaxRequests::changeDimensions( const std::string& sUUID,
        const std::string& sKey, const Dimensions_t& tDim,
        const UpdateContentFunc& fCallback )
{
    std::string sResponse;
    if ( !GetNode(sUUID, sResponse) )
    {
        return false;
    }
    nlohmann::json oData = nlohmann::json::parse(sResponse);
    return fCallback(sUUID, oData[sKey]["value"], tDim);
}


void Remote::MetaxRequests::updateNodeContent( const nlohmann::json& oData,
        const std::string& sKey, const std::string& sValue,
        const Dimensions_t& tDim, nlohmann::json& oUpdNode )
{
    Dimensions_t tNewDim = tDim;
    Dimensions_t tPrevDim = getPreviousDim(oData[sKey]);
    restoreDimIfNeeded(tDim, tPrevDim, tNewDim);
    oUpdNode["view"] = DIM2STR(tNewDim);
    oUpdNode["value"] = sValue;
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

bool Remote::MetaxRequests::SetDimensions( const std::string& sUUID,
        const Dimensions_t& tDim )
{
    std::string sDim = DIM2STR(tDim);
    return UpdateNode(sUUID, "view", sDim);
}

bool Remote::MetaxRequests::UpdateTitle( const std::string& sUUID,
        const std::string& sValue, const Dimensions_t& tDim )
{
    std::string sResponse;
    if ( !GetNode(sUUID, sResponse) )
    {
        return false;
    }
    nlohmann::json oData = nlohmann::json::parse(sResponse);
    nlohmann::json oUpdData;
    updateNodeContent(oData, "title", sValue, tDim, oUpdData);
    return UpdateNode(sUUID, "title", oUpdData);
}

bool Remote::MetaxRequests::UpdateText( const std::string& sUUID,
        const std::string& sValue, const Dimensions_t& tDim )
{
    std::string sResponse;
    if ( !GetNode(sUUID, sResponse) )
    {
        return false;
    }
    nlohmann::json oData = nlohmann::json::parse(sResponse);
    nlohmann::json oUpdData;
    updateNodeContent(oData, "txt", sValue, tDim, oUpdData);
    return UpdateNode(sUUID, "txt", oUpdData);
}

bool Remote::MetaxRequests::UpdateImage( const std::string& sUUID,
        const std::string& sValue, const Dimensions_t& tDim )
{
    std::string sResponse;
    if ( !GetNode(sUUID, sResponse) )
    {
        return false;
    }
    nlohmann::json oData = nlohmann::json::parse(sResponse);
    nlohmann::json oUpdData;
    updateNodeContent(oData, "img", sValue, tDim, oUpdData);
    return UpdateNode(sUUID, "img", oUpdData);
}

bool Remote::MetaxRequests::SetTags( const std::string& sUUID,
        const Strings& vTags )
{
    nlohmann::json oData = nlohmann::json::array();
    for ( const auto& sTag : vTags )
    {
        oData.push_back(sTag);
    }
    return UpdateNode(sUUID, "tags", oData);
}

bool Remote::MetaxRequests::AddTags( const std::string& sUUID,
        const Strings& vTags )
{
    std::string sResponse;
    if ( !GetNode(sUUID, sResponse) )
    {
        return false;
    }
    nlohmann::json oData = nlohmann::json::parse(sResponse);
    nlohmann::json oTagsData = oData["tags"];
    for ( const auto& sTag : vTags )
    {
        oTagsData.push_back(sTag);
    }
    return UpdateNode(sUUID, "tags", oTagsData);
}

bool Remote::MetaxRequests::SetTitleVisibility( const std::string& sUUID,
        bool bValue )
{
    UpdateContentFunc fCallback = std::bind(&Remote::MetaxRequests::UpdateTitle,
            this, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3);
    return changeVisibility(sUUID, "title", bValue, fCallback);
}

bool Remote::MetaxRequests::SetTextVisibility( const std::string& sUUID,
        bool bValue )
{
    UpdateContentFunc fCallback = std::bind(&Remote::MetaxRequests::UpdateText,
            this, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3);
    return changeVisibility(sUUID, "txt", bValue, fCallback);
}

bool Remote::MetaxRequests::SetImageVisibility( const std::string& sUUID,
        bool bValue )
{
    UpdateContentFunc fCallback = std::bind(&Remote::MetaxRequests::UpdateImage,
            this, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3);
    return changeVisibility(sUUID, "img", bValue, fCallback);
}

bool Remote::MetaxRequests::SetTitleDimensions( const std::string& sUUID,
        const Dimensions_t& tDim )
{
    UpdateContentFunc fCallback = std::bind(&Remote::MetaxRequests::UpdateTitle,
            this, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3);
    return changeDimensions(sUUID, "title", tDim, fCallback);
}

bool Remote::MetaxRequests::SetTextDimensions( const std::string& sUUID,
        const Dimensions_t& tDim )
{
    UpdateContentFunc fCallback = std::bind(&Remote::MetaxRequests::UpdateText,
            this, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3);
    return changeDimensions(sUUID, "txt", tDim, fCallback);
}

bool Remote::MetaxRequests::SetImageDimensions( const std::string& sUUID,
        const Dimensions_t& tDim )
{
    UpdateContentFunc fCallback = std::bind(&Remote::MetaxRequests::UpdateImage,
            this, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3);
    return changeDimensions(sUUID, "img", tDim, fCallback);
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
