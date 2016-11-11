#include "allolithe/al_SoundEngine.hpp"
#include <algorithm>
#include "allolithe/al_Exceptions.hpp"

namespace al {

//-------------------------------------------------------------------------------------

al::SoundEngine& DefaultSoundEngine(void)
{
	static SoundEngine* default_instance = new al::SoundEngine;
	return *default_instance;
}

int SoundEngine::instantiateModule(int moduleID)
{
	if( moduleID > ModuleNames.size()-1 || moduleID < 0 ) 
		throw ModuleNotRegisteredException(moduleID);
	else
	{		
		instantiatedNodeIDs.push_back(ModuleConstructors[moduleID]());
		std::cout << "Instantiated module " << ModuleNames[moduleID] << " with ID " <<  instantiatedNodeIDs.back() << std::endl;
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

int SoundEngine::setAndInstantiateSink(int sink_module_id)
{
	int nodeID = instantiateModule(sink_module_id);
	setSink(nodeID, sink_module_id);
	return nodeID;
}

void SoundEngine::setSink(int sink_node_id, int sink_module_id)
{
	sink_ref = &al::SinkModule::getSinkModuleRef(sink_node_id);
	std::cout << "Setting Sink as: " << ModuleNames[sink_module_id] << std::endl;
}

al::SinkModule& SoundEngine::getSink(void) 
{
	if( sink_ref == NULL)
	{
		throw SinkNotSetException();
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


void SoundEngine::patch(int destination_nodeID, int inlet_index, int source_nodeID, int outlet_index)
{
	bool sink_is_running;
	try
	{
		sink_is_running = getSink().isRunning();
	}
	catch(SinkNotSetException)
	{
		sink_is_running = false; // Sink isn't running (since it isn't set)
	}

	if( sink_is_running )
	{
		throw PatchingException(PatchingExceptionType::SINK_IS_RUNNING);
	}
	else
	{
		try
		{
			lithe::Patcher::connect(
			al::Module::getModuleRef(destination_nodeID).getInlet(inlet_index), 
			al::Module::getModuleRef(source_nodeID).getOutlet(outlet_index));
		}
		catch(std::runtime_error e)
		{
			throw PatchingException(PatchingExceptionType::LITHE_ERROR_PATCHING, e);
		}
		catch(std::range_error e)
		{
			throw PatchingException(PatchingExceptionType::LITHE_ERROR_PATCHING, e);
		}
	}

}

void SoundEngine::unpatch(int destination_nodeID, int inlet_index, int source_nodeID, int outlet_index)
{
	bool sink_is_running;
	try
	{
		sink_is_running = getSink().isRunning();
	}
	catch(SinkNotSetException)
	{
		sink_is_running = false; // Sink isn't running (since it isn't set)
	}

	if( sink_is_running )
	{
		throw PatchingException(PatchingExceptionType::SINK_IS_RUNNING);
	}
	else
	{
		try
		{
			lithe::Patcher::disconnect(
				al::Module::getModuleRef(destination_nodeID).getInlet(inlet_index), 
				al::Module::getModuleRef(source_nodeID).getOutlet(outlet_index));
		}
		catch(std::runtime_error e)
		{
			throw PatchingException(PatchingExceptionType::LITHE_ERROR_UNPATCHING, e);
		}
		catch(std::range_error e)
		{
			throw PatchingException(PatchingExceptionType::LITHE_ERROR_UNPATCHING, e);
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

bool SoundEngine::isRunning(void)
{
	return getSink().isRunning(); 
}


}; //namespace al