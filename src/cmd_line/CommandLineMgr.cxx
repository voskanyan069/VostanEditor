#include "cmd_line/CommandLineMgr.hxx"
#include "commands/CommandIndex.hxx"
#include "commands/CommandBase.hxx"

#include "cli/cli.h"
#include "cli/clilocalsession.h"
#include "cli/filehistorystorage.h"

void CMD::Menu::
cliLocalSessionExitHandler(std::ostream& out)
{
    out << "VostanEditor session closed!\n";
    assert(0 != m_scheduler);
    m_scheduler->Stop();
}

void CMD::Menu::
defaultCommandHandler(std::ostream& out, const std::string& cmd)
{
    m_tcl_engine->evalCommand(cmd);
}

void CMD::Menu::
commandHandler(std::ostream& out, const std::string& cmd,
        std::vector<std::string> args)
{
    assert(0 != m_tcl_engine);
    std::string command = cmd + " ";
    for(auto a: args) {
        command += " " + a;
    }
    m_tcl_engine->evalCommand(command);
}

void CMD::Menu::
start()
{
    m_scheduler->Run();
}

void CMD::Menu::
initCommands()
{
    typedef std::vector<std::string> SV;
    assert(nullptr != m_menu);
    const auto cmds = commands::CommandIndex::getCommands();
    for (auto cmd: cmds) {
        assert(nullptr != cmd);
        SV param_names;
        const commands::CommandBase::PARAMETERS& params = cmd->getParameters();
        for(auto p: params) {
            param_names.push_back(p.name);
        }
        std::string cmd_name = cmd->getName();
        m_menu->Insert(cmd_name,
                param_names,
                [this, cmd_name](std::ostream& out, SV data) {
                    commandHandler(out, cmd_name, data);
                },
                cmd->getDescription(),
                param_names);
    }
}

CMD::Menu::
Menu(CMD::TclEngine* tcl_engine) :
    m_cli(nullptr),
    m_menu(nullptr),
    m_session(nullptr),
    m_scheduler(nullptr),
    m_tcl_engine(tcl_engine)
{
    cli::SetColor();
    m_menu = std::make_unique<cli::Menu>("VostanEditor");
    initCommands();
    m_cli = new cli::Cli( std::move(m_menu),
            std::make_unique<cli::FileHistoryStorage>(".cli"));
    m_scheduler = new cli::LoopScheduler();
    m_session = new cli::CliLocalTerminalSession(
            *m_cli, *m_scheduler, std::cout, 200);
    auto f = std::mem_fun(&Menu::cliLocalSessionExitHandler);
    m_session->ExitAction(std::bind(f, this, std::placeholders::_1));
    auto g = std::mem_fn(&Menu::defaultCommandHandler);
    m_cli->DefaultCommandHandler(std::bind(g, this,
                std::placeholders::_1, std::placeholders::_2));
}
