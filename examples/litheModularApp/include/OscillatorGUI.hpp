#ifndef OSCILLATOR_GUI_HPP
#define OSCILLATOR_GUI_HPP

#include "glv/glv_core.h"
#include "allolithe/al_allolithe.hpp"

class OscillatorGUI : public glv::View
{
public:
	OscillatorGUI(al::Module& module)
	{
		for(int i=0; i< module.numParams(); ++i)
		{
			parameter_gui << module.parameter(i);
		}

		(*this)<< parameter_gui.getDetachableView();
		addHandler(glv::Event::MouseDrag, glv::Behavior::mouseMove);
	}
	al::ParameterGUI parameter_gui;

	// glv::View inner;
};

#endif