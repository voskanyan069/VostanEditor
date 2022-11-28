#include <iostream>
#include "cmd_line/TclEngine.hxx"
#include "commands/CommandBase.hxx"
#include "utils/VostanException.hxx"

void CMD::TclEngine::
evalCommand(const std::string& cmd)
{
    // boost::replace_all(cmd, "\\", "/");
    assert(0 != m_interpreter);
    int eval_result = Tcl_Eval(m_interpreter, cmd.c_str());
    if (eval_result == 1) {
        if (std::strcmp(Tcl_GetStringResult(m_interpreter), "")) {
            std::cout << Tcl_GetStringResult(m_interpreter) << std::endl;
        }
    }

}

static void printHelp(commands::CommandBase* c)
{
    assert(0 != c);
    commands::CommandBase::PARAMETERS params = c->getParameters();
    std::cout << "Description: " << c->getDescription() << std::endl;
    std::cout << "Usage: " << c->getName() << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    for (size_t i = 0; i < params.size(); ++i) {
        std::cout << "    " << "\e[1m" << params[i].name << "\e[0m"
            << ": " << params[i].description << std::endl;
    }
}

static bool readParam(int n, Tcl_Obj* const o[],
        commands::CommandBase* c, int name_index, int value_index)
{
    assert(0 != c);
    assert(name_index < n);
    std::string name = Tcl_GetString(o[name_index]);
    if (! c->paramExists(name)) {
        throw vostan_exception("Unknown parameter " + name);
    }
    if ("-help" == name) {
        printHelp(c);
        return true;
    }
    if (value_index >= n) {
        throw vostan_exception("No value specified for parameter " + name);
    }
    std::string value = Tcl_GetString(o[value_index]);
    c->setParamValue(name, value);
    return false;
}

static int command_handler(ClientData d, Tcl_Interp* t, int n,
        Tcl_Obj* const o[]) throw()
{
    try {
        typedef commands::CommandBase CB;
        CB* c = static_cast<CB*>(d);
        assert(0 != c);
        c->resetParams();
        bool helpRequested = false;
        for (int i = 1; i < n && !helpRequested; i += 2) {
            helpRequested = readParam(n, o, c, i, i + 1);
        }
        if (!helpRequested) {
            try
            {
                std::string res = c->run();
                Tcl_SetObjResult(t, Tcl_NewStringObj(res.c_str(), -1));
            }
            catch ( const vostan_exception& ex )
            {
                std::cout << ex.what() << std::endl;
                return TCL_ERROR;
            }
        }
    } catch (std::exception& e) {
        Tcl_SetObjResult(t, Tcl_NewStringObj(e.what(), -1));
        return TCL_ERROR;
    }
    return TCL_OK;
}

void CMD::TclEngine::
initCommands()
{
    // commands::CommandIndex* cmd_index = new commands::CommandIndex();
    // const auto cmds = cmd_index->getCommands();
    commands::CommandIndex::load();
    const auto cmds = commands::CommandIndex::getCommands();
    for (auto cmd: cmds) {
        Tcl_CreateObjCommand(m_interpreter, cmd->getName().c_str(),
                command_handler, cmd, NULL);
    }
}

CMD::TclEngine::
TclEngine() :
    m_interpreter(nullptr)
{
    m_interpreter = Tcl_CreateInterp();
    initCommands();
}
