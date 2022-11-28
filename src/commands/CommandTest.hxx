#ifndef __CMDS_COMMAND_TEST_HXX__
#define __CMDS_COMMAND_TEST_HXX__

// Headers from this project
#include "commands/CommandBase.hxx"

namespace commands {
    class CommandTest;
}

class commands::CommandTest : public commands::CommandBase
{
    public:
        virtual std::string getName();
        virtual std::string getDescription();
        virtual std::string run();

    public:
        CommandTest();
        ~CommandTest();
};

#endif // __CMDS_COMMAND_TEST_HXX__
