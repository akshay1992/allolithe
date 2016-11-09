#ifndef MOCKSE
#define MOCKSE

#include <iostream>
using namespace std;

class MockSoundEngine
{
public:
	bool isRunning() {return runningState;}
	void run() { runningState = true; cout << "SoundEngine Running" << endl;}
	void stop() { runningState = false; cout << "SoundEngine Stopped" << endl;}

	int instantiateModule(int moduleID) 
	{ 
		cout << "Instantiating Module" << endl;
		if (moduleID==11) 
			return 12; 
		else 
			return -1; 
	}

private:
	bool runningState = false;
};

class MockModule 
{
public:
	static MockModule& getModuleRef(int i)
	{
		if ( i ==12 )
		{		
			MockModule * instance = new MockModule();
			return *instance;
		}
	}

	glv::View& createDefaultView() 
	{ 
		glv::View* inst = new glv::View(glv::Rect(80,80));
		return *inst;
	}
};

#endif // MOCKSE