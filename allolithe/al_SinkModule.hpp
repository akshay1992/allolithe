#ifndef AL_SINK_MODULE_HPP
#define AL_SINK_MODULE_HPP

#include "Lithe/LitheCore.h"
#include "allocore/ui/al_Parameter.hpp"
#include "alloGLV/al_ParameterGUI.hpp"
#include <vector>

namespace al{

/** @brief Base class for a sink in a lithe audiograph. 
	
	This is a decorated lithe::Sink to include parameters and an automatically 
	generated GLV view. It also contains a mechanism to start/stop processing the audiograph.

	@ingroup allolithe\
*/

class SinkModule : public lithe::Sink
{
public:
	SinkModule(int numInlets, int numParameters);

	~SinkModule(void);

	/** @brief Use this to fill up the parameters vector

		Instantiation using new keyword is supported since ~Module cleans this up using
		the deletes

		Note: This is not called by default. Make sure to call this in the 
		constructor of any derived class.
	*/
	virtual void instantiate_parameters(void) =0;

	int numParams(void) {return numParameters;}
	
	/// @brief Gets the al::Parameter specified by index
	al::Parameter& parameter(int index);

	/// @brief returns true if parameters have been instantiated
	bool paramsInstnatiated(void) { return (( parameters.size() == 0) ? false : true);}

	/// @brief Used to get a specific instance of a node in an audiograph. 
	static SinkModule& getSinkModuleRef(int nodeID);

	/** @brief NodeID is the ID of this module as a node in the audiograph.

		Note that this is different from the ModuleID. Every node in an audiograph has
		a nodeID regardless of what kind of node it is. 
	*/
	int getNodeID() { return getID(); }

	/** @brief ModuleID is the ID given to a module on registering it with the SoundEngine

		The SoundEngine then uses this as a way to keep track of all available Modules, along with 
		their instances and interconnections.
	*/
	int getSinkModuleID() { return moduleID; }

	/// @brief Creates a basic GLV View of showing all the parameters
	glv::View& createDefaultView(bool debugging=false);

	void setName(std::string name) { module_name = name; }
	std::string& getName(void) { return module_name; }

	void run(void);
	void stop(void) { mRunning = false; } 
	bool isRunning(void) { return mRunning.load(); }

	/// This is where the processing code goes. 
	virtual void onSound(al::AudioIOData& io)=0;

	/// @brief Used to check if current IO device is compatible.
	virtual void checkIOcompatibility(al::AudioIOData& io)=0;

protected:
	std::vector<al::Parameter*> parameters;
	const int numParameters;

private:
	virtual void DSP(void) override;
	std::atomic<bool> mRunning;
	using lithe::Node::getID;
	using lithe::Node::getNodeRef;
	std::string module_name;
	int moduleID = -1; 
};

}; // namespace al

#endif AL_SINK_MODULE_HPP