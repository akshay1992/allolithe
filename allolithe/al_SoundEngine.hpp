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

/** @brief SoundEngine connects the audiograph to the IO, and handles node management.

	SoundEngine can only instantiate and handle modules that are registered. Unregistered
	modules would have to be instantiated and managed manually.
*/
class SoundEngine
{
public:
	SoundEngine() {}
	
	static int instantiateModule(int moduleID);

	static void deleteModuleInstance(int nodeID);

	/** @brief Processes the audio graph and writes to the IO.
	
		Note that it is the responsibility of the sink to output the correct number of channels. 
	*/
	virtual void onSound(al::AudioIOData& io);

	/// @brief Set the sink from which SoundEngine must process the audiograph.
	void setSink(al::SinkModule& sink);

	/** @brief Registers a module with the SoundEngine.

	*/
	static int RegisterModule(std::string module_name, ModuleFactoryFunction module_factory_function);

private:
	al::SinkModule* sink_ref;
	static std::vector<int> instantiatedNodeIDs;
    static std::vector<std::string> ModuleNames;
  	static std::vector<ModuleFactoryFunction> ModuleConstructors;
};

}; // namespace al


#endif // AL_SOUNDENGINE_HPP