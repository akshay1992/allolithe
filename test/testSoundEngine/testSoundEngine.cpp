#include "testSoundEngine.h"
#include "allolithe/al_SoundEngine.hpp"
#include "MockModule.hpp"
#include "DummyModule.hpp"
#include "DummySink.hpp"

SoundEngineTest::SoundEngineTest(void) {}

SoundEngineTest::~SoundEngineTest(void) {}

void SoundEngineTest::SetUp(void) {}

void SoundEngineTest::TearDown(void) {}

TEST_F(SoundEngineTest, default_module_factory)
{
	MockModule::id = 1;
	EXPECT_EQ( al::default_module_factory<MockModule>(), 1 );

	MockModule::id = 12;
	EXPECT_EQ( al::default_module_factory<MockModule>(), 12 );
}

TEST_F(SoundEngineTest, RegisterModule)
{
	al::SoundEngine se;
	EXPECT_EQ( se.RegisterModule<MockModule>("MockModule"), 0);
	EXPECT_EQ( se.RegisterModule("MockModule2", MockFactory), 1);
}

TEST_F(SoundEngineTest, DefaultRegisterModule)
{
	int id = al::RegisterModule("MockModule", al::default_module_factory<MockModule>);
	EXPECT_EQ( al::RegisterModule("MockModule2", MockFactory), id+1);
	EXPECT_EQ( al::RegisterModule<MockModule>("MockModule"), id+2);

}

TEST_F(SoundEngineTest, instantiateModule )
{
	int i = al::DefaultSoundEngine().instantiateModule(DummyModule::moduleID);
	EXPECT_EQ( al::DefaultSoundEngine().instantiateModule(DummyModule::moduleID), i+1);
	EXPECT_EQ( al::DefaultSoundEngine().instantiateModule(DummyModule::moduleID), i+2);
	EXPECT_EQ( al::DefaultSoundEngine().instantiateModule(DummyModule::moduleID), i+3);
}


TEST_F(SoundEngineTest, deleteModuleInstance )
{
	EXPECT_NO_THROW(al::Module::getModuleRef(0));
	al::DefaultSoundEngine().deleteModuleInstance(0);
	EXPECT_THROW(al::Module::getModuleRef(0), std::runtime_error);
	
	EXPECT_NO_THROW(al::Module::getModuleRef(1));
	al::DefaultSoundEngine().deleteModuleInstance(1);
	EXPECT_THROW(al::Module::getModuleRef(1), std::runtime_error);
	
	EXPECT_NO_THROW(al::Module::getModuleRef(2));
	al::DefaultSoundEngine().deleteModuleInstance(2);
	EXPECT_THROW(al::Module::getModuleRef(2), std::runtime_error);
}

TEST_F(SoundEngineTest, patching_and_unpatching)
{
	// TODO
}

TEST_F(SoundEngineTest, setgetSink)
{
	EXPECT_THROW( al::DefaultSoundEngine().getSink(), std::runtime_error);
	al::DefaultSoundEngine().setSink(DummySink::moduleID);
	EXPECT_NO_THROW( al::DefaultSoundEngine().getSink());
}

