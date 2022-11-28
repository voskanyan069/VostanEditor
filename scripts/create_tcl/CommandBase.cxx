#include "commands/CommandName.hxx"

#include "utils/VostanException.hxx"

commands::CommandName::
CommandName()
: CommandBase()
{
    registerParam("", "");
}

std::string commands::CommandName::
getName()
{
    return "cmd_name";
}

std::string commands::CommandName::
getDescription()
{
    return "CommandDescription";
}

std::string commands::CommandName::
run()
{
    return "";
}

commands::CommandName::
~CommandName()
{
}
