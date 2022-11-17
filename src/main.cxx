//#include "cmd/CommandLineMgr.hxx"
#include "remote/MetaxRequests.hxx"
#include "io/Messaging.hxx"
#include "utils/Defines.hxx"
#include "utils/Typedefs.hxx"

#include <iostream>
#include <fstream>

Remote::MetaxRequests* pMetax = new Remote::MetaxRequests("localhost",8001);
IO::Messaging* pMsg = IO::Messaging::GetInstance();
std::ostream* pLogFile = nullptr;
Strings vecTempNodes;

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

std::string createNode()
{
    std::string sResponse;
    std::string sUUID = pMetax->CreateNode("New node");
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s\n", sUUID);
    pMsg->ShowMessage("CreatedNode: %s\n", sResponse);
    vecTempNodes.push_back(sUUID);
    return sUUID;
}

void updateNode( const std::string& sUUID )
{
    std::string sResponse;
    nlohmann::json oData;
    oData["value"] = "Updated Title";
    pMetax->UpdateNode(sUUID, "title", oData);
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s\n", sUUID);
    pMsg->ShowMessage("UpdatedNode: %s\n", sResponse);
}

void addChildNode( const std::string& sUUID )
{
    std::string sResponse;
    std::string sChildUUID = createNode();
    pMetax->AddChildNode(sUUID, sChildUUID, true);
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s | ChildUUID: %s\n", sUUID, sChildUUID);
    pMsg->ShowMessage("AddedChild: %s\n", sResponse);
    vecTempNodes.push_back(sChildUUID);

    std::string i;
    std::cout << "Enter to delete " << sChildUUID << " child...";
    std::getline(std::cin, i);
    pMetax->DeleteChildNode(sUUID, sChildUUID, true);
}

void addChildNodes( const std::string& sUUID, int iCount )
{
    std::string sResponse;
    Strings vecChildUUID;
    for ( int i = 0; i < iCount; ++i )
    {
        std::string sChildUUID = createNode();
        vecChildUUID.push_back(sChildUUID);
        vecTempNodes.push_back(sChildUUID);
    }
    pMetax->AddChildNode(sUUID, vecChildUUID);
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s | ChildCount: %s\n", sUUID,vecChildUUID.size());
    pMsg->ShowMessage("AddedChilds: %s\n", sResponse);
}

void connectChildNode( const std::string& sUUID )
{
    std::string sResponse;
    std::string sChildUUID = createNode();
    pMetax->ConnectChildNode(sUUID, sChildUUID);
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s | ChildUUID: %s\n", sUUID, sChildUUID);
    pMsg->ShowMessage("ConnectChild: %s\n", sResponse);
    vecTempNodes.push_back(sChildUUID);

    std::string i;
    std::cout << "Enter to disconnect " << sChildUUID << " child...";
    std::getline(std::cin, i);
    pMetax->DisconnectChildNode(sUUID, sChildUUID);
}

void connectChildNodes( const std::string& sUUID, int iCount )
{
    std::string sResponse;
    Strings vecChildUUID;
    for ( int i = 0; i < iCount; ++i )
    {
        std::string sChildUUID = createNode();
        vecChildUUID.push_back(sChildUUID);
        vecTempNodes.push_back(sChildUUID);
    }
    pMetax->ConnectChildNode(sUUID, vecChildUUID);
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s | ChildCount: %s\n", sUUID,vecChildUUID.size());
    pMsg->ShowMessage("AddedChilds: %s\n", sResponse);
}

void deleteNode( const std::string& sUUID )
{
    std::string sResponse;
    pMetax->DeleteNode(sUUID);
    pMetax->GetNode(sUUID, sResponse);
    pMsg->ShowMessage("UUID: %s\n", sUUID);
    pMsg->ShowMessage("DeleteNode: %s\n", sResponse);
}

void deleteNodes()
{
    for ( const auto& sUUID : vecTempNodes )
    {
        deleteNode(sUUID);
    }
}

int main(int argc, char** argv)
{
    initMessaging();
    std::string sUUID = createNode();
    updateNode(sUUID);
    addChildNode(sUUID);
    addChildNodes(sUUID, 5);
    connectChildNode(sUUID);
    connectChildNodes(sUUID, 5);

    std::string i;
    std::cout << "\n\n\nURL: http://localhost:8001/db/get?id=" << sUUID;
    std::cout << "\nEnter to exit...";
    std::getline(std::cin, i);

    deleteNodes();
    cleanMessaging();
    delete pMetax;
    return 0;
}
