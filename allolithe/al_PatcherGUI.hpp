#ifndef AL_PATCHER_GUI_HPP
#define AL_PATCHER_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"

#include "allolithe/al_allolithe.hpp"
#include "allolithe/al_PatcherGUIComponents.hpp"

namespace al{

/** @brief A UI for patching allolithe Modules

	This UI runs a SoundEngine in it's backend and is written using GLV. 
*/
class PatcherGUI : public glv::GLV
{
public:
	PatcherGUI(al::SoundEngine& sound_engine = al::DefaultSoundEngine());
	void openWindow(void);

	virtual void onDraw(glv::GLV& g) override;

	~PatcherGUI(void);

	al::SoundEngine& sound_engine_ref;

private:
	RunStopButton run_button;
	QuitButton quit_button;
	ModuleList module_selector;

	glv::Window* win;
};

}; // namespace al

#endif // INCLUDE_PATCHER_GUI_HPP