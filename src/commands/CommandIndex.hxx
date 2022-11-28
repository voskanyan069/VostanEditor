#ifndef __CMDS_COMMAND_INDEX_HXX__
#define __CMDS_COMMAND_INDEX_HXX__

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
        static const COMMAND_VECTOR& getCommands();
        static void load();

    private:
        static COMMAND_VECTOR m_commands;
        static void registerCommand(CommandBase*);

    private:
        CommandIndex();
        ~CommandIndex();
};

#endif // __CMDS_COMMAND_INDEX_HXX__
