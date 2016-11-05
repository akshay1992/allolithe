#ifndef SINK_MODULE_ONE_H
#define SINK_MODULE_ONE_H

#include "allolithe/al_allolithe.hpp"

// #include "gmock/gmock.h"
// using ::testing::_;

class SinkModuleOne : public al::SinkModule
{
public:
	SinkModuleOne(int numOutlets, int numParameters) : 
		al::SinkModule(numOutlets, numParameters)
	{

	}

	virtual void instantiate_parameters(void) override
	{
		for(int i=0; i<numParams(); ++i)
		{
			parameters.push_back( new al::Parameter("Param"+std::to_string(i), "group", i));
		}
	}

	virtual void spatialize(al::AudioIOData& io) override{}
	// MOCKMETHOD1(spatialize, void(al::AudioIOData& io));


	virtual void checkIOcompatibility(al::AudioIOData& io) override
	{
		if( io.channelsOut() < min_reqired_channels) 
			throw std::runtime_error("Incompatible IO with Sink "+getName());
		else if( io.framesPerBuffer() < lowest_buffer_length )
			throw std::runtime_error("Incompatible IO with Sink "+getName());
		else if ( io.framesPerSecond() < min_sample_rate )
			throw std::runtime_error("Incompatible IO with Sink "+getName());
	}

private:
	const int min_reqired_channels=8;
	const int min_sample_rate=48000;
	const int lowest_buffer_length= 128;


};

#endif  // SINK_MODULE_ONE_H