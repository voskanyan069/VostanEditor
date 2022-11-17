#ifndef __IO_MESSAGING_HXX__
#define __IO_MESSAGING_HXX__

#include "utils/Defines.hxx"
#include "utils/Types.hxx"

#include <boost/format.hpp>

#include <string>
#include <vector>
#include <fstream>

namespace IO {
    class Messaging;
}; // IO

class IO::Messaging
{
public:
    static Messaging* GetInstance();
    Messaging( const Messaging& ) = delete;
    void operator=( const Messaging& ) = delete;
    virtual ~Messaging() = default;

private:
    Messaging() = default;

public:
    void RegisterStream( std::ostream* pStream );

public:
    template <typename... Args>
    void ShowMessage( const std::string& sFmt, Args&&... vArgs )
    {
        std::string sMsg = boost::str((boost::format(sFmt) % ... % vArgs));
        showMessage(MessageType_t::STD, sMsg);
    }

    template <typename... Args>
    void ShowError( const std::string& sFmt, Args&&... vArgs )
    {
        std::string sMsg = boost::str((boost::format(sFmt) % ... % vArgs));
        showMessage(MessageType_t::ERROR, sMsg);
    }

    template <typename... Args>
    void ShowWarning( const std::string& sFmt, Args&&... vArgs )
    {
        std::string sMsg = boost::str((boost::format(sFmt) % ... % vArgs));
        showMessage(MessageType_t::WARN, sMsg);
    }

protected:
    void dumpTime( std::ostream* pStream );
    void dumpMsgType( std::ostream* pStream, const MessageType_t& eMsgType );
    void checkForInnerMsgType( const std::string& sMessage,
            MessageType_t& eType );
    void showMessage( const MessageType_t& eMsgType, const std::string& sMsg );
    void setConsoleColor( const MessageType_t& eMsgType, std::ostream* pOut );
    void resetConsole( std::ostream* pStream );

protected:
    std::vector<std::ostream*> m_vecStreams;
};

#endif // __IO_MESSAGING_HXX__
