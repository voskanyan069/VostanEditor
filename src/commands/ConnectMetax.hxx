#ifndef __CMDS_CONNECT_HXX__
#define __CMDS_CONNECT_HXX__

// Headers from this project
#include "commands/CommandBase.hxx"

namespace commands {
    class ConnectMetax;
}

class commands::ConnectMetax : public commands::CommandBase
{
    public:
        virtual std::string getName();
        virtual std::string getDescription();
        virtual std::string run();

    public:
        ConnectMetax();
        ~ConnectMetax();
};

#endif // __CMDS_CONNECT_HXX__
