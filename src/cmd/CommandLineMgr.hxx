#ifndef __CMD_COMMAND_LINE_MGR_HXX__
#define __CMD_COMMAND_LINE_MGR_HXX__

#include "cli/cli.h"
#include "cli/filehistorystorage.h"
#include "cli/clilocalsession.h"
#include "cli/loopscheduler.h"

namespace CMD {
    class CommandLineMgr; 
    class Menu;
}; // CMD

class CMD::Menu
{
    private:
        std::unique_ptr<cli::Menu> m_menu;
        cli::Cli* m_cli;
        cli::CliLocalTerminalSession* m_session;
        cli::LoopScheduler* m_scheduler;

    private:
        void cliLocalSessionExitHandler(std::ostream&);
        void defaultCommandHandler(std::ostream&, const std::string&);

    public:
        Menu();
};

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
