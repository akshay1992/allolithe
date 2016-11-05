#include "testModule.h"
#include "allolithe/al_allolithe.hpp"
#include "ModuleOne.h"

ModuleTest::ModuleTest(void) {}

ModuleTest::~ModuleTest(void) {}

void ModuleTest::SetUp(void) {
}

void ModuleTest::TearDown(void) {}

TEST_F(ModuleTest, constructor_test)
{
	ModuleOne m2(1,1, 2), m3(2,2, 3), m5(3,2, 5);

	EXPECT_EQ(m2.numInlets(), 1);
	EXPECT_EQ(m2.numOutlets(), 1);
	EXPECT_EQ(m2.numParams(), 2);

	EXPECT_EQ(m3.numInlets(), 2);
	EXPECT_EQ(m3.numOutlets(), 2);
	EXPECT_EQ(m3.numParams(), 3);

	EXPECT_EQ(m5.numInlets(), 3);
	EXPECT_EQ(m5.numOutlets(),2);
	EXPECT_EQ(m5.numParams(), 5);
}

TEST_F(ModuleTest, instantiate_parameters)
{
	ModuleOne m2(1,1, 2);

	EXPECT_FALSE( m2.paramsInstnatiated() );
	m2.instantiate_parameters();
	EXPECT_TRUE( m2.paramsInstnatiated() );

	EXPECT_NO_THROW( m2.parameter(0) );
	EXPECT_NO_THROW( m2.parameter(1) );
	
	EXPECT_FLOAT_EQ(m2.parameter(0).get(), 0.0); 	// default value
	EXPECT_FLOAT_EQ(m2.parameter(1).get(), 1.0);	// default value
	
	EXPECT_THROW( m2.parameter(2), std::range_error );
	EXPECT_THROW( m2.parameter(-1), std::range_error );
}

TEST_F(ModuleTest, getModuleRef)
{
	ModuleOne m3(2,2, 3);
	int id = m3.getNodeID();

	al::Module& m = al::Module::getModuleRef(id);
	EXPECT_EQ( &m3, &m);
	EXPECT_EQ(m.numInlets(), 2);
	EXPECT_EQ(m.numOutlets(), 2);
	EXPECT_EQ(m.numParams(), 3);
}

TEST_F(ModuleTest, get_set_moduleName)
{
	ModuleOne m2(2,2,1);
	ASSERT_STREQ( m2.getName().c_str(), "");
	m2.setName("myModule");
	ASSERT_STREQ( m2.getName().c_str(), "myModule");
}