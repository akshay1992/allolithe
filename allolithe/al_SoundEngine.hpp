#ifndef AL_SOUNDENGINE_HPP
#define AL_SOUNDENGINE_HPP

#include <vector>
#include <string>
#include <iostream>
#include "allocore/io/al_AudioIOData.hpp"
#include "allolithe/al_Module.hpp"
#include "allolithe/al_SinkModule.hpp"

namespace al{

/// @brief Function pointer to module instantiation functions.
typedef int (*ModuleFactoryFunction)(void);

/** @brief A factory function that instnatiates modules. 

	Used when nothing special needs to be done to instantiate modules.
*/
template<typename T> 
int default_module_factory(void)
{
	T* new_instance = new T;
	return new_instance->getNodeID();
}

/** @brief SoundEngine connects the audiograph to the IO, and handles node management.

	SoundEngine can only instantiate and handle modules that are registered. Unregistered
	modules would have to be instantiated and managed manually.
*/
class SoundEngine
{
public:
	int instantiateModule(int moduleID);

	void deleteModuleInstance(int nodeID);

	/** @brief Processes the audio graph via the sink set using setSink and writes to the IO.
	
		Note that it is the responsibility of the sink to output to the correct channels. 
	*/
	virtual void onSound(al::AudioIOData& io);

	void patch(int destination_nodeID, int inlet_index, int source_nodeID, int outlet_index);

	void unpatch(int destination_nodeID, int inlet_index, int source_nodeID, int outlet_index);

	bool unpatch_from_inlet(int nodeID, int inlet_index);

	/// @brief Set the sink from which SoundEngine must process the audiograph. Automatically instantiates and returns the nodeID of the sink
	int setAndInstantiateSink(int sink_module_id) ;
	
	///@brief Set the sink from which SoundEngine must process the audiograph. Must already be instantiated
	void setSink(int sink_node_id, int sink_module_id);

	/// @brief Get the sink from which SoundEngine processes the audiograph.
	al::SinkModule& getSink(void);

	/** @brief Registers a module with the SoundEngine.

		The factory function is one where the module is instantiated and any setup is done. 
		It returns the nodeID of the module
	*/
	int RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function);

	/** @brief Registers a module with the SoundEngine using a default factory function default_module_factory.
			
		If there are any special steps to be taken in instantiating your module, use the other RegisterModule function provided.
	*/
	template<typename T>
	int RegisterModule(std::string module_name)
	{
		return RegisterModule(module_name, default_module_factory<T>);
	}

	void run() { getSink().run(); std::cout << "Running Sink " << std::endl; }
	void stop() { getSink().stop(); std::cout << "Stopping Sink " << std::endl; }
	bool isRunning(void);

private:
	al::SinkModule* sink_ref = NULL;
	std::vector<int> instantiatedNodeIDs = std::vector<int>();
	std::vector<std::string> ModuleNames = std::vector<std::string>();
	std::vector<ModuleFactoryFunction> ModuleConstructors =  std::vector<ModuleFactoryFunction>();
};

/// @brief Returns the default instance of the SoundEngine (instantited as a static within this function)
al::SoundEngine& DefaultSoundEngine(void);

/// @brief Sorthand for registering a Module to the default sound engine in DefaultSoundEngine()
int RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function);

/// @brief Sorthand for registering a Module to the default sound engine in DefaultSoundEngine()
template<typename T>
int RegisterModule(std::string module_name)
{
	return RegisterModule(module_name, default_module_factory<T>);
}

}; // namespace al

#endif // AL_SOUNDENGINE_HPP