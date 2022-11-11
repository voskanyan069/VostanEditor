#ifndef __REMOTE_METAX_REQUESTS_HXX__
#define __REMOTE_METAX_REQUESTS_HXX__

#include "utils/Types.hxx"

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
    MetaxRequests( const std::string& sHostName );
    MetaxRequests( const std::string& sHostName, int iPort );
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

    /// Helper member functions
private:
    void esablishConnection();

    /// Helper members
private:
    int m_iPort;
    ConnType_t m_eConnType;
    std::string m_sRealHostName;
};

#endif // __REMOTE_METAX_REQUESTS_HXX__
