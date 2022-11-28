#ifndef __CMDS_CREATE_NODE_HXX__
#define __CMDS_CREATE_NODE_HXX__

// Headers from this project
#include "commands/CommandBase.hxx"

namespace commands {
    class CreateNode;
}

class commands::CreateNode : public commands::CommandBase
{
    public:
        virtual std::string getName();
        virtual std::string getDescription();
        virtual std::string run();

    public:
        CreateNode();
        ~CreateNode();
};

#endif // __CMDS_CREATE_NODE_HXX__
