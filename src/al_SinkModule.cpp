#include "../allolithe/al_SinkModule.hpp"

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

	re_sort();
	mRunning = true; 
}

void SinkModule::re_sort(void)
{
	lithe::BFS::sort(*this); // Sort the audio graph and enable 1-sample delays
}

// void SinkModule::Process(void)
// {
// 	DSP();
// 	doneProcessing();
// 	lithe::Node::resetAll_ProcessState(); // IMPORTANT
// }

al::SinkModule& SinkModule::getSinkModuleRef(int nodeID)
{
	try
	{
		al::SinkModule* module_ref = static_cast<al::SinkModule*>(lithe::Node::getNodeRef(nodeID));
		if(module_ref == NULL)
			throw std::runtime_error("Sink Module instance not found or instance was destroyed. NodeID: "+std::to_string(nodeID));
		else
			return *module_ref;
	}
	catch(const char * str)
	{
		throw std::runtime_error(str);
	}
}

void SinkModule::onSound(al::AudioIOData& io)
{
	if(isRunning())
	{
		while(io())
		{
			// Process();
			/// Fill audio objects
			lithe::Node::resetAll_ProcessState(); // IMPORTANT: DO THIS AFTER EVERY SAMPLE
		}
		// spatialize(io);
		/// Put buffer processors here
	}
}

}; // namespace al