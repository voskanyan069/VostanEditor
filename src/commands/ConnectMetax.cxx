#include "commands/ConnectMetax.hxx"

#include "utils/VostanException.hxx"

commands::ConnectMetax::
ConnectMetax()
: CommandBase()
{
    registerParam("-host", "Hostname of the Metax server.");
    registerParam("-port", "Port of the Metax server.");
}

std::string commands::ConnectMetax::
getName()
{
    return "metax::connect";
}

std::string commands::ConnectMetax::
getDescription()
{
    return "Connect to metax server.";
}

std::string commands::ConnectMetax::
run()
{
    std::string sHost = getParamValue<std::string>("-host");
    int iPort = -1;
    if ( isParamProvided("-port") )
    {
        iPort = getParamValue<int>("-port");
    }
    m_pMetax = new Remote::MetaxRequests(sHost, iPort);
    return "";
}

commands::ConnectMetax::
~ConnectMetax()
{
}
