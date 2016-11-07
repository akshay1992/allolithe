#include "allolithe/al_SinkModule.hpp"

namespace al{ 

SinkModule::SinkModule(int numInlets, int numParameters) :
	al::Module(numInlets, 0, numParameters),
	mRunning(false)
{
}

void SinkModule::run() 
{ 
	if(isRunning()) 
		return;

	lithe::BFS::sort(*this); // Sort the audio graph and enable 1-sample delays
	mRunning = true; 
}

void SinkModule::Process(void)
{
	DSP();
	doneProcessing();
	lithe::Node::resetAll_ProcessState(); // IMPORTANT
}

void SinkModule::onSound(al::AudioIOData& io)
{
	if(isRunning())
	{
		while(io())
		{
			Process();
			/// Fill audio objects
		}
		spatialize(io);
	}
}

}; // namespace al