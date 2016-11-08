#ifndef MOCK_MODULE_HPP
#define MOCK_MODULE_HPP

class MockModule
{
public:
	int getNodeID() { return id;}
	static int id;
};

int MockModule::id;

int MockFactory(void)
{
	static int i;
	return i++;
}


#endif // MOCK_MODULE_HPP