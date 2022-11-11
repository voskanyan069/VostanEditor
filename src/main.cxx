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
    Remote::MetaxRequests* pMetax = new Remote::MetaxRequests("localhost",8001);
    IO::Messaging* pMsg = IO::Messaging::GetInstance();
    initMessaging();
    /// Create node
    std::string sResponse;
    std::string sUUID = pMetax->CreateNode("New node");
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s\n", sUUID);
    pMsg->ShowMessage("CreatedNode: %s\n", sResponse);
    /// Update node
    nlohmann::json oData;
    oData["value"] = "changed";
    pMetax->UpdateNode(sUUID, oData);
    sResponse = "";
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s\n", sUUID);
    pMsg->ShowMessage("UpdatedNode: %s\n", sResponse);
    /// Delete node
    pMetax->DeleteNode(sUUID);
    sResponse = "";
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s\n", sUUID);
    pMsg->ShowMessage("DeleteNode: %s\n", sResponse);
    delete pMetax;
    cleanMessaging();
    return 0;
}
