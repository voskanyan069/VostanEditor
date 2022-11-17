#include "curlwrp/CurlWrapper.hxx"
#include "utils/Typedefs.hxx"
#include "io/Messaging.hxx"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <iostream>

static bool s_bConnected = false;

bool checkRequestStatus( const CURLcode& eCode, char* sErrBuf )
{
    IO::Messaging* pMsg = IO::Messaging::GetInstance();
    if ( CURLE_OK == eCode )
    {
        pMsg->ShowMessage("Request successed\n");
        return true;
    }
    sprintf(sErrBuf, "%s\n", curl_easy_strerror(eCode));
    pMsg->ShowError("Request failed: [%d] %s", (int)eCode, sErrBuf);
    return false;
}

std::size_t writeToString( void* pContent, std::size_t iSize,
        std::size_t iNmemb, void* pOutput )
{
    std::size_t iRealSize = iSize * iNmemb;
    std::string* pString = static_cast<std::string*>(pOutput);
    pString->append(static_cast<char*>(pContent), iRealSize);
    return iRealSize;
}

std::size_t writeToFile( void* pContent, std::size_t iSize,
        std::size_t iNmemb, FILE* pOutput )
{
    std::size_t iWritten = fwrite(pContent, iSize, iNmemb, pOutput);
    return iWritten;
}

curl_slist* constructHttpHeader( const Strings& vecHeaders )
{
    struct curl_slist* pList = nullptr;
    for ( const auto& sHeader : vecHeaders )
    {
        pList = curl_slist_append(pList, sHeader.c_str());
    }
    return pList;
}

bool sendCurlGetRequestImpl( const std::string& sUrl, const Strings& vecHeaders,
    std::string& sResponse )
{
    IO::Messaging* pMsg = IO::Messaging::GetInstance();
    pMsg->ShowMessage("Sending GET request to %s\n", sUrl);
    char sErrBuffer[CURL_ERROR_SIZE];
    sErrBuffer[0] = 0;
    CURL* pCURL = curl_easy_init();
    curl_slist* pHeadersList = constructHttpHeader(vecHeaders);
    curl_easy_setopt(pCURL, CURLOPT_URL, sUrl.c_str());
    curl_easy_setopt(pCURL, CURLOPT_HTTPHEADER, pHeadersList);
    curl_easy_setopt(pCURL, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, &writeToString);
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, &sResponse);
    curl_easy_setopt(pCURL, CURLOPT_ERRORBUFFER, sErrBuffer);
    CURLcode eCode = curl_easy_perform(pCURL);
    curl_slist_free_all(pHeadersList);
    curl_easy_cleanup(pCURL);
    return checkRequestStatus(eCode, sErrBuffer);
}

bool sendCurlPostRequestImpl( const std::string& sUrl, const std::string& sData,
        const Strings& vecHeaders, std::string& sResponse )
{
    IO::Messaging* pMsg = IO::Messaging::GetInstance();
    pMsg->ShowMessage("Sending POST request to %s with %s content\n",
            sUrl, sData);
    char sErrBuffer[CURL_ERROR_SIZE];
    sErrBuffer[0] = 0;
    CURL* pCURL = curl_easy_init();
    curl_slist* pHeadersList = constructHttpHeader(vecHeaders);
    curl_easy_setopt(pCURL, CURLOPT_URL, sUrl.c_str());
    curl_easy_setopt(pCURL, CURLOPT_HTTPHEADER, pHeadersList);
    curl_easy_setopt(pCURL, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(pCURL, CURLOPT_POSTFIELDS, sData.c_str());
    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, &writeToString);
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, &sResponse);
    curl_easy_setopt(pCURL, CURLOPT_ERRORBUFFER, sErrBuffer);
    CURLcode eCode = curl_easy_perform(pCURL);
    curl_slist_free_all(pHeadersList);
    curl_easy_cleanup(pCURL);
    return checkRequestStatus(eCode, sErrBuffer);
}

void showEstablishingLogs( int iSeconds )
{
    IO::Messaging* pMsg = IO::Messaging::GetInstance();
    for ( ; !s_bConnected && 0 <= iSeconds; --iSeconds )
    {
        pMsg->ShowWarning("Seconds left for trying establish connection: "
                "%d...\n", iSeconds);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

bool sendCurlPingRequestImpl( const std::string& sUrl, int iTimeoutSec = 7 )
{
    IO::Messaging* pMsg = IO::Messaging::GetInstance();
    s_bConnected = false;
    pMsg->ShowMessage("Trying to establish connection to %s ...\n", sUrl);
    std::thread outputTh(&showEstablishingLogs, iTimeoutSec);
    outputTh.detach();
    char sErrBuffer[CURL_ERROR_SIZE];
    sErrBuffer[0] = 0;
    CURL* pCURL = curl_easy_init();
    curl_easy_setopt(pCURL, CURLOPT_URL, sUrl.c_str());
    curl_easy_setopt(pCURL, CURLOPT_CONNECTTIMEOUT, iTimeoutSec * 2);
    curl_easy_setopt(pCURL, CURLOPT_NOBODY, 1);
    curl_easy_setopt(pCURL, CURLOPT_ERRORBUFFER, sErrBuffer);
    CURLcode eCode = curl_easy_perform(pCURL);
    curl_easy_cleanup(pCURL);
    s_bConnected = checkRequestStatus(eCode, sErrBuffer);
    return s_bConnected;
}

bool Remote::CurlWrapper::PingServer( const std::string& sUrl, int iTimeoutSec )
{
    return sendCurlPingRequestImpl(sUrl, iTimeoutSec);
}

bool Remote::CurlWrapper::PostRequest( const std::string& sUrl )
{
    std::string sTmp;
    return sendCurlPostRequestImpl(sUrl, "", {}, sTmp);
}

bool Remote::CurlWrapper::PostRequest(
        const std::string& sUrl,
        const std::string& sData,
        const Strings& vecHeaders,
        std::string& sResponse )
{
    return sendCurlPostRequestImpl(sUrl, sData, vecHeaders, sResponse);
}

bool Remote::CurlWrapper::PostRequest(
        const std::string& sUrl,
        const nlohmann::json& oData,
        const Strings& vecHeaders,
        std::string& sResponse )
{
    std::string sData = oData.dump();
    return sendCurlPostRequestImpl(sUrl, sData, vecHeaders, sResponse);
}

bool Remote::CurlWrapper::GetRequest(
        const std::string& sUrl,
        std::string& sResponse )
{
    return sendCurlGetRequestImpl(sUrl, {}, sResponse);
}

bool Remote::CurlWrapper::GetRequest(
        const std::string& sUrl,
        const Strings& vecHeaders,
        std::string& sResponse )
{
    return sendCurlGetRequestImpl(sUrl, vecHeaders, sResponse);
}
