#include "commands/CommandTest.hxx"

#include "utils/VostanException.hxx"

commands::CommandTest::
CommandTest()
: CommandBase()
{
    registerParam("-CommandTest", "ufhuehf");
}

std::string commands::CommandTest::
getName()
{
    return "CommandTest";
}

std::string commands::CommandTest::
getDescription()
{
    return "CommandTest description";
}

std::string commands::CommandTest::
run()
{
    if (isParamProvided("-CommandTest")) {
        std::cout << getParamValue<std::string>("-CommandTest") << std::endl;
        return "";
        // throw vostan_exception("fiehji");
    }
    std::cout << "TESTTTTT" << std::endl;
    std::cout << "TESTTTTT" << std::endl;
    std::cout << "TESTTTTT" << std::endl;
    std::cout << "TESTTTTT" << std::endl;
    std::cout << "TESTTTTT" << std::endl;
    std::cout << "TESTTTTT" << std::endl;
    std::cout << "TESTTTTT" << std::endl;
    return "";
}

commands::CommandTest::
~CommandTest()
{
}
