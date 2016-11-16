#include "testSoundEngine.h"

#include "ModuleOne.hpp"
#include "ModuleTwo.hpp"
#include "SinkOne.hpp"
#include "SinkTwo.hpp"

#include "MockModule.hpp"
// #include "DummyModule.hpp"
// #include "DummySink.hpp"
// #include "PatchingtestModules.hpp"

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
	al::ModuleInfo moduleOneInfo = se_register_test.RegisterModule<ModuleOne>("ModuleOne", false);
	EXPECT_EQ(moduleOneInfo.moduleID, 1000);
	EXPECT_TRUE("ModuleOne" == moduleOneInfo.moduleName);
	EXPECT_EQ( moduleOneInfo.isASink, false);

	al::ModuleInfo moduleTwoInfo = se_register_test.RegisterModule("ModuleTwo", ModuleTwoFactory, false);
	EXPECT_EQ(moduleTwoInfo.moduleID, 1001);
	EXPECT_TRUE("ModuleTwo" == moduleTwoInfo.moduleName);
	EXPECT_EQ( moduleTwoInfo.isASink, false);

	al::ModuleInfo sinkOneInfo = se_register_test.RegisterModule<SinkOne>("SinkOne", true);
	EXPECT_EQ(sinkOneInfo.moduleID, 1002);
	EXPECT_TRUE("SinkOne" == sinkOneInfo.moduleName);
	EXPECT_EQ( sinkOneInfo.isASink, true);

	al::ModuleInfo sinkTwoInfo = se_register_test.RegisterModule("SinkTwo", SinkTwoFactory, true);
	EXPECT_EQ(sinkTwoInfo.moduleID, 1003);
	EXPECT_TRUE("SinkTwo" == sinkTwoInfo.moduleName);
	EXPECT_EQ( sinkTwoInfo.isASink, true);
}

TEST_F(SoundEngineTest, setgetSink)
{
	al::ModuleInfo sink_module = se_sink_set_test.RegisterModule<SinkOne>("SinkOne", true);
	EXPECT_THROW( se_sink_set_test.getSink(), al::SinkNotSetException);
	al::NodeInfo sink = se_sink_set_test.instantiateModule(sink_module.moduleID);
	se_sink_set_test.setSink(sink.nodeID, sink.moduleID);
	EXPECT_NO_THROW( se_sink_set_test.getSink());

	se_sink_set_test.setAndInstantiateSink(sink.moduleID);
	EXPECT_NO_THROW( se_sink_set_test.getSink());
}

TEST_F(SoundEngineTest, getModuleInfo)
{
	se_module_info_test.RegisterModule<ModuleOne>("ModuleOne", false);
	se_module_info_test.RegisterModule("ModuleTwo", ModuleTwoFactory, false);
	se_module_info_test.RegisterModule<SinkOne>("SinkOne", true);
	se_module_info_test.RegisterModule("SinkTwo", SinkTwoFactory, true);

	al::ModuleInfo moduleOneInfo = se_module_info_test.getModuleInfo(1000);
	EXPECT_EQ(moduleOneInfo.moduleID, 1000);
	EXPECT_TRUE("ModuleOne" == moduleOneInfo.moduleName);
	EXPECT_EQ( moduleOneInfo.isASink, false);

	al::ModuleInfo moduleTwoInfo = se_module_info_test.getModuleInfo(1001);
	EXPECT_EQ(moduleTwoInfo.moduleID, 1001);
	EXPECT_TRUE("ModuleTwo" == moduleTwoInfo.moduleName);
	EXPECT_EQ( moduleTwoInfo.isASink, false);

	al::ModuleInfo sinkOneInfo = se_module_info_test.getModuleInfo(1002);
	EXPECT_EQ(sinkOneInfo.moduleID, 1002);
	EXPECT_TRUE("SinkOne" == sinkOneInfo.moduleName);
	EXPECT_EQ( sinkOneInfo.isASink, true);

	al::ModuleInfo sinkTwoInfo = se_module_info_test.getModuleInfo(1003);
	EXPECT_EQ(sinkTwoInfo.moduleID, 1003);
	EXPECT_TRUE("SinkTwo" == sinkTwoInfo.moduleName);
	EXPECT_EQ( sinkTwoInfo.isASink, true);
}


TEST_F(SoundEngineTest, instantiateModule )
{
	se_module_instantiation_test.RegisterModule<ModuleOne>("ModuleOne", false);
	se_module_instantiation_test.RegisterModule("ModuleTwo", ModuleTwoFactory, false);
	se_module_instantiation_test.RegisterModule<SinkOne>("SinkOne", true);
	se_module_instantiation_test.RegisterModule("SinkTwo", SinkTwoFactory, true);


	int i = 2;// se_module_instantiation_test.numLitheNodes();

	al::NodeInfo node = se_module_instantiation_test.instantiateModule(1000);
	EXPECT_EQ(node.nodeID, i);
	EXPECT_EQ(node.moduleID, 1000);
	node = se_module_instantiation_test.instantiateModule(1001);
	EXPECT_EQ(node.nodeID, 1010);	// This is hard coded to verify that the factory function is being called.
	EXPECT_EQ(node.moduleID, 1001);

	al::NodeInfo sink = se_module_instantiation_test.instantiateModule(1002);
	EXPECT_EQ(sink.nodeID, i+1);
	EXPECT_EQ(sink.moduleID, 1002);
	sink = se_module_instantiation_test.instantiateModule(1003);
	EXPECT_EQ(sink.nodeID, 2020);	// This is hard coded to verify that the factory function is being called.
	EXPECT_EQ(sink.moduleID, 1003);

	EXPECT_THROW(se_module_instantiation_test.instantiateModule(1010), al::ModuleNotRegisteredException);
	EXPECT_THROW(se_module_instantiation_test.instantiateModule(1110), al::ModuleNotRegisteredException);
	EXPECT_THROW(se_module_instantiation_test.instantiateModule(1020), al::ModuleNotRegisteredException);
}

TEST_F(SoundEngineTest, deleteModuleInstance )
{
	se_module_delete_test.RegisterModule<ModuleOne>("ModuleOne", false);
	se_module_delete_test.RegisterModule<SinkOne> ("SinkOne" , true);
	// EXPECT_NO_THROW();
	al::NodeInfo node = se_module_delete_test.instantiateModule(1000);
	al::NodeInfo sink = se_module_delete_test.instantiateModule(1001);

	EXPECT_NO_THROW( se_module_delete_test.deleteModuleInstance(node.nodeID) );
	EXPECT_THROW( al::Module::getModuleRef(node.nodeID), al::NodeNotFoundException); 

	// No problem deleting when not actively used by sound engine.
	EXPECT_NO_THROW(se_module_delete_test.deleteModuleInstance(sink.nodeID));
	
	sink = se_module_delete_test.instantiateModule(1001);
	se_module_delete_test.setSink(sink.nodeID, sink.moduleID); // When used by sound engine
	EXPECT_THROW(se_module_delete_test.deleteModuleInstance(sink.nodeID), al::CannotDeleteActiveSinkException);

	EXPECT_THROW(se_module_delete_test.deleteModuleInstance(sink.nodeID+10), al::NodeNotFoundException);
}

TEST_F(SoundEngineTest, patching_and_unpatching)
{
	// TODO
	se_patch_unpatch_test.RegisterModule<ModuleOne>("ModuleOne", false);
	se_patch_unpatch_test.RegisterModule<SinkOne> ("SinkOne" , true);

	al::NodeInfo module = se_patch_unpatch_test.instantiateModule(1000);
	al::NodeInfo another_module = se_patch_unpatch_test.instantiateModule(1000);
	al::NodeInfo sink = se_patch_unpatch_test.instantiateModule(1001);


	EXPECT_NO_THROW( se_patch_unpatch_test.patch(sink, 0, module, 0) );
	EXPECT_NO_THROW( se_patch_unpatch_test.unpatch(sink, 0, module, 0) );

	EXPECT_NO_THROW( se_patch_unpatch_test.patch(another_module, 0, module, 0) ) ;
	EXPECT_NO_THROW( se_patch_unpatch_test.unpatch(another_module, 0, module, 0) ) ;

	EXPECT_THROW( se_patch_unpatch_test.patch(sink, 0, module, 1), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.patch(sink, 123, module, 1), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.patch(sink, 5, module, 2), al::PatchingException);
	
	EXPECT_THROW( se_patch_unpatch_test.unpatch(sink, 2, another_module, 0), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.unpatch(sink, 0, module, 1), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.unpatch(sink, 1, module, 1), al::PatchingException);

	se_patch_unpatch_test.setSink(sink.nodeID, 1001);
	se_patch_unpatch_test.getSink().run();

	EXPECT_THROW( se_patch_unpatch_test.patch(sink, 0, module, 0), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.unpatch(sink, 0, module, 0), al::PatchingException);

	EXPECT_THROW( se_patch_unpatch_test.patch(another_module, 0, module, 0), al::PatchingException) ;
	EXPECT_THROW( se_patch_unpatch_test.unpatch(another_module, 0, module, 0), al::PatchingException) ;

	EXPECT_THROW( se_patch_unpatch_test.patch(sink, 0, module, 1), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.patch(sink, 123, module, 1), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.patch(sink, 5, module, 2), al::PatchingException);
	
	EXPECT_THROW( se_patch_unpatch_test.unpatch(sink, 2, another_module, 0), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.unpatch(sink, 0, module, 1), al::PatchingException);
	EXPECT_THROW( se_patch_unpatch_test.unpatch(sink, 1, module, 1), al::PatchingException);
}