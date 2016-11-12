#ifndef MOCK_MODULE_HPP
#define MOCK_MODULE_HPP

class MockModule
{
public:
	int getNodeID() { return id;}
	static int id;
};

int MockModule::id;

#endif // MOCK_MODULE_HPP