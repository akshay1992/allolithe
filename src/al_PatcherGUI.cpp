#include "allolithe/al_PatcherGUI.hpp"

#include <iostream>
using namespace std;

namespace al{

PatcherGUI::PatcherGUI(al::SoundEngine& sound_engine) : 
	sound_engine_ref(sound_engine),
	module_selector(sound_engine_ref),
	run_button(sound_engine_ref)
{
	name("LithePatcherGUI");
	module_selector.refreshLlist();
	(*this).add(module_selector);
	(*this).add(run_button);
	(*this).add(quit_button);
}

void PatcherGUI::openWindow(void)
{
	win = new glv::Window(800,600, "LitheModular", this);
	glv::Application::run();
}

void PatcherGUI::onDraw(glv::GLV& g)
{
}

PatcherGUI::~PatcherGUI(void)
{
// 	if (win != NULL)
// 		delete win;
}


}; //namespace