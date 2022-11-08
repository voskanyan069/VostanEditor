#ifndef __REMOTE_METAX_REQUESTS_HXX__
#define __REMOTE_METAX_REQUESTS_HXX__

#include "utils/Types.hxx"

#include <string>

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
    void CreateNode( /* ... */ );
    void DeleteNode( const std::string& sUUID );
    void GetNode( const std::string& sUUID, std::string& sResonseData );

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
