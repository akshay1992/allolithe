#ifndef STEREO_SINK
#define STEREO_SINK

#include "allolithe/al_allolithe.hpp"
#include "al_StereoSinkGUI.hpp"

namespace al{

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

	virtual void onSound(al::AudioIOData& io) override
	{
		if(isRunning())
		{
			while(io())
			{
				float out0=0;
				float out1=0;
				for( int i; i<numInlets(); ++i)
				{
					out0 += getInlet(0).getSample().audio;
					out1 += getInlet(0).getSample().audio;
				}
				io.out(0) = out0;
				io.out(1) = out1;
			}
		}
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


}; // namespace al

#endif // STEREO_SINK