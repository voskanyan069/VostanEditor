#include "commands/CommandIndex.hxx"
#include "commands/ConnectMetax.hxx"
#include "commands/CreateNode.hxx"
#include "commands/CommandHelp.hxx"
// New Include

commands::COMMAND_VECTOR commands::CommandIndex::m_commands;

void commands::CommandIndex::
load()
{
    registerCommand(new ConnectMetax());
    registerCommand(new CreateNode());
	registerCommand(new CommandHelp());
// New Command
}

const commands::COMMAND_VECTOR& commands::CommandIndex::
getCommands()
{
    return m_commands;
}

void commands::CommandIndex::
registerCommand(CommandBase* command)
{
    m_commands.push_back(command);
}
