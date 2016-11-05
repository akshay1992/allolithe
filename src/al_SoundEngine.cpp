#include "allolithe/al_SoundEngine.hpp"
#include <algorithm>

namespace al {

std::vector<int> SoundEngine::instantiatedNodeIDs;
std::vector<std::string> SoundEngine::ModuleNames;
std::vector<ModuleFactoryFunction> SoundEngine::ModuleConstructors;

int SoundEngine::instantiateModule(int moduleID)
{
	if( moduleID > ModuleNames.size()-1 ) 
		throw std::range_error("Module with id: " + std::to_string(moduleID) + " not registered");
	else
	{
		instantiatedNodeIDs.push_back(ModuleConstructors[moduleID]());
		return instantiatedNodeIDs.back();
	}
}

void SoundEngine::deleteModuleInstance(int nodeID)
{
	for(int i=0; i<instantiatedNodeIDs.size(); ++i)
	{
		if( instantiatedNodeIDs[i] == nodeID )
		{
			instantiatedNodeIDs.erase(instantiatedNodeIDs.begin()+i);
			delete &al::Module::getModuleRef(nodeID);
			return;
		}
	}
	throw std::runtime_error("Unable to find node with ID:"+std::to_string(nodeID));
}

void SoundEngine::onSound(al::AudioIOData& io)
{
	if( sink_ref != NULL)
	{
		sink_ref->onSound(io);
	}
}

void SoundEngine::setSink(al::SinkModule& sink)
{
	sink_ref = &sink;
}

int SoundEngine::RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function)
{
	SoundEngine::ModuleNames.push_back(module_name);
	SoundEngine::ModuleConstructors.push_back(module_factory_function);

	int module_id = SoundEngine::ModuleConstructors.size() - 1;
	std::cout << "Registered Module: " << module_name << "  ID: " << module_id << std::endl;
	return module_id;
}

}; //namespace al