#ifndef AL_MODULE_GUI_HPP
#define AL_MODULE_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"

#include "allolithe/al_Module.hpp"
#include "allolithe/al_SoundEngine.hpp"

#include <memory>
#include <iostream>
using namespace std;
namespace al{

class Inlets;
class Outlets;
class ModuleGUI;
class PatcherGUI;

class ModuleGUIKeyDownEvent : public glv::EventHandler
{
public:
	ModuleGUIKeyDownEvent(ModuleGUI& moduleGUI);
	
	virtual bool onEvent(glv::View &v, glv::GLV &g);

	glv::Notifier notifier; // To notify patcher GUI
	ModuleGUI& moduleGUI_ref;
};


class MouseUpOutletEvent : public glv::EventHandler
{
public: 
	MouseUpOutletEvent(Outlets& outlets);

	virtual bool onEvent(glv::View &v, glv::GLV &g);

	glv::Notifier notifier;
	Outlets& outlets_ref;
};

class MouseUpInletEvent : public glv::EventHandler
{
public: 
	MouseUpInletEvent(Inlets& inlets) : inlets_ref(inlets)
	{	}

	virtual bool onEvent(glv::View &v, glv::GLV &g);

	Inlets& inlets_ref;
	static bool active;
};


class Outlets : public glv::View
{
public:
	Outlets(al::SoundEngine& se, al::ModuleGUI& moduleGUI, int size = 15);

	/// Gets to abs co-ordinate of the center of a given outlet
	glv::Point2 getPatchPoint(int index);
	
	virtual void onDraw( glv::GLV& g) override;

	int numOutlets;
	int size;
	// al::Module& module_ref;
	al::ModuleGUI& moduleGUI_ref;
	al::SoundEngine& soundengine_ref;
	int selected_outlet = -1;
	std::unique_ptr<glv::Buttons> buttons;
	MouseUpOutletEvent mouseUpOutletEvent;
};

class Inlets : public glv::View
{
public:
	Inlets(al::ModuleGUI& moduleGUI, int size = 15);

	/// Gets to abs co-ordinate of the center of a given inlet
	glv::Point2 getPatchPoint(int index);

	virtual void onDraw( glv::GLV& g) override;

	int numInlets;
	int size;
	
	// al::Module& module;
	al::ModuleGUI& moduleGUI_ref;
	int selected_inlet = -1;
	static int last_selected_inlet_index;
	static Inlets* last_selected_inlets_ref;
	unique_ptr<glv::Buttons> buttons;
	MouseUpInletEvent mouseUpInletEvent;
};


class ModuleGUI
{
public:

	ModuleGUI(al::PatcherGUI& gui, al::SoundEngine& se, al::Module& module, std::string& moduleName);

	~ModuleGUI();

	glv::View& getView(void);

	void addInlets(void);

	void addOutlets(void);

	void addName(std::string& moduleName, bool show_node_id=false);

	void addParameters(void);

	void addParameter(al::Parameter& parameter);

	void setParentPatcherGUI(PatcherGUI& patcherGUI);


	static void widgetChangeCallback(const glv::Notification& n);

	void deleteSelf(void);
	
	struct WidgetWrapper
	{
		ParameterWrapper<float> *parameter;
		glv::Widget *widget;
		glv::Label *label;	// optional
		std::mutex *lock;
	};

	friend class Outlets;
	friend class Inlets;

public:
	// Indexes to the relevant patch chords in patcher GUI.
	std::vector<int> patch_indices;	
	al::Module& module_ref;
	al::SoundEngine& soundengine_ref;
	Inlets* inlets;
	Outlets* outlets;
	const int pad = 10;
	/// @brief container for patching information to be passed in the event of a patch
	// const int padTop = 10;
	// const int padBottom = 10;

	al::PatcherGUI& parentPatcherGUI_ref;	//set this at creation time
	glv::Notifier unpatch_notifier;
	ModuleGUIKeyDownEvent moduleGUIKeyDownEvent;
	unique_ptr<glv::View> top;
	unique_ptr<glv::Box> mBox;
	unique_ptr<glv::Label> name_label;
	std::mutex mParameterGUILock;
	std::vector<WidgetWrapper *> mWrappers;
};

}; // namespace al

#endif //AL_MODULE_GUI_HPP