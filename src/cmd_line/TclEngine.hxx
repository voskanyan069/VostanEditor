#ifndef __CMD_TCL_ENGINE_HXX__
#define __CMD_TCL_ENGINE_HXX__

// Standard headers
#include <string>
#include <tcl.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

// Headers from other projects
#include "commands/CommandIndex.hxx"

namespace CMD {
    // class Menu;
    class TclEngine;
}


class CMD::TclEngine
{
    private:
        Tcl_Interp* m_interpreter;
    
    public:
        TclEngine();
    
    // private:
        void initCommands();

    public:
        void evalCommand(const std::string&);
};

#endif // __CMD_TCL_ENGINE_HXX__
