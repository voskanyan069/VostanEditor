#ifndef __CMDS_COMMAND_TEST_HXX__
#define __CMDS_COMMAND_TEST_HXX__

// Standard headers
#include <string>

// Headers from this project
#include "commands/CommandBase.hxx"

namespace commands {
    class CommandTest;
}

class commands::CommandTest : public commands::CommandBase
{
    public:
        // virtual std::string getName();
        // virtual std::string getDescription();
        // virtual int run();

    public:
        CommandTest();
        ~CommandTest();
};

#endif // __CMDS_COMMAND_TEST_HXX__
