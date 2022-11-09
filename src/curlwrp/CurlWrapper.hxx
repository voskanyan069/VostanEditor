#ifndef __CURL_WRAPPER_HXX__
#define __CURL_WRAPPER_HXX__

/// Headers from local libraries
#include "utils/Typedefs.hxx"

/// Headers from third party libraries
#include <curl/curl.h>
#include <nlohmann/json.hpp>

/// Headers from standard libraries
#include <string>
#include <vector>

/// Forward declarations
namespace Remote { namespace CurlWrapper {
    bool PingServer( const std::string& sUrl, int iTimeoutSec = 7 );
    bool PostRequest( const std::string& sUrl );
    bool PostRequest( const std::string& sUrl,
            const std::string& sData,
            const Strings& vecHeaders,
            std::string& sResponse );
    bool PostRequest( const std::string& sUrl,
            const nlohmann::json& oData,
            const Strings& vecHeaders,
            std::string& sResponse );
    bool GetRequest( const std::string& sUrl,
            const Strings& vecHeaders,
            std::string& sResponse );
}; }; // Remote::CurlWrapper

#endif // __CURL_WRAPPER_HXX__
