#include "commands/CreateNode.hxx"

#include "remote/MetaxRequests.hxx"

#include "utils/VostanException.hxx"

commands::CreateNode::
CreateNode()
: CommandBase()
{
    registerParam("-title", "Title for new node");
}

std::string commands::CreateNode::
getName()
{
    return "create_node";
}

std::string commands::CreateNode::
getDescription()
{
    return "Command To create new node.";
}

std::string commands::CreateNode::
run()
{
    if (nullptr == m_pMetax) {
        throw vostan_exception("Please connect to metax server.");
    }
    std::string title = getParamValue<std::string>("-title");
    m_pMetax->CreateNode(title);
    return "";
}

commands::CreateNode::
~CreateNode()
{
}
