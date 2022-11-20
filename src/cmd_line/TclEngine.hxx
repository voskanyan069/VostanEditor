#ifndef __CMD_TCL_ENGINE_HXX__
#define __CMD_TCL_ENGINE_HXX__

// Standard headers
#include <string>
#include <tcl.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

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
    
    public:
        void evalCommand(const std::string&);
};

#endif // __CMD_TCL_ENGINE_HXX__
