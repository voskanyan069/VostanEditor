//#include "cmd/CommandLineMgr.hxx"
#include "remote/MetaxRequests.hxx"
#include "io/Messaging.hxx"
#include "utils/Defines.hxx"

#include <iostream>
#include <fstream>

std::ostream* pLogFile = nullptr;

void initMessaging()
{
    IO::Messaging* pMsg = IO::Messaging::GetInstance();
    pLogFile = new std::ofstream("editor.log", std::ios_base::app);
    pMsg->RegisterStream(&std::cout);
    pMsg->RegisterStream(pLogFile);
}

void cleanMessaging()
{
    DELETE_PTR(pLogFile);
}

int main(int argc, char** argv)
{
    initMessaging();
    //CMD::Menu* m_menu = new CMD::Menu();
    Remote::MetaxRequests* pMetax = new Remote::MetaxRequests("localhost",8001);
    std::string sUUID = pMetax->CreateNode();
    pMetax->DeleteNode(sUUID);
    delete pMetax;
    cleanMessaging();
    return 0;
}
