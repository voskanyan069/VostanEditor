#ifndef __CMDS_COMMAND_BASE_HXX__
#define __CMDS_COMMAND_BASE_HXX__

// Standard headers
#include <vector>
#include <map>

// Headers from this project

namespace commands {
    class CommandIndex;
    class CommandBase;
    typedef std::vector<CommandBase*> COMMAND_VECTOR;
}

class commands::CommandIndex
{
    public:
        const COMMAND_VECTOR& getCommands();
        void load();

    private:
        COMMAND_VECTOR m_commands;
        void registerCommand(CommandBase*);

    public:
        CommandIndex();
        ~CommandIndex();
};

#endif // __CMDS_COMMAND_BASE_HXX__
