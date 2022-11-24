#include "commands/CommandBase.hxx"

bool commands::CommandBase::
paramExists(const std::string& name)
{
    return 0 != m_param_set.count(name);
}

void commands::CommandBase::
setParamValue(const std::string& name, const std::string& value)
{
    m_param_values[name] = value;
}

void commands::CommandBase::
resetParams()
{
    m_param_values.clear();
}

commands::CommandBase::PARAMETERS commands::CommandBase::
getParameters()
{
    return m_parameters;
}

void commands::CommandBase::
registerParam(const std::string& name, const std::string& description)
{
    Parameter p;
    p.name = name;
    p.description = description;
    m_parameters.push_back(p);
    m_param_set.insert(name);
}

bool commands::CommandBase::
isParamProvided(const std::string& name)
{
    return m_param_values.find(name) != m_param_values.end();
}

commands::CommandBase::
CommandBase()
    : m_parameters()
    , m_param_set()
    , m_param_values()
{
    registerParam("-help", "");
}

commands::CommandBase::
~CommandBase()
{
}
