#ifndef __CMD_COMMAND_LINE_MGR_HXX__
#define __CMD_COMMAND_LINE_MGR_HXX__

#include "cmd_line/TclEngine.hxx"

#include "cli/cli.h"
#include "cli/filehistorystorage.h"
#include "cli/clilocalsession.h"
#include "cli/loopscheduler.h"

namespace CMD {
    class Menu;
    class TclEngine;
}; // CMD

class CMD::Menu
{
    private:
        std::unique_ptr<cli::Menu> m_menu;
        cli::Cli* m_cli;
        cli::CliLocalTerminalSession* m_session;
        cli::LoopScheduler* m_scheduler;
        CMD::TclEngine* m_tcl_engine;

    private:
        void cliLocalSessionExitHandler(std::ostream&);
        void defaultCommandHandler(std::ostream&, const std::string&);
        void commandHandler(std::ostream& out, const std::string&,
                std::vector<std::string>);
        void initCommands();

    public:
        void start();

    public:
        Menu(CMD::TclEngine*);
};

#endif // __CMD_COMMAND_LINE_MGR_HXX__
