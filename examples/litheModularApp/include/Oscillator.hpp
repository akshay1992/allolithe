#ifndef OSCILLATOR
#define OSCILLATOR
#include <iostream>
#include "allolithe/al_alloLithe.hpp"
#include "Gamma/Oscillator.h"
#include "OscillatorGUI.hpp"

class Oscillator : public al::Module
{
public:
	friend OscillatorGUI;
	enum Params : int
	{
		FREQUENCY,
		AZ,
		EL,
		DIST,
		NUM_PARAMS
	};
	
	enum Inlets : int
	{
		FM, 
		NUM_INLETS
	};

	enum Outlets : int {
		SINE,
		SQUARE, 
		SAW, 
		TRIANGLE,
		NUM_OUTLETS
	};

	Oscillator(void) : al::Module(NUM_INLETS, NUM_OUTLETS, NUM_PARAMS) 
	{
		instantiate_parameters();
	}

	~Oscillator(void)
	{
		for(int i=0; i<parameters.size(); ++i)
		{
			if (parameters[i] != NULL) 
				delete parameters[i];
		}

		if (gui != NULL) delete gui;
	}

	virtual void instantiate_parameters(void) override
	{
		parameters.push_back( new al::Parameter("frequency", std::to_string(getNodeID()), 440.0, "", 0.0, 20000.0 ));
		parameters.push_back( new al::Parameter("az", std::to_string(getNodeID()), 0, "", -1.0, +1.0 ));
		parameters.push_back( new al::Parameter("el", std::to_string(getNodeID()), 0, "", -1.0, +1.0 ));
		parameters.push_back( new al::Parameter("dist", std::to_string(getNodeID()), 0, "", -1.0, +1.0 ));
	}

	void updateFreqs(float frequency)
	{
		float new_freq = parameters[FREQUENCY]->get();
		sine.freq(new_freq);
		square.freq(new_freq);
		saw.freq(new_freq);
		dwo.freq(new_freq);
	}

	virtual void DSP(void) override
	{
		/// TODO: Add modulation code here. 
		getOutlet(SINE).setSample( lithe::Sample( sine(), parameters[AZ]->get(), parameters[EL]->get(), parameters[DIST]->get()) );
		getOutlet(SQUARE).setSample( lithe::Sample( square(), parameters[AZ]->get(), parameters[EL]->get(), parameters[DIST]->get()) );
		getOutlet(SAW).setSample( lithe::Sample( saw(), parameters[AZ]->get(), parameters[EL]->get(), parameters[DIST]->get()) );
		getOutlet(TRIANGLE).setSample( lithe::Sample( dwo(), parameters[AZ]->get(), parameters[EL]->get(), parameters[DIST]->get()) );
	}

	virtual glv::View& createView(bool debugging = false) override
	{

		gui = new OscillatorGUI(*this);
		return *gui;
	}

	static int moduleID;
	static std::string moduleName;

private:
	OscillatorGUI* gui;
	gam::Sine<> sine;
	gam::Square<> square;
	gam::Saw<> saw;
	gam::DWO<> dwo; /// For triangle wave
};

// std::string Oscillator::moduleName = "Oscillator";
// int Oscillator::moduleID = al::RegisterModule<Oscillator>(Oscillator::moduleName);

#undef MODULE_NAME
#endif // OSCILLATOR