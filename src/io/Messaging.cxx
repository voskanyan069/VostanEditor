#include "io/Messaging.hxx"
#include "utils/Defines.hxx"
#include "utils/Types.hxx"

#include <string>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <mutex>

static std::mutex s_oMsgMutex;

IO::Messaging* IO::Messaging::GetInstance()
{
    static IO::Messaging* pMsg = new IO::Messaging();
    return pMsg;
}

void IO::Messaging::RegisterStream( std::ostream* pStream )
{
    m_vecStreams.push_back(pStream);
}

void IO::Messaging::dumpTime( std::ostream* pStream )
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    *pStream << " [" << buf << "]";
}

void IO::Messaging::dumpMsgType( std::ostream* pStream,
        const MessageType_t& eMsgType )
{
    std::string sMsgType;
    switch ( eMsgType )
    {
    case MessageType_t::STD:
        {
            sMsgType = "Info";
            break;
        }
    case MessageType_t::WARN:
        {
            sMsgType = "Warning";
            break;
        }
    case MessageType_t::ERROR:
        {
            sMsgType = "Error";
            break;
        }
    }
    *pStream << " " << sMsgType << ": ";
}

void IO::Messaging::checkForInnerMsgType( const std::string& sMessage,
        MessageType_t& eType )
{
    std::string sMsg = sMessage;
    std::transform(sMsg.begin(), sMsg.end(), sMsg.begin(), tolower);
    if ( std::string::npos != sMsg.find("error") )
    {
        eType = MessageType_t::ERROR;
    }
    else if ( std::string::npos != sMsg.find("warn") ||
            std::string::npos != sMsg.find("warning") )
    {
        eType = MessageType_t::WARN;
    }
    else if ( std::string::npos != sMsg.find("info") ||
            std::string::npos != sMsg.find("std") )
    {
        eType = MessageType_t::STD;
    }
}

void IO::Messaging::showMessage( const MessageType_t& eMsgType,
        const std::string& sMessage )
{
    MessageType_t eType = eMsgType;
    checkForInnerMsgType(sMessage, eType);
    std::lock_guard<std::mutex> lock(s_oMsgMutex);
    for ( std::ostream* pStream : m_vecStreams )
    {
        setConsoleColor(eType, pStream);
        if ( &std::cout != pStream )
        {
            dumpTime(pStream);
            dumpMsgType(pStream, eType);
        }
        *pStream << sMessage;
        resetConsole(pStream);
    }
}

void IO::Messaging::setConsoleColor( const MessageType_t& eMsgType,
        std::ostream* pStream )
{
    if ( &std::cout != pStream )
    {
        return;
    }
    switch ( eMsgType )
    {
    case MessageType_t::STD:
        {
            *pStream << STD_TERMINAL;
            break;
        }
    case MessageType_t::WARN:
        {
            *pStream << WARN_TERMINAL;
            break;
        }
    case MessageType_t::ERROR:
        {
            *pStream << ERROR_TERMINAL;
            break;
        }
    }
}

void IO::Messaging::resetConsole( std::ostream* pStream )
{
    if ( &std::cout != pStream )
    {
        return;
    }
    *pStream << RESET_TERMINAL;
}
