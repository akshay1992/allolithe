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
	try 
	{
		ModuleInfo& module = RegisteredModules.at(moduleID);
		NodeInfo node_info;

		node_info.nodeID = module.factory_function();
		node_info.moduleID = moduleID;
		
		InstantiatedNodes[node_info.nodeID] = node_info;
		std::cout << "Instantiated module: " << module.moduleName << " NodeID: " <<  node_info.nodeID << std::endl;

		return node_info;
	}
	catch(std::out_of_range e)
	{
		throw ModuleNotRegisteredException(moduleID);
	}
	catch(std::exception e)
	{
		throw std::runtime_error(e.what());
	}
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

	std::cout << "Deleting module: " << getModuleInfo(getNodeInfo(nodeID).moduleID).moduleName << " NodeID: " <<  nodeID << std::endl;
	delete &al::Module::getModuleRef(nodeID);
	InstantiatedNodes.erase(nodeID);
	return;
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
	try
	{
		ModuleInfo& info = RegisteredModules[sink_module_id];
		std::cout << "Setting Sink as: " << info.moduleName << std::endl;
		sink_ref = &al::SinkModule::getSinkModuleRef(sink_node_id);
	}
	catch(std::out_of_range e)
	{
		throw ModuleNotRegisteredException(sink_node_id);
		sink_ref = NULL;
	}
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

	RegisteredModules[info.moduleID] = info; 	// Add to std::map of modules
	std::cout << "Registered Module: " << info.moduleName << "  ID: " << info.moduleID << std::endl;
	return info;
}

ModuleInfo RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function, bool is_a_sink_module )
{
	return DefaultSoundEngine().RegisterModule(module_name, module_factory_function, is_a_sink_module);
}


void SoundEngine::patch(NodeInfo& destination_node, int inlet_index, NodeInfo& source_node, int outlet_index)
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
		al::Module::getModuleRef(destination_node.nodeID).getInlet(inlet_index),
		al::Module::getModuleRef(source_node.nodeID).getOutlet(outlet_index));
	}
	catch(std::runtime_error e)
	{
		throw PatchingException(PatchingExceptionType::LITHE_ERROR_PATCHING, e);
	}
	catch(std::range_error e)
	{
		throw PatchingException(PatchingExceptionType::LITHE_ERROR_PATCHING, e);
	}
	catch(NodeNotFoundException e)
	{
		throw PatchingException(PatchingExceptionType::LITHE_ERROR_PATCHING, e);
	}
}

void SoundEngine::unpatch(NodeInfo& destination_node, int inlet_index, NodeInfo& source_node, int outlet_index)
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
				al::Module::getModuleRef(destination_node.nodeID).getInlet(inlet_index), 
				al::Module::getModuleRef(source_node.nodeID).getOutlet(outlet_index));
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

std::vector<NodeInfo> SoundEngine::activeNodes(void)
{
	std::vector<NodeInfo> active_nodes;
	for(std::map<int, NodeInfo>::iterator it = InstantiatedNodes.begin(); it != InstantiatedNodes.end(); ++it) 
	{
		active_nodes.push_back(it->second);
	}
	return active_nodes;
}

std::vector<ModuleInfo> SoundEngine::getRegisteredModules(void)
{
	std::vector<ModuleInfo> available_modules;
	for(std::map<int, ModuleInfo>::iterator it = RegisteredModules.begin(); it != RegisteredModules.end(); ++it) 
	{
		available_modules.push_back(it->second);
	}
	return available_modules;
}

bool SoundEngine::isRunning(void)
{
	return getSink().isRunning(); 
}

int SoundEngine::is_instantiated(int nodeID)
{
	try 
	{
		InstantiatedNodes.at(nodeID);
	}
	catch( std::out_of_range e) 
	{
		throw NodeNotFoundException(nodeID);
	}
}

ModuleInfo& SoundEngine::getModuleInfo(int moduleID)
{
	return RegisteredModules.at(moduleID);
}

NodeInfo& SoundEngine::getNodeInfo(int nodeID)
{
	return InstantiatedNodes.at(nodeID);
}

}; //namespace al