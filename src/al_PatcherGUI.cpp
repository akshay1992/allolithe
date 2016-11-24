#include "allolithe/al_PatcherGUI.hpp"

#include <iostream>
using namespace std;

namespace al{

PatcherGUI::PatcherGUI(al::SoundEngine& sound_engine) : 
	sound_engine_ref(sound_engine),
	module_selector(sound_engine_ref),
	run_button(sound_engine_ref)
{
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

bool PatcherGUI::onEvent(glv::Event::t e, glv::GLV& glv)
{
	switch(e){
	// case glv::Event::MouseDrag:	return false;
	// case glv::Event::MouseDown: return false;
	// case glv::Event::KeyDown:  return false;
	}
	return true;	// bubble unrecognized events to parent
}

PatcherGUI::~PatcherGUI(void)
{
// 	if (win != NULL)
// 		delete win;
}


}; //namespace