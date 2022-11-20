#include <iostream>
#include "cmd_line/TclEngine.hxx"

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

CMD::TclEngine::
TclEngine() :
    m_interpreter(nullptr)
{
    m_interpreter = Tcl_CreateInterp();
}
