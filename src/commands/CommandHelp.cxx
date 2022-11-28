#include "commands/CommandHelp.hxx"

#include "utils/VostanException.hxx"
#include "commands/CommandIndex.hxx"

commands::CommandHelp::
CommandHelp()
: CommandBase()
{
    registerParam("", "");
}

std::string commands::CommandHelp::
getName()
{
    return "help";
}

std::string commands::CommandHelp::
getDescription()
{
    return "Print avalaible commands.";
}

std::string commands::CommandHelp::
run()
{
    std::cout << "List of available commands:" << std::endl;
    std::cout << "To get detailed help on each command type '<command> -help'"
        << std::endl << std::endl;;
    std::cout << "    - " << "\e[1m" << "exit" << "\e[0m"
        << ":\n        exits form the NOC_SOLVER CLI" << std::endl;
    const commands::COMMAND_VECTOR& cmds = commands::CommandIndex::getCommands();
    for (size_t i = 0; i < cmds.size(); ++i) {
        assert(nullptr != cmds[i]);
        std::cout << "    - " << "\e[1m" << cmds[i]->getName() << "\e[0m"
            << ": " << std::endl << "        " << cmds[i]->getDescription()
            << std::endl;
    }
    return "";
}

commands::CommandHelp::
~CommandHelp()
{
}
