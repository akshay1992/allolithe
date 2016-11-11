#ifndef INCLUDE_PATCHER_GUI_HPP
#define INCLUDE_PATCHER_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"

#include "allolithe/al_allolithe.hpp"

// Components
#include "RunStopButton.hpp"

namespace al{

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