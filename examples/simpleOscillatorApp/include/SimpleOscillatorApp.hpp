#include <iostream>

#include "allocore/io/al_App.hpp"
#include "Oscillator.hpp"

using namespace al;

// We inherit from App to create our own custom application
class MyApp : public App{
public:
	Oscillator osc ;
	ParameterGUI gui;

	Mesh mesh;
	double phase;

	// This constructor is where we initialize the application
	MyApp(): phase(0){

		// Add a unit sphere to the mesh
		addSphere(mesh);

		// Configure the camera lens
		lens().near(0.1).far(25).fovy(45);

		// Set navigation position and orientation
		nav().pos(0,0,4);
		nav().quat().fromAxisAngle(0.*M_2PI, 0,1,0);

		// Initialize a single window; anything in App::onDraw will be rendered
		// Arguments: position/dimensions, title, frames/second
		initWindow(Window::Dim(0,0, 600,400), "Simple App", 40);

		// This disables the default navigation keyboard/mouse controls;
		// leaving this enabled may cause some of the user input callbacks to
		// not trigger, such as onMouseDrag.
		//window().remove(navControl());

		// Set background color (default is black)
		background(HSV(0.5, 1, 0.5));

		// Initialize audio so that App::onSound is called
		// Arguments: sample rate (Hz), block size, output channels, input channels
		initAudio(44100, 128, 2, 1);
		gam::sampleRate(44100);
		
		gui.setParentApp(this);
		gui << osc.parameter(0) << osc.parameter(1) << osc.parameter(2) << osc.parameter(3) ;

	}


	// This is the audio callback
	void onSound(AudioIOData& io){

		// Things here occur at block rate...

		// This is the sample loop
		while(io()){
			//float in = io.in(0);
			osc.Process();
			lithe::Sample sample = osc.getOutlet(0).getSample();

			float out1 = sample.audio * sample.az;
			float out2 = sample.audio * sample.el;

			lithe::Node::resetAll_ProcessState();

			io.out(0) = out1;
			io.out(1) = out2;
		}
	}


	// This is the application's (graphical) model update.
	// This is called once for each frame of graphics. Typically, you will
	// update your application's geometry, physics, etc. here.
	void onAnimate(double dt){
		// The phase will ramp from 0 to 1 over 10 seconds. We will use it to
		// animate the sphere.
		double period = 10;
		phase += dt / period;
		if(phase >= 1.) phase -= 1.;
	}


	// This is the application's view update.
	// This is called one or more times per frame, for each window, viewport,
	// and eye (for stereoscopic). Typically, this is where you instruct the
	// GPU to render something.
	void onDraw(Graphics& g){

		// Note: we don't need to do all the normal graphics setup as this
		// is handled by the App's stereographic object (App::stereo()).
		// We can just draw our geometry immediately!

		g.polygonMode(Graphics::LINE); // wireframe mode
		g.pushMatrix();
		g.rotate(phase*360, 0,1,0);
		g.draw(mesh);
		g.popMatrix();
	}


	// This is called whenever a key is pressed.
	void onKeyDown(const Keyboard& k){

		// Use a switch to do something when a particular key is pressed
		switch(k.key()){

		// For printable keys, we just use its character symbol:
		case '1': printf("Pressed 1.\n"); break;
		case 'y': printf("Pressed y.\n"); break;
		case 'n': printf("Pressed n.\n"); break;
		case '.': printf("Pressed period.\n"); break;
		case ' ': printf("Pressed space bar.\n"); break;

		// For non-printable keys, we have to use the enums described in the
		// Keyboard class:
		case Keyboard::RETURN: printf("Pressed return.\n"); break;
		case Keyboard::DELETE: printf("Pressed delete.\n"); break;
		case Keyboard::F1: printf("Pressed F1.\n"); break;
		}
	}

	// This is called whenever a mouse button is pressed.
	void onMouseDown(const Mouse& m){
		switch(m.button()){
		case Mouse::LEFT: printf("Pressed left mouse button.\n"); break;
		case Mouse::RIGHT: printf("Pressed right mouse button.\n"); break;
		case Mouse::MIDDLE: printf("Pressed middle mouse button.\n"); break;
		}
	}

	// This is called whenever the mouse is dragged.
	void onMouseDrag(const Mouse& m){
		// Get mouse coordinates, in pixels, relative to top-left corner of window
		int x = m.x();
		int y = m.y();
		printf("Mouse dragged: %3d, %3d\n", x,y);
	}

	// *****************************************************
	// NOTE: check the App class for more callback functions
};