#include "commands/CommandIndex.hxx"
#include "commands/CommandTest.hxx"

commands::COMMAND_VECTOR commands::CommandIndex::m_commands;

void commands::CommandIndex::
load()
{
    registerCommand(new CommandTest());
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
