#ifndef __CMDS_HELP_HXX__
#define __CMDS_HELP_HXX__

// Headers from this project
#include "commands/CommandBase.hxx"

namespace commands {
    class CommandHelp;
}

class commands::CommandHelp : public commands::CommandBase
{
    public:
        virtual std::string getName();
        virtual std::string getDescription();
        virtual std::string run();

    public:
        CommandHelp();
        ~CommandHelp();
};

#endif // __CMDS_HELP_HXX__
