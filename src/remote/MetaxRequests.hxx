#ifndef __REMOTE_METAX_REQUESTS_HXX__
#define __REMOTE_METAX_REQUESTS_HXX__

#include "utils/Types.hxx"
#include "utils/Typedefs.hxx"
#include "utils/Defines.hxx"

#include <nlohmann/json.hpp>

#include <string>
#include <map>
#include <functional>

namespace Remote {
    class MetaxRequests;
}; // Remote

class Remote::MetaxRequests
{
    /// Inner type definitons
private:
    typedef std::function<bool(const std::string&, const std::string&,
            const Dimensions_t&)> UpdateContentFunc;

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

    /// Public API for interaction with node content
public:
    bool SetDimensions( const std::string& sUUID, const Dimensions_t& tDim );
    bool UpdateTitle( const std::string& sUUID, const std::string& sValue,
           const Dimensions_t& tDim = RESTORE_DIM );
    bool UpdateText( const std::string& sUUID, const std::string& sValue,
           const Dimensions_t& tDim = RESTORE_DIM );
    bool UpdateImage( const std::string& sUUID, const std::string& sValue,
           const Dimensions_t& tDim = RESTORE_DIM );
    bool SetTags( const std::string& sUUID, const Strings& vTags );
    bool AddTags( const std::string& sUUID, const Strings& vTags );
    bool SetTitleVisibility( const std::string& sUUID, bool bValue );
    bool SetTextVisibility( const std::string& sUUID, bool bValue );
    bool SetImageVisibility( const std::string& sUUID, bool bValue );
    bool SetTitleDimensions( const std::string& sUUID,
            const Dimensions_t& tDim );
    bool SetTextDimensions( const std::string& sUUID,
            const Dimensions_t& tDim );
    bool SetImageDimensions( const std::string& sUUID,
            const Dimensions_t& tDim );

    /// Public API for interaction with child nodes
public:
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

    /// Public API for interaction with child nodes content
public:
    bool SetChildDimensions( const std::string& sParentUUID,
            const std::string& sChildUUID, const Dimensions_t& tDim );
    bool SetChildLeafCount( const std::string& sParentUUID,
            const std::string& sChildUUID, int iLeafCnt );
    bool SetChildTitleVisibility( const std::string& sParentUUID,
            const std::string& sChildUUID, bool bValue );
    bool SetChildTextVisibility( const std::string& sParentUUID,
            const std::string& sChildUUID, bool bValue );
    bool SetChildImageVisibility( const std::string& sParentUUID,
            const std::string& sChildUUID, bool bValue );
    bool SetChildTitleDimensions( const std::string& sParentUUID,
            const std::string& sChildUUID, const Dimensions_t& tDim );
    bool SetChildTextDimensions( const std::string& sParentUUID,
            const std::string& sChildUUID, const Dimensions_t& tDim );
    bool SetChildImageDimensions( const std::string& sParentUUID,
            const std::string& sChildUUID, const Dimensions_t& tDim );

    /// Helper member functions
private:
    void esablishConnection();
    void addChildToLayout( nlohmann::json& oLayoutNode,
            const std::string& sChildUUID );
    void addChildToOut( nlohmann::json& oOutNode,
            const std::string& sChildUUID );
    int findNodeByUUID( const nlohmann::json& oArray,
            const std::string& sUUID );
    void str2dim( const std::string& sStr, Dimensions_t& tDim );
    Dimensions_t getPreviousDim( const nlohmann::json& oPrevDim );
    void restoreDimIfNeeded( const Dimensions_t& tDim,
            const Dimensions_t& tPrevDim, Dimensions_t& tNewDim );
    void updateNodeContent( const nlohmann::json& oData,
            const std::string& sKey, const std::string& sValue,
            const Dimensions_t& tDim, nlohmann::json& oUpdNode );
    bool changeVisibility( const std::string& sUUID, const std::string& sKey,
            bool bValue, const UpdateContentFunc& fCallback );
    bool changeDimensions( const std::string& sUUID, const std::string& sKey,
            const Dimensions_t& tDim, const UpdateContentFunc& fCallback );
    bool updateChildNodeVisibility( const std::string& sParentUUID,
            const std::string& sChildUUID, const std::string& sKey,
            bool bValue );
    bool updateChildNodeDimensions( const std::string& sParentUUID,
            const std::string& sChildUUID, const std::string& sKey,
            const Dimensions_t& tDim );

    /// Helper members
private:
    int m_iPort;
    ConnType_t m_eConnType;
    std::string m_sRealHostName;
};

#endif // __REMOTE_METAX_REQUESTS_HXX__
