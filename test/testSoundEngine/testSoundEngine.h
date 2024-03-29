#include "gtest/gtest.h"
#include "allolithe/al_SoundEngine.hpp"
#include "allolithe/al_Exceptions.hpp"

// The fixture for testing class Block.
class SoundEngineTest : public ::testing::Test {

protected:

    // You can do set-up work for each test here.
    SoundEngineTest();
    
    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~SoundEngineTest();

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp();

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown();

    al::SoundEngine se_register_test;
    al::SoundEngine se_sink_set_test;
    al::SoundEngine se_module_info_test;
    al::SoundEngine se_module_instantiation_test;
    al::SoundEngine se_module_delete_test;
    al::SoundEngine se_patch_unpatch_test;
};
