#include "testSoundEngine.h"
#include "allolithe/al_SoundEngine.hpp"
#include "allolithe/al_Exceptions.hpp"

#include "MockModule.hpp"
#include "DummyModule.hpp"
#include "DummySink.hpp"
#include "PatchingtestModules.hpp"

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
	int module = PatchingTest_SoundEngine.instantiateModule(Module1::moduleID);
	int another_module = PatchingTest_SoundEngine.instantiateModule(Module1::moduleID);
	int sink = PatchingTest_SoundEngine.instantiateModule(Sink1::moduleID);

	EXPECT_NO_THROW( PatchingTest_SoundEngine.patch(sink, 0, module, 0) );
	EXPECT_NO_THROW( PatchingTest_SoundEngine.unpatch(sink, 0, module, 0) );

	EXPECT_NO_THROW( PatchingTest_SoundEngine.patch(another_module, 0, module, 0) ) ;
	EXPECT_NO_THROW( PatchingTest_SoundEngine.unpatch(another_module, 0, module, 0) ) ;

	EXPECT_THROW( PatchingTest_SoundEngine.patch(sink, 0, module, 1), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.patch(sink, 123, module, 1), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.patch(sink, 5, module, 2), al::PatchingException);
	
	EXPECT_THROW( PatchingTest_SoundEngine.unpatch(sink, 2, another_module, 0), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.unpatch(sink, 0, module, 1), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.unpatch(sink, 1, module, 1), al::PatchingException);

	PatchingTest_SoundEngine.setSink(sink, Sink1::moduleID);
	PatchingTest_SoundEngine.getSink().run();

	EXPECT_THROW( PatchingTest_SoundEngine.patch(sink, 0, module, 0), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.unpatch(sink, 0, module, 0), al::PatchingException);

	EXPECT_THROW( PatchingTest_SoundEngine.patch(another_module, 0, module, 0), al::PatchingException) ;
	EXPECT_THROW( PatchingTest_SoundEngine.unpatch(another_module, 0, module, 0), al::PatchingException) ;

	EXPECT_THROW( PatchingTest_SoundEngine.patch(sink, 0, module, 1), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.patch(sink, 123, module, 1), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.patch(sink, 5, module, 2), al::PatchingException);
	
	EXPECT_THROW( PatchingTest_SoundEngine.unpatch(sink, 2, another_module, 0), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.unpatch(sink, 0, module, 1), al::PatchingException);
	EXPECT_THROW( PatchingTest_SoundEngine.unpatch(sink, 1, module, 1), al::PatchingException);
}

TEST_F(SoundEngineTest, setgetSink)
{
	EXPECT_THROW( al::DefaultSoundEngine().getSink(), al::SinkNotSetException);
	int sink = al::DefaultSoundEngine().instantiateModule(DummySink::moduleID);
	al::DefaultSoundEngine().setSink(sink, DummySink::moduleID);
	EXPECT_NO_THROW( al::DefaultSoundEngine().getSink());

	al::DefaultSoundEngine().setAndInstantiateSink(DummySink::moduleID);
	EXPECT_NO_THROW( al::DefaultSoundEngine().getSink());
}