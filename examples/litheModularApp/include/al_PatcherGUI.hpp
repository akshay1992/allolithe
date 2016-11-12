#ifndef INCLUDE_PATCHER_GUI_HPP
#define INCLUDE_PATCHER_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"
#include "allolithe/al_allolithe.hpp"

namespace al{

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
		attachVariable(state);
	}

	virtual bool onEvent(glv::Event::t e, glv::GLV& g) override
	{
		switch(e)
		{
			case glv::Event::MouseDown: return true;
		}
	}
	bool state = false;
};


// ---------------P A T C H E R     G U I ------------------
class PatcherGUI : public glv::GLV
{
public:
	PatcherGUI(al::SoundEngine& sound_engine = al::DefaultSoundEngine());
	void openWindow(void);

	virtual bool onEvent(glv::Event::t e, glv::GLV& glv) override;
	void mouseDown_handler(glv::Event::t e, glv::GLV& glv);

	void keyDown_handler(glv::Event::t e, glv::GLV& glv);
	bool toggleRunSoundEngine(void);

	~PatcherGUI(void);

	al::SoundEngine& sound_engine_ref;

private:
	RunStopButton run_button;

	glv::Window* win;
};

}; // namespace al

#endif // INCLUDE_PATCHER_GUI_HPP