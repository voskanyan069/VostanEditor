#include "commands/CommandTest.hxx"


// commands::CommandTest::
// CommandTest()
// : CommandBase()
// {
//     registerParam("-CommandTest", "ufhuehf");
// }
// 
// commands::CommandTest::
// ~CommandTest()
// {
// }

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

int commands::CommandTest::
run()
{
    std::cout << "TESTTTTT" << std::endl;
    return 0;
}
