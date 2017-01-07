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

struct PatchInfo
{
	PatchInfo(Inlets& inlets_ref, Outlets& outlets_ref) : inlets_ref(inlets_ref), outlets_ref(outlets_ref){}
	int source_nodeID;
	int inlet_index;
	Inlets& inlets_ref;
	
	int destination_nodeID;
	int outlet_index;
	Outlets& outlets_ref;

	void print()
	{
		std::cout << source_nodeID << " " << inlet_index << " -- "<< destination_nodeID << " " << outlet_index  << std::endl;
	}
};

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

	ModuleGUI(al::SoundEngine& se, al::Module& module, std::string& moduleName) :
		soundengine_ref(se), 
		module_ref(module),
		moduleGUIKeyDownEvent(*this)
	{
		mBox.reset(new glv::Box(glv::Direction::S) );
		mBox->pos(glv::Place::TL, 10, 0);
		
		addInlets();
		addName(moduleName);
		addParameters();
		addOutlets();

		top.reset(new glv::View(glv::Rect(0, 0, mBox->width() + 2*pad, mBox->height())) );
		*top << mBox.get();

		top->addHandler(glv::Event::MouseDrag, glv::Behavior::mouseMove);
		top->enable(glv::Property::Controllable);
		top->enable(glv::Property::DrawBorder);

		top->addHandler(glv::Event::KeyDown, moduleGUIKeyDownEvent);
	}

	~ModuleGUI()
	{
		cout << "deleting module GUI" << endl;
		
		for( PatchInfo& p: patches)
		{

		}

		top->remove();
		soundengine_ref.deleteModuleInstance(module_ref.getNodeID());
	}

	glv::View& getView(void) { return *top; }

	void addInlets(void)
	{
		inlets = new Inlets(*this, 15);
		inlets->pos(glv::Place::TL, 0, 0);
		(*mBox) << inlets;
	}

	void addOutlets(void)
	{	
		outlets = new Outlets(soundengine_ref, *this, 15);
		outlets->pos(glv::Place::TL, 0, 0);
		(*mBox) << outlets;
	}

	void addName(std::string& moduleName, bool show_node_id=false)
	{
		if(show_node_id)
			name_label.reset( new glv::Label(moduleName+" NodeID: " +std::to_string(module_ref.getNodeID())) );
		else
			name_label.reset( new glv::Label(moduleName) );
		name_label->pos(10, 0);
		(*mBox) << name_label.get();
	}


	void addParameters(void)
	{
		for( int i=0; i < module_ref.numParams(); ++i)
		{
			addParameter(module_ref.parameter(i));
		}
	}

	void addPatch(PatchInfo& p)
	{
		patches.push_back(p);
	}


	void addParameter(al::Parameter& parameter)
	{
		int numInt = 2 + ceil(log10(fmax(fabs(parameter.max()), fabs(parameter.min()))));
		glv::NumberDialer *number  = new glv::NumberDialer(numInt, 7 - numInt,
		                                                   parameter.max(),
		                                                   parameter.min());
		number->setValue(parameter.get());
		WidgetWrapper *wrapper = new WidgetWrapper;
		wrapper->parameter = &parameter;
		wrapper->lock = &mParameterGUILock;
		wrapper->widget = static_cast<glv::Widget *>(number);
		mWrappers.push_back(wrapper);
		// number->attach(ParameterGUI::widgetChangeCallback, glv::Update::Value, wrapper);

		glv::Box *box = new glv::Box;
		*box << number << new glv::Label(parameter.getName());
		box->fit();
		(*mBox) << box;
		mBox->fit();
		// mBox.fit();
		// parameter.registerChangeCallback(ParameterGUI::valueChangedCallback, wrapper);

		// return *this;
	}

	static void widgetChangeCallback(const glv::Notification& n) {
		glv::Widget &sender = *n.sender<glv::Widget>();
		WidgetWrapper &receiver = *n.receiver<WidgetWrapper>();
		receiver.lock->lock();
		double value = sender.getValue<double>();
		receiver.lock->unlock();
		receiver.parameter->setNoCalls(value, &sender);
	}

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
	std::vector<PatchInfo> patches;
	al::Module& module_ref;
	al::SoundEngine& soundengine_ref;
	Inlets* inlets;
	Outlets* outlets;
	const int pad = 10;
	/// @brief container for patching information to be passed in the event of a patch
	// const int padTop = 10;
	// const int padBottom = 10;

	ModuleGUIKeyDownEvent moduleGUIKeyDownEvent;
	unique_ptr<glv::View> top;
	unique_ptr<glv::Box> mBox;
	unique_ptr<glv::Label> name_label;
	std::mutex mParameterGUILock;
	std::vector<WidgetWrapper *> mWrappers;
};

}; // namespace al

#endif //AL_MODULE_GUI_HPP