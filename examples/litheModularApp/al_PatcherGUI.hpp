#ifndef INCLUDE_PATCHER_GUI_HPP
#define INCLUDE_PATCHER_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"

#include "RunStopButton.hpp"
#include "allolithe/al_allolithe.hpp"
#include "DummyModule.hpp"

namespace al{

class PatcherGUI : public glv::GLV
{
public:
	PatcherGUI(void)
	{
		run_button.attachVariable(run_state);
	}

	void openWindow(void)
	{
		win = new glv::Window(800,600, "LitheModular", this);

		(*this) << run_button;
		glv::Application::run();
	}

	virtual bool onEvent(glv::Event::t e, glv::GLV& glv) override
	{
		switch(e){
		// case glv::Event::MouseDrag:	glv::Behavior::mouseMove; return true;
		case glv::Event::MouseDown: mouseDown_handler(e, glv);	return false;
		case glv::Event::KeyDown: keyDown_handler(e, glv); return false;
		}
		return true;	// bubble unrecognized events to parent
	}

	void mouseDown_handler(glv::Event::t e, glv::GLV& glv)
	{
		int x = glv.mouse().x();
		int y = glv.mouse().y();
		if( run_button.containsPoint(x, y) && glv.mouse().left() )
		{
			run_state = toggleRunSoundEngine();
		}
		if( glv.mouse().right() )
		{
			cout << "New module finder dropdown" << endl;
			sound_engine_ref.instantiateModule((DummyModule::moduleID));
			glv::View& view = MockModule::getModuleRef(0).createDefaultView(); 
			(*this).add(view);
			view.pos(x, y);
		}
	}

	void keyDown_handler(glv::Event::t e, glv::GLV& glv)
	{
		switch(glv.keyboard().key())
		{
			case 'q': glv::Application::quit(); 
			case 'r': run_state = toggleRunSoundEngine(); 
		}
	}

	bool toggleRunSoundEngine(void)
	{
		if(sound_engine_ref.isRunning() )
		{
			sound_engine_ref.stop();
			return sound_engine_ref.isRunning();
		}
		else
		{
			sound_engine_ref.run();
			return sound_engine_ref.isRunning();
		}
	}

	~PatcherGUI(void)
	{
		if (win != NULL)
			delete win;
	}

private:
	RunStopButton run_button;
	bool run_state = false;

	glv::Window* win;
	al::SoundEngine& sound_engine_ref = DefaultSoundEngine();
};

}; // namespace al

#endif // INCLUDE_PATCHER_GUI_HPP