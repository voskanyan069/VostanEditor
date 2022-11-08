#ifndef __CURL_WRAPPER_HXX__
#define __CURL_WRAPPER_HXX__

#include <string>

namespace nlohmann {
    class json;
}; // nlohmann

namespace CurlWrapper {
    void PostRequest( const std::string& sHostName,
                      const std::string& sUrlPath,
                      const std::string& sContent );
    void PostRequest( const std::string& sHostName,
                      const std::string& sUrlPath,
                      const nlohmann::json& oData );
    void GetRequest( const std::string& sHostName,
                     const std::string& sUrlPath,
                     std::string& sReponse );
}; // CurlWrapper

#endif // __CURL_WRAPPER_HXX__
