#include "allolithe/al_SinkModule.hpp"

namespace al{

SinkModule::SinkModule(int numOutlets, int numParameters) :
	lithe::Sink(numOutlets),
	numParameters(numParameters)
{
}

al::Parameter& SinkModule::parameter(int index)
{
	if( index < 0 || index >= numParams() )
	{
		throw std::range_error("Parameter index not valid");
	}
	else
	{
		return *parameters[index];
	}
}

SinkModule::~SinkModule(void)
{
	for(int i=0; i<parameters.size(); ++i)
	{
		if(parameters[i] != NULL) delete parameters[i];
	}
}

glv::View& SinkModule::createDefaultView(bool debugging)
{
	// TODO:

}

SinkModule& SinkModule::getSinkModuleRef(int nodeID)
{
	try
	{
		al::SinkModule* module_ref = static_cast<al::SinkModule*>(lithe::Node::getNodeRef(nodeID));
		if(module_ref == NULL)
			throw std::runtime_error("SinkModule instance not found or instance was destroyed");
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
	if(!isRunning())
	{
		return;		
	}
	else
	{
		//Per buffer area

		//// Per sample area
		{
			Process(); // the audio graph / sample
			/// You want to overrider the DSP function

			// for loop to fill one frame
		}
		////

		// Scene.render?
	}
}

void SinkModule::DSP(void)
{
	for(int i=0; i<numInlets(); ++i)
	{
		Sample
	}

}

void SinkModule::run() 
{ 
	if(isRunning()) 
		return;

	refreshProcessOrder();
	mRunning = true; 
}

}; // namespace al