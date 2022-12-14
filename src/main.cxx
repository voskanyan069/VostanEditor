#include "cmd_line/CommandLineMgr.hxx"
#include "cmd_line/TclEngine.hxx"
#include "remote/MetaxRequests.hxx"
#include "io/Messaging.hxx"
#include "utils/Defines.hxx"
#include "utils/Typedefs.hxx"

#include <iostream>
#include <fstream>

#include <QWidget>
#include <QApplication>

Remote::MetaxRequests* pMetax = nullptr;
IO::Messaging* pMsg = IO::Messaging::GetInstance();
std::ostream* pLogFile = nullptr;
Strings vecTempNodes;

void initMessaging()
{
    pLogFile = new std::ofstream("editor.log", std::ios_base::app);
    pMsg->RegisterStream(&std::cout);
    pMsg->RegisterStream(pLogFile);
}

void cleanMessaging()
{
    DELETE_PTR( pLogFile );
}

std::string createNode()
{
    std::string sResponse;
    std::string sUUID = pMetax->CreateNode("New node");
    if ( !sUUID.empty() && pMetax->GetNode(sUUID, sResponse) )
    {
        pMsg->ShowMessage("UUID: %s\n", sUUID);
        pMsg->ShowMessage("CreatedNode: %s\n", sResponse);
        vecTempNodes.push_back(sUUID);
    }
    else
    {
        std::exit(-1);
    }
    return sUUID;
}

void updateNode( const std::string& sUUID )
{
    std::string sResponse;
    nlohmann::json oData;
    if ( pMetax->UpdateTitle(sUUID, "Updated Title", {1, 20, 40, 60, 80}) &&
            pMetax->UpdateTitle(sUUID, "UT 2", {1, 20, -2, 60, -1}) &&
            pMetax->UpdateText(sUUID, "Updated Text", {1, 20, -2, 20, -1}) &&
            pMetax->SetTitleVisibility(sUUID, false) &&
            pMetax->SetImageVisibility(sUUID, true) &&
            pMetax->SetImageDimensions(sUUID, {0, 10, 10, 10, 10}) &&
            pMetax->SetTextDimensions(sUUID, {1, 60, -2, -1, -1}) &&
            pMetax->SetTags(sUUID, {"tag1"}) &&
            pMetax->SetTags(sUUID, {"tag2", "tag3"}) &&
            pMetax->AddTags(sUUID, {"tag4", "tag5"}) &&
            pMetax->SetDimensions(sUUID, {1, 2, -1, 5, 6}) &&
            pMetax->GetNode(sUUID, sResponse) )
    {
        pMsg->ShowMessage("UUID: %s\n", sUUID);
        pMsg->ShowMessage("UpdatedNode: %s\n", sResponse);
    }
    else
    {
        std::exit(-1);
    }
}

void addChildNode( const std::string& sUUID )
{
    std::string sResponse;
    std::string sChildUUID = createNode();
    if ( pMetax->AddChildNode(sUUID, sChildUUID, true) &&
            pMetax->GetNode(sUUID, sResponse) )
    {
        pMsg->ShowMessage("UUID: %s | ChildUUID: %s\n", sUUID, sChildUUID);
        pMsg->ShowMessage("AddedChild: %s\n", sResponse);
        vecTempNodes.push_back(sChildUUID);
    }
    else
    {
        std::exit(-1);
    }
}

void addChildNodes( const std::string& sUUID, int iCount )
{
    std::string sResponse;
    Strings vecChildUUID;
    for ( int i = 0; i < iCount; ++i )
    {
        std::string sChildUUID = createNode();
        if ( sChildUUID.empty() )
        {
            std::exit(-1);
        }
        vecChildUUID.push_back(sChildUUID);
        vecTempNodes.push_back(sChildUUID);
    }
    if ( pMetax->AddChildNode(sUUID, vecChildUUID) &&
            pMetax->GetNode(sUUID, sResponse) )
    {
        pMsg->ShowMessage("UUID: %s | ChildCount: %s\n",
                sUUID, vecChildUUID.size());
        pMsg->ShowMessage("AddedChilds: %s\n", sResponse);
    }
    else
    {
        std::exit(-1);
    }
}

void connectChildNode( const std::string& sUUID )
{
    std::string sResponse;
    std::string sChildUUID = createNode();
    if ( !sChildUUID.empty() && pMetax->ConnectChildNode(sUUID, sChildUUID) &&
            pMetax->GetNode(sUUID, sResponse) )
    {
        pMsg->ShowMessage("UUID: %s | ChildUUID: %s\n", sUUID, sChildUUID);
        pMsg->ShowMessage("ConnectChild: %s\n", sResponse);
        vecTempNodes.push_back(sChildUUID);
    }
    else
    {
        std::exit(-1);
    }
}

void connectChildNodes( const std::string& sUUID, int iCount )
{
    std::string sResponse;
    Strings vecChildUUID;
    for ( int i = 0; i < iCount; ++i )
    {
        std::string sChildUUID = createNode();
        if ( sChildUUID.empty() )
        {
            std::exit(-1);
        }
        vecChildUUID.push_back(sChildUUID);
        vecTempNodes.push_back(sChildUUID);
    }
    if ( pMetax->ConnectChildNode(sUUID, vecChildUUID) &&
            pMetax->GetNode(sUUID, sResponse) )
    {
        pMsg->ShowMessage("UUID: %s | ChildCount: %s\n",
                sUUID, vecChildUUID.size());
        pMsg->ShowMessage("AddedChilds: %s\n", sResponse);
    }
    else
    {
        std::exit(-1);
    }
}

void updateChildNode( const std::string& sUUID )
{
    std::string sChildUUID = createNode();
    vecTempNodes.push_back(sChildUUID);
    pMetax->AddChildNode(sUUID, sChildUUID);
    pMetax->SetChildLeafCount(sUUID, sChildUUID, 5);
    pMetax->SetChildDimensions(sUUID, sChildUUID, {1, 1, 1, 1, 1});
    pMetax->SetChildDimensions(sUUID, sChildUUID, {1, 1, 1, 1, 1});
    pMetax->SetChildTitleDimensions(sUUID, sChildUUID, {1, 10, 10, 10, 10});
    pMetax->SetChildTextDimensions(sUUID, sChildUUID, {1, 20, 20, 20, 20});
    pMetax->SetChildImageDimensions(sUUID, sChildUUID, {0, 30, 30, 30, 30});
    pMetax->SetChildTitleVisibility(sUUID, sChildUUID, 0);
    pMetax->SetChildTextVisibility(sUUID, sChildUUID, 1);
    pMetax->SetChildImageVisibility(sUUID, sChildUUID, 1);
}

void deleteNode( const std::string& sUUID )
{
    std::string sResponse;
    if ( pMetax->DeleteNode(sUUID) &&
            pMetax->GetNode(sUUID, sResponse) )
    {
        pMsg->ShowMessage("UUID: %s\n", sUUID);
        std::size_t iIdx = sResponse.find("Getting file failed");
        if ( std::string::npos == iIdx )
        {
            pMsg->ShowMessage("DeleteNode: %s\n", sResponse);
        }
        else
        {
            pMsg->ShowMessage("DeleteNode: %s successfully deleted\n", sUUID);
        }
    }
    else
    {
        std::exit(-1);
    }
}

void deleteNodes()
{
    for ( const auto& sUUID : vecTempNodes )
    {
        deleteNode(sUUID);
    }
}

void start_cli(CMD::TclEngine* tcl_engine)
{
    CMD::Menu* cmd_mgr = new CMD::Menu(tcl_engine);
    cmd_mgr->start();
}

int testRequests(int argc, char** argv)
{
    pMetax = new Remote::MetaxRequests("localhost", 8001);
    std::string sUUID = createNode();
    updateNode(sUUID);
    addChildNode(sUUID);
    addChildNodes(sUUID, 5);
    updateChildNode(sUUID);
    connectChildNode(sUUID);
    connectChildNodes(sUUID, 5);
    std::string i;
    std::cout << "\n\n\nURL: http://localhost:8001/db/get?id=" << sUUID;
    std::cout << "\nEnter to exit...";
    std::getline(std::cin, i);
    deleteNodes();
    return 0;
}

int testCli(int argc, char** argv)
{
    initMessaging();
    for (int ai = 1; ai < argc && strcmp(argv[ai], "--"); ai++)
    {
        if (argv[ai][0] == '-') {
            if (strcmp("-cli", argv[ai]) == 0) {
                CMD::TclEngine* tcl_engine = new CMD::TclEngine(); 
                start_cli(tcl_engine);
            } else {
                std::cout << "\nWrong paramters: "
                    << argv[ai] << std::endl;
            }
        }
    }
    return 0;
}

int testGui(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget window;
    window.resize(250, 150);
    window.setWindowTitle("Simple example");
    window.show();
    return app.exec();
}

int main(int argc, char** argv)
{
    if ( argc < 2 )
    {
        std::cout << "Error: expected 1 argument provided 0" << std::endl;
        return 1;
    }
    initMessaging();
    int rc = 0;
    if ( strcmp("-test", argv[1]) == 0 )
    {
        rc = testRequests(argc, argv);
    }
    else if ( strcmp("-cli", argv[1]) == 0 )
    {
        rc = testCli(argc, argv);
    }
    else if ( strcmp("-gui", argv[1]) == 0 )
    {
        rc = testGui(argc, argv);
    }
    cleanMessaging();
    return rc;
}
