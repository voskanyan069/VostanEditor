#ifndef __UTILS_VOSTANEXCEPTION_HXX__
#define __UTILS_VOSTANEXCEPTION_HXX__

// Standard headers
#include <exception>
#include <string>

class vostan_exception: public std::exception
{
    public: 
	virtual const char* what() const noexcept {
	    return m_message.c_str();
	}

	vostan_exception(const std::string& message = "")
	    : std::exception()
	    , m_message(message)
        { }

    protected:
	std::string m_message;
};

#endif // __UTILS_VOSTANEXCEPTION_HXX__
