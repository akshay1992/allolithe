#ifndef STEREO_SINK
#define STEREO_SINK

#include "allolithe/al_allolithe.hpp"
#include "al_StereoSinkGUI.hpp"

class StereoSink : public al::SinkModule
{
public:
	StereoSink(int numInlets=1) : al::SinkModule(numInlets, numInlets)
	{
		instantiate_parameters();
	}
	
	virtual void instantiate_parameters(void) override
	{
		for(int i=0; i< numInlets(); ++i)
		{
			parameters.push_back( new al::Parameter("Obj"+std::to_string(i)+" Gain", std::to_string(getNodeID()), 0, "", 0.0, 1.0 ));
		}
	}

	/// @brief Put the spatializer algorithm here. 
	virtual void spatialize(al::AudioIOData& io) override
	{
		while(io)
		{
			for(int i=0;)
		}

	}

	virtual void DSP()
	{

	}

	virtual void checkIOcompatibility(al::AudioIOData& io) override
	{

	}

	virtual glv::View& createView(bool debugging=false) override
	{	
		gui = new al::StereoSinkGUI(*this);
		return *gui;
	}

	static int moduleID;
	static std::string moduleName;

	al::StereoSinkGUI* gui;
};


#endif // STEREO_SINK