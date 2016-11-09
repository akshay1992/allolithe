#include "allolithe/al_Module.hpp"

namespace al{

Module::Module(int numInlets, int numOutlets, int numParameters) :
	lithe::Node(numInlets, numOutlets),
	numParameters(numParameters)
{
}

al::Parameter& Module::parameter(int index)
{
	if( index < 0 || index >= numParams() )
	{
		throw std::range_error("Parameter index not valid");
	}
	else
	{
		return *parameters[index];
	}
}

Module::~Module(void)
{
	for(int i=0; i<parameters.size(); ++i)
	{
		if(parameters[i] != NULL) delete parameters[i];
	}
}

glv::View& Module::createDefaultView(bool debugging)
{
	// TODO:
	return *new glv::View(glv::Rect(80, 80));

}

Module& Module::getModuleRef(int nodeID)
{
	try
	{
		al::Module* module_ref = static_cast<al::Module*>(lithe::Node::getNodeRef(nodeID));
		if(module_ref == NULL)
			throw std::runtime_error("Module instance not found or instance was destroyed. NodeID: "+std::to_string(nodeID));
		else
			return *module_ref;
	}
	catch(const char * str)
	{
		throw std::runtime_error(str);
	}
}



}; // namespace al