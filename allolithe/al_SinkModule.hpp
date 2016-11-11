#ifndef AL_SINK_MODULE_HPP
#define AL_SINK_MODULE_HPP

#include "allolithe/al_Module.hpp"

namespace al{

/** @brief SinkModule is a type of module that has no outlets
	
	This module usually handles the spatialization and interfaces with the IO. It 
	is useful to treat each Inlet signal as a separate audio object and
	spatialize it. 
*/
class SinkModule : public al::Module
{
public:
	SinkModule(int numInlets, int numParameters);

	/// @brief Put the spatializer algorithm here. 
	// virtual void spatialize(al::AudioIOData& io)=0;

	/** @brief Use this to check if current IO device is compatible (min numChannels, .etc)
		before starting your audio callbacks
	*/
	virtual void checkIOcompatibility(al::AudioIOData& io) {}

	// virtual void Process(void) override;

	static al::SinkModule& getSinkModuleRef(int nodeID);

	void run(void);
	void stop(void) { mRunning = false; } 
	bool isRunning(void) { return mRunning; }

	/** @brief This is where the processing code goes. 
		
		Note: Remember to call lithe::Node::resetAll_ProcessState 
		after one sample is pulled from all inlets in the audiograph.
	*/
	virtual void onSound(al::AudioIOData& io);

private:
	std::atomic<bool> mRunning;

	/// @brief This function isn't used since we are processing per-buffer. use onSound instead
	using al::Module::Process;
	/// @brief This function isn't usedsince we are processing per-buffer. use onSound instead
	using al::Module::DSP;
};

}; // namespace al

#endif // AL_SINK_MODULE_HPP