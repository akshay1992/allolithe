#ifndef DUMMY_MODULE_HPP
#define DUMMY_MODULE_HPP

#include "allolithe/al_Module.hpp"
#include "allolithe/al_SoundEngine.hpp"

class DummyModule : public al::Module
{
public:
	DummyModule(void) : al::Module(1, 1, 1)
	{
		counter++;
		nodeID = counter;
	}

	~DummyModule(void)
	{
		counter--;
	}

	int getNodeID() {return nodeID;}
	int getModuleID() {return moduleID;}
	
	virtual void instantiate_parameters(void) {}

	static const std::string module_name;
	static const int moduleID;
	static int counter;
	int nodeID;
};

const std::string DummyModule::module_name = std::string("DummyModule");
const int DummyModule::moduleID = al::RegisterModule<DummyModule>(DummyModule::module_name);
int DummyModule::counter = -1;

#endif // DUMMY_MODULE_HPP