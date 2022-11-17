#ifndef __REMOTE_METAX_REQUESTS_HXX__
#define __REMOTE_METAX_REQUESTS_HXX__

#include "utils/Types.hxx"
#include "utils/Typedefs.hxx"

#include <nlohmann/json.hpp>

#include <string>
#include <map>

namespace Remote {
    class MetaxRequests;
}; // Remote

class Remote::MetaxRequests
{
    /// Public API for instanse creation
public:
    MetaxRequests( const std::string& sHostName, int iPort = -1 );
    ~MetaxRequests();

    /// Getter/Setters
public:
    std::string GetHostname();

    /// Public API for interaction with nodes
public:
    std::string CreateNode( const std::string& sNodeTitle );
    bool UpdateNode( const std::string& sUUID,
            const std::map<std::string, std::string>& mapPairs );
    bool UpdateNode( const std::string& sUUID, const nlohmann::json& oData );
    bool UpdateNode( const std::string& sUUID, const std::string& sKey,
            const nlohmann::json& oData );
    bool GetNode( const std::string& sUUID, std::string& sResponse );
    bool DeleteNode( const std::string& sUUID );
    bool AddChildNode( const std::string& sParentUUID,
            const std::string& sChildUUID, bool bConnectNode = false );
    bool AddChildNode( const std::string& sParentUUID,
            const Strings& vecChildUUID, bool bConnectNode = false );
    bool ConnectChildNode( const std::string& sParentUUID,
            const std::string& sChildUUID );
    bool ConnectChildNode( const std::string& sParentUUID,
            const Strings& vecChildUUID );
    bool DisconnectChildNode( const std::string& sParentUUID,
            const std::string& sChildUUID );
    bool DisconnectChildNode( const std::string& sParentUUID,
            const Strings& vecChildUUID );
    bool DeleteChildNode( const std::string& sParentUUID,
            const std::string& sChildUUID, bool bDisconnectNode = false );
    bool DeleteChildNode( const std::string& sParentUUID,
            const Strings& vecChildUUID, bool bDisconnectNode = false );

    /* TODO
    bool SetDimensions( const std::string& sUUID, ... );
    bool SetChildDimensions( const std::string& sUUID, ... );
    bool SetChildLeafCount( const std::string& sUUID, ... );
    */

    /// Public API for interaction with node content
public:
    /* TODO
    bool UpdateTitle( const std::string& sUUID, ... );
    bool UpdateText( const std::string& sUUID, ... );
    bool UpdateImage( const std::string& sUUID, ... );
    bool UpdateTags( const std::string& sUUID, ... );
    bool SetTitleVisibility( const std::string& sUUID, ... );
    bool SetTextVisibility( const std::string& sUUID, ... );
    bool SetImageVisibility( const std::string& sUUID, ... );
    bool SetTitleDimensions( const std::string& sUUID, ... );
    bool SetTextDimensions( const std::string& sUUID, ... );
    bool SetImageDimensions( const std::string& sUUID, ... );
    bool SetChildTitleVisibility( const std::string& sUUID, ... );
    bool SetChildTextVisibility( const std::string& sUUID, ... );
    bool SetChildImageVisibility( const std::string& sUUID, ... );
    bool SetChildTitleDimensions( const std::string& sUUID, ... );
    bool SetChildTextDimensions( const std::string& sUUID, ... );
    bool SetChildImageDimensions( const std::string& sUUID, ... );
    */

    /// Helper member functions
private:
    void esablishConnection();
    void addChildToLayout( nlohmann::json& oLayoutNode,
            const std::string& sChildUUID );
    void addChildToOut( nlohmann::json& oOutNode,
            const std::string& sChildUUID );
    int findNodeByUUID( const nlohmann::json& oArray,
            const std::string& sUUID );

    /// Helper members
private:
    int m_iPort;
    ConnType_t m_eConnType;
    std::string m_sRealHostName;
};

#endif // __REMOTE_METAX_REQUESTS_HXX__
