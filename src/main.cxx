#include "cmd/CommandLineMgr.cxx"
#include "remote/MetaxRequests.cxx"

int main(int argc, char** argv)
{
    //CMD::Menu* m_menu = new CMD::Menu();
    Remote::MetaxRequests* pMetax = new Remote::MetaxRequests("localhost",8001);
    pMetax->CreateNode();
    return 0;
}
