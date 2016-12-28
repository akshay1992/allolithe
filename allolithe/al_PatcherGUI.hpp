#ifndef AL_PATCHER_GUI_HPP
#define AL_PATCHER_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"

#include "allolithe/al_allolithe.hpp"
#include "allolithe/al_PatcherGUIComponents.hpp"

#include <iostream> 
#include <memory>
using namespace std;

#define VERTICES_SIZE 2

namespace al{

void PatchCallback(const glv::Notification &n);

/** @brief A UI for patching allolithe Modules

	This UI runs a SoundEngine in it's backend and is written using GLV. 
*/
class PatcherGUI : public glv::GLV
{
public:
	PatcherGUI(al::SoundEngine& sound_engine = al::DefaultSoundEngine());
	void openWindow(void);

	virtual void onDraw(glv::GLV& g) override;

	al::ViewpointWindow& createAlloWindow(void);

	~PatcherGUI(void);

	// void setParentWindow(al::Window& win) { top_view->parentWindow(win); }

	al::GLVDetachable* top_view;

private:
	al::SoundEngine& sound_engine_ref;
	RunStopButton run_button;
	QuitButton quit_button;
	ModuleList module_selector;

	al::ViewpointWindow* allo_win;
	glv::Window* win;
};

}; // namespace al

#endif // INCLUDE_PATCHER_GUI_HPP