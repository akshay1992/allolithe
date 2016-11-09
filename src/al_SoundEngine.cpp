#include "allolithe/al_SoundEngine.hpp"
#include <algorithm>

namespace al {

al::SoundEngine& DefaultSoundEngine(void)
{
	static SoundEngine* default_instance = new al::SoundEngine;
	return *default_instance;
}

int SoundEngine::instantiateModule(int moduleID)
{
	if( moduleID > ModuleNames.size()-1 || moduleID < 0 ) 
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
	getSink().onSound(io);
}

void SoundEngine::setSink(int sink_module_id)
{
	// sink_ref = &sink;
	int nodeID = instantiateModule(sink_module_id);
	sink_ref = &al::SinkModule::getSinkModuleRef(nodeID);
}

al::SinkModule& SoundEngine::getSink(void) 
{
	if( sink_ref == NULL)
	{
		throw std::runtime_error("Sink not set");
	}
	else
	{
		return *sink_ref; 			
	}
}

int SoundEngine::RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function)
{
	if( module_name == "" )
		std::runtime_error("Can't register module. No name provided");

	ModuleNames.push_back(module_name);
	ModuleConstructors.push_back(module_factory_function);

	int module_id = ModuleConstructors.size() - 1;
	std::cout << "Registered Module: " << module_name << "  ID: " << module_id << std::endl;

	return module_id;
}

int RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function)
{
	return DefaultSoundEngine().RegisterModule(module_name, module_factory_function);
}

bool SoundEngine::patch(int destination_nodeID, int inlet_index, int source_nodeID, int outlet_index)
{
	if(getSink().isRunning() )
	{
		throw std::runtime_error("Cannot connect patch when sink is running");
	}
	else
	{
		try
		{
			lithe::Patcher::connect(
				al::Module::getModuleRef(destination_nodeID).getInlet(inlet_index), 
				al::Module::getModuleRef(source_nodeID).getOutlet(outlet_index));
			return true;
		}
		catch(...)
		{
			return false;				
		}
	}
}

bool SoundEngine::unpatch(int destination_nodeID, int inlet_index, int source_nodeID, int outlet_index)
{
	if(getSink().isRunning() )
	{
		throw std::runtime_error("Cannot connect patch when sink is running");
	}
	else
	{
		try
		{
			lithe::Patcher::disconnect(
				al::Module::getModuleRef(destination_nodeID).getInlet(inlet_index), 
				al::Module::getModuleRef(source_nodeID).getOutlet(outlet_index));
			return true;
		}
		catch(...)
		{
			return false;				
		}
	}
}

bool SoundEngine::unpatch_from_inlet(int nodeID, int inlet_index)
{
	if(getSink().isRunning() )
	{
		throw std::runtime_error("Cannot connect patch when sink is running");
	}
	else
	{
		try
		{
			al::Module::getModuleRef(nodeID).getInlet(inlet_index).disconnect();
		}
		catch (...)
		{
			return false;
		}
	}
}


}; //namespace al