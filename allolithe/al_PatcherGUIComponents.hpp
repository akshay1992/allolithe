#ifndef AL_PATCHER_GUI_COMPONENTS_HPP
#define AL_PATCHER_GUI_COMPONENTS_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"
#include "allolithe/al_allolithe.hpp"
#include "allolithe/al_ModuleGUI.hpp"

/// @brief THis file contains all the GUI components used by al::PatcherGUI

namespace al{
	
struct InstantiateModuleEvent : public glv::EventHandler
{
	InstantiateModuleEvent(al::SoundEngine& se, int& selected_module_id, glv::Buttons& b) : 
		selected_module_id(selected_module_id) ,
		sound_engine_ref(&se),
		buttons(b)
	{	}

	virtual bool onEvent(glv::View &v, glv::GLV &g)
	{
		int x = g.mouse().x();
		int y = g.mouse().y();
		if( ! v.containsPoint(x, y) )
		{
			ModuleInfo module = sound_engine_ref->getModuleInfo(selected_module_id);

			NodeInfo info = sound_engine_ref->instantiateModule(module.moduleID);

			if( ! sound_engine_ref->sinkIsSet() && module.isASink )
				sound_engine_ref->setAndInstantiateSink(module.moduleID);

			al::ModuleGUI* gui = new ModuleGUI( al::Module::getModuleRef(info.nodeID), module.moduleName );
			glv::View* v = &gui->getView();
			buttons.parent->parent->add(*v);
			v->pos(x, y);

			buttons.setValue(0);
			return false;
		}
		buttons.setValue(0);
		return true;
	}

	glv::Buttons& buttons;
	al::SoundEngine* sound_engine_ref;
	int& selected_module_id;
};

class ModuleList : public glv::Box
{
public:
	InstantiateModuleEvent instantiateModuleEvent;

	ModuleList(al::SoundEngine& se) : 
		sound_engine_ref(&se),
		glv::Box(glv::Direction::S),
		instantiateModuleEvent(se, selected_module_id, buttons)
	{
		name("ModuleList");
	}

	void refreshLlist(void)
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

	void onDraw(glv::GLV& g)
	{
		for(int i=0; i<buttons.size(); ++i)
		{
			if(buttons.getValue(i) == 1)
			{
				selected_module_id = modules[i].moduleID;
				return;
			}
		}
		selected_module_id = -1;
	}

	glv::Buttons buttons;
	std::vector<ModuleInfo> modules;
	al::SoundEngine* sound_engine_ref;
	int selected_module_id = -1;
};

class QuitButton : public glv::Button
{
public:
	QuitButton(void) : 
		glv::Button((glv::Rect(60, 30)))
	{
		name("QuitButton");
		anchor(glv::Place::BR);
		pos(glv::Place::BR);
		property(glv::Visible, true);

		(*this) << (new glv::Label("Quit"))->anchor(0.5, 0.5).pos(glv::Place::CC);
	}

	virtual bool onEvent(glv::Event::t e, glv::GLV& g) override
	{
		switch(e)
		{
			case glv::Event::MouseDown: glv::Application::quit();
			default: return true;
		}
	}
};

class RunStopButton : public glv::Button
{
public:
	RunStopButton(al::SoundEngine& se) : 
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

	virtual bool onEvent(glv::Event::t e, glv::GLV& g) override
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
	bool state = false;
	al::SoundEngine& sound_engine;
};

}; // namespace al

#endif // AL_PATCHER_GUI_COMPONENTS_HPP