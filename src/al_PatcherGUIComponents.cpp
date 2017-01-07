#include "allolithe/al_PatcherGUIComponents.hpp"
#include "allolithe/al_ModuleGUI.hpp"

namespace al{

InstantiateModuleEvent::InstantiateModuleEvent(al::SoundEngine& se, std::string& selected_module_id, glv::Buttons& b) : 
	selected_module_id(selected_module_id) ,
	sound_engine_ref(se),
	buttons(b)
{

}

bool InstantiateModuleEvent::onEvent(glv::View &v, glv::GLV &g)
{
	int x = g.mouse().x();
	int y = g.mouse().y();
	if( ! v.containsPoint(x, y) )
	{
		ModuleInfo module = sound_engine_ref.getModuleInfo(selected_module_id);

		NodeInfo info = sound_engine_ref.instantiateModule(module.moduleName);

		if( ! sound_engine_ref.sinkIsSet() && module.isASink )
			sound_engine_ref.setAndInstantiateSink(module.moduleName);

		al::Module& module_ref = al::Module::getModuleRef(info.nodeID);
		al::ModuleGUI* gui = new ModuleGUI(sound_engine_ref, module_ref, module.moduleName);
		glv::View* v = &gui->getView();
		buttons.parent->parent->add(*v);
		v->pos(x, y);

		buttons.setValue(0);
		return false;
	}
	buttons.setValue(0);
	return true;
}

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

ModuleList::ModuleList(al::SoundEngine& se) : 
		sound_engine_ref(&se),
		glv::Box(glv::Direction::S),
		instantiateModuleEvent(se, selected_module_id, buttons)
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
	buttons.addHandler(glv::Event::MouseUp, instantiateModuleEvent);
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

	for( shared_ptr<PatchInfo> p : patches)
	{
		glv::Point2 origin, destination;

		destination = p->inlets_ref.getPatchPoint(p->inlet_index);
		origin = p->outlets_ref.getPatchPoint(p->outlet_index);

		gd.addVertex(origin.x, origin.y);
		gd.addColor(glv::HSV(0,1,1));
		gd.addVertex(destination.x, destination.y);
		gd.addColor(glv::HSV(0,1,1));
		glv::draw::paint(glv::draw::Lines, gd);
	}
}

void PatchChords::addPatch(std::shared_ptr<PatchInfo> p)
{
	patches.push_back(p);
}

void PatchChords::removePatchAtIndex(int p_index)
{
	patches.erase(patches.begin() + 1);
}


}; // namespace al