#ifndef __CMDS_COMMAND_NAME_HXX__
#define __CMDS_COMMAND_NAME_HXX__

// Headers from this project
#include "commands/CommandBase.hxx"

namespace commands {
    class CommandName;
}

class commands::CommandName : public commands::CommandBase
{
    public:
        virtual std::string getName();
        virtual std::string getDescription();
        virtual std::string run();

    public:
        CommandName();
        ~CommandName();
};

#endif // __CMDS_COMMAND_NAME_HXX__
