#ifndef __CMD_COMMAND_LINE_MGR_HXX__
#define __CMD_COMMAND_LINE_MGR_HXX__

namespace CMD {
   class CommandLineMgr; 
}; // CMD

namespace Cli {
    class Menu;
}; // Cli

class CMD::CommandLineMgr
{
public:
    CommandLineMgr* GetInstance();
    ~CommandLineMgr();
    CommandLineMgr(CommandLineMgr const&) = delete;
    void operator=(CommandLineMgr const&) = delete;

private:
    CommandLineMgr();

public:
    void AddCommand();
};

#endif // __CMD_COMMAND_LINE_MGR_HXX__
