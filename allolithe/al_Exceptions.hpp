#ifndef ALLOLITHE_EXCEPTIONS_HPP
#define ALLOLITHE_EXCEPTIONS_HPP

#include <exception>
#include <stdexcept>
#include <sstream>

namespace al{

class ParametersNotInstantiatedException : public std::range_error
{
public:
	ParametersNotInstantiatedException(int id) : std::range_error("Parameters not instantiated"), nodeID(id) {}
	virtual const char* what() const throw()
  	{
	    message.str( "" );
	    message << std::range_error::what() << "   NodeID: " << std::to_string(nodeID);
	    return message.str().c_str();
	}

private:
	int nodeID;
    static std::ostringstream message;
};


class NodeNotFoundException : public std::range_error
{
public:
	NodeNotFoundException(int id) : std::range_error("Node not found"), nodeID(id) {}
	virtual const char* what() const throw()
  	{
	    message.str( "" );
	    message << std::range_error::what() << "   ID: " << std::to_string(nodeID);
	    return message.str().c_str();
	}

private:
	int nodeID;
    static std::ostringstream message;
};

class SinkNotSetException : std::runtime_error
{
public:
	SinkNotSetException(void) : std::runtime_error( "Sink not set" ) {	}
};

class ModuleNotRegisteredException : std::range_error
{
public:
	ModuleNotRegisteredException(int id) : std::range_error("Module not registered"), moduleID(id) {}
	virtual const char* what() const throw()
  	{
	    message.str( "" );
	    message << std::range_error::what() << "   ID: " << std::to_string(moduleID);
	    return message.str().c_str();
	}

private:
	int moduleID;
    static std::ostringstream message;
};

enum class PatchingExceptionType 
{
	SINK_IS_RUNNING,
	LITHE_ERROR_PATCHING, 
	LITHE_ERROR_UNPATCHING
};

class PatchingException : std::runtime_error
{
public:
	PatchingException(PatchingExceptionType exception_type, std::runtime_error& details) 
		: std::runtime_error("Patching Exception "), 
		  type(exception_type)
	  	{
			additional_message = details.what();
		}

	PatchingException(PatchingExceptionType exception_type, std::range_error& details) 
	: std::runtime_error("Patching Exception "), 
	  type(exception_type)
  	{
		additional_message = details.what();
	}

	PatchingException(PatchingExceptionType exception_type) 
		: std::runtime_error("Patching Exception "), 
		  type(exception_type) 
		{	}

	virtual const char* what() const throw()
  	{
	    message.str( "" );
	    message << std::runtime_error::what() << " :  " ;
	    switch (type)
	    {
	    	case PatchingExceptionType::SINK_IS_RUNNING: message << " Cannot connect patch when sink is running"; break;
	    	case PatchingExceptionType::LITHE_ERROR_PATCHING: message << "Patching Error: " << additional_message ; break;
	    	case PatchingExceptionType::LITHE_ERROR_UNPATCHING: message << "Unpatching Error: " << additional_message ; break;
	    }
	    return message.str().c_str();
	}
private:
	PatchingExceptionType type;
    static std::ostringstream message;
    std::string additional_message;
};


class CannotDeleteActiveSinkException : std::runtime_error
{
public:
	CannotDeleteActiveSinkException(void) : 
		std::runtime_error( "Cannot delete Sink. It is being used by the SoundEngine" ) 
	{	}
};

}; // namespace al


#endif  //ALLOLITHE_EXCEPTIONS_HPP