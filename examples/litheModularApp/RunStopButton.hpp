#ifndef RUN_STOP_BUTTON_HPP
#define RUN_STOP_BUTTON_HPP

#include "glv/glv.h"
#include "MockSoundEngine.hpp"
#include "al_PatcherGUI.hpp"
#include <iostream>
using namespace std;

class RunStopButton : public glv::Button
{
public:
	RunStopButton(void) : 
		glv::Button((glv::Rect(40, 40)))
	{
		anchor(glv::Place::BL);
		pos(glv::Place::BL);
		property(glv::Visible, true);
		symbolOn(glv::draw::triangleR);
		symbolOff(glv::draw::rectTrunc<4,4,4,4>);
	}

	virtual bool onEvent(glv::Event::t e, glv::GLV& g) override
	{
		switch(e)
		{
			case glv::Event::MouseDown: return true;
		}
	}
};

#endif // RUN_STOP_BUTTON_HPP