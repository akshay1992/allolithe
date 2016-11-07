#ifndef MODULE_EFFECT_HPP
#define MODULE_EFFECT_HPP

#include "allolithe/al_allolithe.hpp"

class ModuleEffect : public al::Module
{
public:
	ModuleEffect(void) : al::Module(1, 1, 0) {}
	
	virtual void instantiate_parameters(void) override {}

	virtual void DSP(void) override
	{
		/// DC_SHIFT by 0.1
		lithe::Sample s = getInlet(0).getSample();
		s.az = lithe::dc_shift_wrap<float>( s.az, 0.1, lithe::Sample::getRange());
		s.el = lithe::dc_shift_wrap<float>( s.el, 0.1, lithe::Sample::getRange());
		s.d = lithe::dc_shift_wrap<float>( s.d, 0.1, lithe::Sample::getRange());
		getOutlet(0).setSample(s);
	}
};

#endif //MODULE_EFFECT_HPP