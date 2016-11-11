#ifndef STEREO_SINK_GUI_HPP
#define STEREO_SINK_GUI_HPP

#include "glv/glv_core.h"
#include "allolithe/al_allolithe.hpp"

namespace al{

class StereoSinkGUI : public glv::View
{
public:
	StereoSinkGUI(al::Module& module)
	{
		for(int i=0; i< module.numParams(); ++i)
		{
			parameter_gui << module.parameter(i);
		}

		(*this)<< parameter_gui.getDetachableView();
		addHandler(glv::Event::MouseDrag, glv::Behavior::mouseMove);
	}
	al::ParameterGUI parameter_gui;
};

}; // namespace al
#endif // STEREO_SINK_GUI_HPP