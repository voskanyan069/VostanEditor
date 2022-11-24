#ifndef __CMDS_COMMAND_BASE_HXX__
#define __CMDS_COMMAND_BASE_HXX__

// Standard headers
#include <string>
#include <set>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

namespace commands {
    class CommandBase;
}


class commands::CommandBase
{
    public:
        struct Parameter {
            std::string name;
            std::string description;
        };
    public:
        typedef std::vector<Parameter> PARAMETERS;
        virtual std::string getName();
        virtual std::string getDescription();
        virtual int run();
        void setParamValue(const std::string&, const std::string&);
        void resetParams();
        PARAMETERS getParameters();
        bool paramExists(const std::string& name);

    protected:
        void registerParam(const std::string& name,
             const std::string& description);
        // template<typename T = std::string>
        // T getParamValue(const std::string& name);
        // template<typename T = std::string>
        // T getParamValue(const std::string& name, const T& default_value);
        bool isParamProvided(const std::string& name);
    
    public:
        void evalCommand(const std::string&);

    private:
        PARAMETERS m_parameters;
        std::set<std::string> m_param_set;
        std::map<std::string, std::string> m_param_values;

    public:
        CommandBase();
        ~CommandBase();
};

#endif // __CMDS_COMMAND_BASE_HXX__
