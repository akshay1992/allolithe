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
	virtual void spatialize(al::AudioIOData& io)=0;

	/// @brief Used to check if current IO device is compatible (min numChannels, .etc)
	virtual void checkIOcompatibility(al::AudioIOData& io)=0;

	virtual void Process(void) override;

	void run(void);
	void stop(void) { mRunning = false; } 
	bool isRunning(void) { return mRunning.load(); }

	/// @brief This is where the processing code goes. 
	void onSound(al::AudioIOData& io);

private:
	std::atomic<bool> mRunning;
};

}; // namespace al

#endif // AL_SINK_MODULE_HPP