#ifndef MODULE_SINK
#define MODULE_SINK

class ModuleSink : public al::SinkModule
{
public:
	ModuleSink(void) : al::SinkModule(1, 0) {}

	virtual void instantiate_parameters(void) override {}

	virtual void DSP(void) override
	{
		// Store sample in s
		s = getInlet(0).getSample();
	}

	virtual void spatialize(al::AudioIOData& io) override {}

	virtual void checkIOcompatibility(al::AudioIOData& io) override {}

	lithe::Sample s;
};

#endif // MODULE_SINK