#ifndef AL_MODULE_GUI_HPP
#define AL_MODULE_GUI_HPP

#include "GLV/glv.h"
#include "GLV/glv_binding.h"

#include "allolithe/al_Module.hpp"

#include <iostream>
using namespace std;
namespace al{

class Inlets;
class Outlets;

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
	Outlets(al::Module& module, int size = 15) : 
		module(module), 
		mouseUpOutletEvent(*this)
	{
		name("Outlets");
		enable(glv::Property::Controllable);
		disable(glv::DrawGrid);

		int numOutlets = module.numOutlets();
		buttons = new glv::Buttons(glv::Rect(numOutlets*size, size), numOutlets, 1, true, true);
		buttons->addHandler(glv::Event::MouseUp, mouseUpOutletEvent);

		buttons->enable(glv::Property::Controllable);
		buttons->padding(5);

		(*this) << buttons;
		fit();
	}

	virtual void onDraw( glv::GLV& g)
	{
		for(int i=0; i<buttons->size(); ++i)
		{
			if(buttons->getValue(i) == 1)
			{
				selected_outlet = i;
				return;
			}
		}
		selected_outlet = -1;
	}

	al::Module& module;
	int selected_outlet = -1;
	glv::Buttons* buttons;
	MouseUpOutletEvent mouseUpOutletEvent;
};

class Inlets : public glv::View
{
public:
	Inlets(al::Module& module, int size = 15) : 
		module(module), 
		mouseUpInletEvent(*this)
	{
		name("Inlets");
		enable(glv::Property::Controllable);
		disable(glv::DrawGrid);

		int numInlets = module.numInlets();
		buttons = new glv::Buttons(glv::Rect(numInlets*size, size), numInlets, 1, true, false);
		buttons->addHandler(glv::Event::MouseUp, mouseUpInletEvent);

		buttons->enable(glv::Property::Controllable);
		buttons->padding(5);

		cout << "Initializing " << module.numInlets() << endl;
		(*this) << buttons;
		fit();
	}

	virtual void onDraw( glv::GLV& g)
	{
		for(int i=0; i<buttons->size(); ++i)
		{
			if(buttons->getValue(i) == 1)
			{
				selected_inlet = i;
				return;
			}
		}
		selected_inlet = -1;
	}

	al::Module& module;
	int selected_inlet = -1;
	static int last_selected_inlet_index;
	static Inlets* last_selected_inlets_ref;
	glv::Buttons* buttons;
	MouseUpInletEvent mouseUpInletEvent;
};

class ModuleGUI
{
public:

	ModuleGUI(al::Module& module, std::string& moduleName) :
		module_ref(module) 
	{
		mBox = new glv::Box(glv::Direction::S);
		mBox->pos(glv::Place::TL, 10, 0);
		
		addInlets();
		addName(moduleName);
		addParameters();
		addOutlets();

		top = new glv::View(glv::Rect(0, 0, mBox->width() + 2*pad, mBox->height()));
		*top << mBox;

		top->addHandler(glv::Event::MouseDrag, glv::Behavior::mouseMove);
		top->enable(glv::Property::Controllable);
		top->enable(glv::Property::DrawBorder);
	}

	glv::View& getView(void) { return *top; }

	void addInlets(void)
	{
		inlets = new Inlets(module_ref, 15);
		inlets->pos(glv::Place::TL, 0, 0);
		(*mBox) << inlets;
	}

	void addOutlets(void)
	{	
		outlets = new Outlets(module_ref, 15);
		outlets->pos(glv::Place::TL, 0, 0);
		(*mBox) << outlets;
	}

	void addName(std::string& moduleName, bool show_node_id=false)
	{
		if(show_node_id)
			name_label = new glv::Label(moduleName+" NodeID: " +std::to_string(module_ref.getNodeID()));
		else
			name_label = new glv::Label(moduleName);
		name_label->pos(10, 0);
		(*mBox) << name_label;
	}


	void addParameters(void)
	{
		for( int i=0; i < module_ref.numParams(); ++i)
		{
			addParameter(module_ref.parameter(i));
		}
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

	// static void valueChangedCallback(float value, void *sender, void *wrapper, void *blockThis) {
	// 	WidgetWrapper *w = static_cast<WidgetWrapper *>(wrapper);
	// 	if (wrapper != blockThis) {
	// 		w->lock->lock(); // Is the setting of the value from the mouse also protected? Do we need to protect that?
	// 		glv::Data &d = w->widget->data();
	// 		d.assign<double>(value); // We need to assign this way to avoid triggering callbacks.
	// 		w->lock->unlock();
	// 	}
	// }



	struct WidgetWrapper
	{
		ParameterWrapper<float> *parameter;
		glv::Widget *widget;
		glv::Label *label;	// optional
		std::mutex *lock;
	};

public:
	al::Module& module_ref;
	Inlets* inlets;
	Outlets* outlets;
	const int pad = 10;
	// const int padTop = 10;
	// const int padBottom = 10;
	glv::View* top;
	glv::Box* mBox;
	glv::Label* name_label;
	std::mutex mParameterGUILock;
	std::vector<WidgetWrapper *> mWrappers;
};

}; // namespace al

#endif //AL_MODULE_GUI_HPP