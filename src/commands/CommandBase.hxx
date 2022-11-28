#ifndef __CMDS_COMMAND_BASE_HXX__
#define __CMDS_COMMAND_BASE_HXX__

// Standard headers
#include <string>
#include <set>
#include <map>
#include <vector>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/type_index.hpp>

#include "utils/VostanException.hxx"
#include "remote/MetaxRequests.hxx"

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
        virtual std::string getName() = 0;
        virtual std::string getDescription() = 0;
        virtual std::string run() = 0;
        void setParamValue(const std::string&, const std::string&);
        void resetParams();
        PARAMETERS getParameters();
        bool paramExists(const std::string& name);

    protected:
        void registerParam(const std::string& name,
             const std::string& description);
        template<typename T = std::string>
        T getParamValue(const std::string& name) {
                auto c = m_param_values.find(name);
                if (c == m_param_values.end()) {
                    throw vostan_exception("Parameter '" + name + "' is not specified");
                    return T();
                }
                try {
                    return boost::lexical_cast<T>(c->second);
                } catch (boost::bad_lexical_cast& e) {
                    throw vostan_exception(
                            "Wrong value is specified for parameter '" + name
                            + "', it should be of type '"
                            + boost::typeindex::type_id<T>().pretty_name() + "'");
                    return T();
                }

        }
        template<typename T = std::string>
        T getParamValue(const std::string& name, const T& default_value) {
            auto c = m_param_values.find(name);
            if (c == m_param_values.end()) {
                return default_value;
            }
            try {
                return boost::lexical_cast<T>(c->second);
            } catch (boost::bad_lexical_cast& e) {
                throw vostan_exception(
                        "Wrong value is specified for parameter '" + name
                        + "', it should be of type '"
                        + boost::typeindex::type_id<T>().pretty_name() + "'");
                return T();
            }

        }
        bool isParamProvided(const std::string& name);
    
    public:
        void evalCommand(const std::string&);

    protected:
        static Remote::MetaxRequests* m_pMetax;

    private:
        PARAMETERS m_parameters;
        std::set<std::string> m_param_set;
        std::map<std::string, std::string> m_param_values;

    protected:
        CommandBase();
        ~CommandBase();
};

#endif // __CMDS_COMMAND_BASE_HXX__
