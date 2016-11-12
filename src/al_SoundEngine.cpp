#include "allolithe/al_SoundEngine.hpp"
#include <algorithm>
#include "allolithe/al_Exceptions.hpp"

#include <iostream>

namespace al {

//-------------------------------------------------------------------------------------

al::SoundEngine& DefaultSoundEngine(void)
{
	static SoundEngine* default_instance = new al::SoundEngine;
	return *default_instance;
}

NodeInfo SoundEngine::instantiateModule(int moduleID)
{
	int info_index = get_module_info_index(moduleID);

	if( info_index != -1 )
	{
		NodeInfo node_info;
		node_info.nodeID = RegisteredModules[info_index].factory_function();
		node_info.moduleID = moduleID;
		InstantiatedNodes.push_back(node_info);
		std::cout << 
		"Instantiated module " << RegisteredModules[info_index].moduleName << 
		" with ID " <<  node_info.nodeID << std::endl;
		return node_info;
	}
	else
	{
		throw ModuleNotRegisteredException(moduleID);
	}

	// if( moduleID > ModuleNames.size()-1 || moduleID < 0 ) 
	// else
	// {		
	// 	instantiatedNodeIDs.push_back(ModuleConstructors[moduleID]());
	// 	std::cout << "Instantiated module " << ModuleNames[moduleID] << " with ID " <<  instantiatedNodeIDs.back() << std::endl;
	// 	return instantiatedNodeIDs.back();
	// }
}

void SoundEngine::deleteModuleInstance(int nodeID)
{
	int index = is_instantiated(nodeID);

	try
	{
		if( getSink().getNodeID() == nodeID)
		{
			throw CannotDeleteActiveSinkException();
		}			
	}
	catch(SinkNotSetException)
	{ 
		/* Not deleting an active sink, it's all goooood */	
	}

	delete &al::Module::getModuleRef(nodeID);
	InstantiatedNodes.erase(InstantiatedNodes.begin()+index);
	return;

	// for(int i=0; i<InstantiatedNodes.size(); ++i)
	// {
	// 	if( InstantiatedNodes[i].nodeID == nodeID )
	// 	{
	// 		if( getSink().getNodeID() == nodeID)
	// 		{
	// 			throw CannotDeleteActiveSinkException();
	// 		}
	// 		else
	// 		{
	// 			InstantiatedNodes.erase(InstantiatedNodes.begin()+i);
	// 			delete &al::Module::getModuleRef(nodeID);
	// 			return;
	// 		}
	// 		// instantiatedNodeIDs.erase(instantiatedNodeIDs.begin()+i);
	// 		// delete &al::Module::getModuleRef(nodeID);
	// 		// return;
	// 	}
	// }
	// throw std::runtime_error("Unable to find node with ID:"+std::to_string(nodeID));
}

void SoundEngine::onSound(al::AudioIOData& io)
{
	getSink().onSound(io);
}

int SoundEngine::setAndInstantiateSink(int sink_module_id)
{
	NodeInfo info = instantiateModule(sink_module_id);
	setSink(info.nodeID, sink_module_id);
	return info.nodeID;
}

void SoundEngine::setSink(int sink_node_id, int sink_module_id)
{
	sink_ref = &al::SinkModule::getSinkModuleRef(sink_node_id);

	int info_index = get_module_info_index(sink_module_id);
	std::cout << "Setting Sink as: " << RegisteredModules[info_index].moduleName << std::endl;
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

ModuleInfo SoundEngine::RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function, bool is_a_sink_module)
{
	if( module_name == "" )
		std::runtime_error("Can't register module. No name provided");

	ModuleInfo info;
	info.moduleName = module_name;
	info.moduleID = RegisteredModules.size() + MODULE_ID_OFFSET;
	info.isASink = is_a_sink_module; 
	info.factory_function = module_factory_function;

	RegisteredModules.push_back(info);

	ModuleIndexPair p;
	p.moduleID = info.moduleID;
	p.info_index = RegisteredModules.size() - 1;

	ModuleIndexHash.push_back(p);

	// ModuleNames.push_back(module_name);
	// ModuleConstructors.push_back(module_factory_function);

	// int module_id = ModuleConstructors.size() - 1;
	// std::cout << "Registered Module: " << module_name << "  ID: " << module_id << std::endl;

	return info;
}

ModuleInfo RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function, bool is_a_sink_module )
{
	return DefaultSoundEngine().RegisterModule(module_name, module_factory_function, is_a_sink_module);
}


void SoundEngine::patch(int destination_nodeID, int inlet_index, int source_nodeID, int outlet_index)
{
	bool sink_is_running;
	try
	{
		if ( getSink().isRunning() )
			throw PatchingException(PatchingExceptionType::SINK_IS_RUNNING);
	}
	catch(SinkNotSetException)
	{
		// Sink isn't running (since it isn't set). It's all gooooood
	}

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

int SoundEngine::get_module_info_index(int moduleID) 
{
	int info_index = -1;

	for(int i=0; i<ModuleIndexHash.size(); ++i)
	{
		if( ModuleIndexHash[i].moduleID == moduleID)
		{
			info_index = ModuleIndexHash[i].info_index;
		}
	}

	if( info_index == -1 )
	{
		throw ModuleNotRegisteredException(moduleID);
	}
	else
	{
		return info_index;
	}
}

int SoundEngine::is_instantiated(int nodeID)
{
	for(int i=0; i<InstantiatedNodes.size(); ++i )
	{
		if( InstantiatedNodes[i].nodeID == nodeID)
		{
			return i;
		}
	}
	
	throw NodeNotFoundException(nodeID);
}

ModuleInfo& SoundEngine::getModuleInfo(int moduleID)
{
	int index = get_module_info_index(moduleID);
	return RegisteredModules[index];
}

}; //namespace al