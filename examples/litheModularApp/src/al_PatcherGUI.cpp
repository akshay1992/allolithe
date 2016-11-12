#include "al_PatcherGUI.hpp"
#include "include_modules.hpp"

#include <iostream>
using namespace std;

namespace al {

PatcherGUI::PatcherGUI(al::SoundEngine& sound_engine) : 
	sound_engine_ref(sound_engine)
{
	sound_engine_ref.instantiateModule(al::StereoSink::moduleID);
	// sound_engine_ref.set

	// sound_engine_ref.setAndInstantiateSink(al::StereoSink::moduleID);
	// (*this).add(sound_engine_ref.getSink().createView());
}

void PatcherGUI::openWindow(void)
{
	win = new glv::Window(800,600, "LitheModular", this);

	(*this) << run_button;
	glv::Application::run();
}

bool PatcherGUI::onEvent(glv::Event::t e, glv::GLV& glv)
{
	switch(e){
	// case glv::Event::MouseDrag:	glv::Behavior::mouseMove; return true;
	case glv::Event::MouseDown: mouseDown_handler(e, glv);	return false;
	case glv::Event::KeyDown: keyDown_handler(e, glv); return false;
	}
	return true;	// bubble unrecognized events to parent
}

void PatcherGUI::mouseDown_handler(glv::Event::t e, glv::GLV& glv)
{
	int x = glv.mouse().x();
	int y = glv.mouse().y();
	if( run_button.containsPoint(x, y) && glv.mouse().left() )
	{
		toggleRunSoundEngine();
	}
	if( glv.mouse().right() )
	{
		cout << "New module finder dropdown" << endl;

		// New module goes here. 
		// Instantiate the GUI also. 

		int id = sound_engine_ref.instantiateModule(Oscillator::moduleID);
		al::Module::getModuleRef(id);
		glv::View& view = al::Module::getModuleRef(id).createView();
		// view.pos(x, y);
		// (*this).add(view);

		sound_engine_ref.patch(sound_engine_ref.getSink().getNodeID(), 0, id, 0);
	}
}

void PatcherGUI::keyDown_handler(glv::Event::t e, glv::GLV& glv)
{
	switch(glv.keyboard().key())
	{
		case 'q': glv::Application::quit(); 
		case 'r': toggleRunSoundEngine(); 
	}
}

bool PatcherGUI::toggleRunSoundEngine(void)
{
	if(sound_engine_ref.isRunning() )
	{
		sound_engine_ref.stop();			
	}
	else
	{
		sound_engine_ref.run();
	}
	run_button.state = sound_engine_ref.isRunning();
	return run_button.state;
}

PatcherGUI::~PatcherGUI(void)
{
	if (win != NULL)
		delete win;
}

}; // namespace al