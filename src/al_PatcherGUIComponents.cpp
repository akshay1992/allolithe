#include "allolithe/al_PatcherGUIComponents.hpp"
#include "allolithe/al_ModuleGUI.hpp"
#include "allolithe/al_PatcherGUI.hpp"
#include <algorithm>
namespace al{

QuitButton::QuitButton(void) : 
	glv::Button((glv::Rect(60, 30)))
{
	name("QuitButton");
	anchor(glv::Place::BR);
	pos(glv::Place::BR);
	property(glv::Visible, true);

	(*this) << (new glv::Label("Quit"))->anchor(0.5, 0.5).pos(glv::Place::CC);
}

bool QuitButton::onEvent(glv::Event::t e, glv::GLV& g)
{
	switch(e)
	{
		case glv::Event::MouseDown: glv::Application::quit();
		default: return true;
	}
}

ModuleList::ModuleList(al::PatcherGUI& gui, al::SoundEngine& se) : 
		sound_engine_ref(&se),
		glv::Box(glv::Direction::S),
		mouseUpEvent(gui, *this) 
{
	name("ModuleList");
}

void ModuleList::refreshLlist(void)
{
	modules = sound_engine_ref->getRegisteredModules();

	// Recreate buttons	
	int numModules = modules.size();
	buttons = glv::Buttons(glv::Rect(120, numModules*15), 1, numModules, false, true);

	// Add labels
	float y = 1./(buttons.size()*2);
	float dy = 2*y;
	for( ModuleInfo& module : modules )
	{
		buttons << (new glv::Label(module.moduleName))->anchor(0.5, y).pos(glv::Place::CC);
		y+=dy;
	}

	// Add handler for instantiating modules
	// buttons.addHandler(glv::Event::MouseUp, instantiateModuleEvent);
	buttons.addHandler(glv::Event::MouseUp, mouseUpEvent);
	(*this) << buttons;
}

void ModuleList::onDraw(glv::GLV& g)
{
	for(int i=0; i<buttons.size(); ++i)
	{
		if(buttons.getValue(i) == 1)
		{
			selected_module_id = modules[i].moduleName;
			return;
		}
	}
	selected_module_id = "";
}

ModuleList::MouseUpEvent::MouseUpEvent(al::PatcherGUI& gui, al::ModuleList& moduleList) : 
	moduleList(moduleList), 
	gui(gui)
{
}

bool ModuleList::MouseUpEvent::onEvent(glv::View &v, glv::GLV &g)
{
	int x = g.mouse().x();
	int y = g.mouse().y();
	if( ! v.containsPoint(x, y) )	// ensures that the user has dragged outside the ModuleList
	{

		al::ModuleGUI& module = gui.instantiateModule(moduleList.selected_module_id);
		module.getView().pos(x, y);

		moduleList.buttons.setValue(0);
		return false;
	}
	moduleList.buttons.setValue(0);
	return true;
}

RunStopButton::RunStopButton(al::SoundEngine& se) : 
	glv::Button((glv::Rect(40, 40))),
	sound_engine(se)
{
	name("RunStopButton");
	anchor(glv::Place::BL);
	pos(glv::Place::BL);
	property(glv::Visible, true);
	symbolOn(glv::draw::triangleR);
	symbolOff(glv::draw::rectTrunc<4,4,4,4>);
	attachVariable(state);
}

bool RunStopButton::onEvent(glv::Event::t e, glv::GLV& g)
{
	switch(e)
	{
		case glv::Event::MouseDown: 
		if( sound_engine.sinkIsSet() )
		{
			if(sound_engine.isRunning())
			{
				sound_engine.stop();
				state = false;
			}
			else
			{
				sound_engine.run();
				state = true;
			}				
			return false;				
		}
		default: return true;
	}
}

void PatchChords::draw(glv::GLV& g)
{
	glv::GraphicsData& gd = g.graphicsData();

	if(delete_queue.size() !=0 )
	{
		while(delete_queue.size() != 0 )
		{
			int i = delete_queue.back();
		    using std::swap;
		    swap(patches[i], patches.back());
		    PatchInfo& p = patches.back();
		    patches.pop_back();

		    delete_queue.pop_back();
		}
	}

	int i=0;
	for( PatchInfo& p : patches)
	{
		glv::Point2 origin, destination;

		destination = p.inlets_ref->getPatchPoint(p.inlet_index);
		origin = p.outlets_ref->getPatchPoint(p.outlet_index);

		gd.addVertex(origin.x, origin.y);
		gd.addColor(glv::HSV(0,1,1));
		gd.addVertex(destination.x, destination.y);
		gd.addColor(glv::HSV(0,1,1));
		glv::draw::paint(glv::draw::Lines, gd);
	}
}

void PatchChords::addPatch(PatchInfo& p)
{
	patches.push_back(p);
}

void PatchChords::removePatchAtIndex(int p_index)
{
	delete_queue.push_back(p_index);
    // using std::swap;
    // swap(patches[p_index], patches.back());
    // PatchInfo& p = patches.back();
    // patches.pop_back();
}


}; // namespace al