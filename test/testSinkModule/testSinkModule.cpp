#include "testSinkModule.h"
#include "SinkModuleOne.h"

void dummyCB(al::AudioIOData& io)
{

}

SinkModuleTest::SinkModuleTest(void) {}

SinkModuleTest::~SinkModuleTest(void) {}

void SinkModuleTest::SetUp(void) {}

void SinkModuleTest::TearDown(void) {}

TEST_F(SinkModuleTest, constructor_test)
{
	SinkModuleOne m2(1,2), m3(2, 3), m5(3, 5);

	EXPECT_EQ(m2.numInlets(), 1);
	EXPECT_EQ(m2.numOutlets(), 0);
	EXPECT_EQ(m2.numParams(), 2);

	EXPECT_EQ(m3.numInlets(), 2);
	EXPECT_EQ(m3.numOutlets(), 0);
	EXPECT_EQ(m3.numParams(), 3);

	EXPECT_EQ(m5.numInlets(), 3);
	EXPECT_EQ(m5.numOutlets(),0);
	EXPECT_EQ(m5.numParams(), 5);

	EXPECT_THROW( m2.getOutlet(0), std::range_error );
	EXPECT_THROW( m3.getOutlet(0), std::range_error );
	EXPECT_THROW( m5.getOutlet(0), std::range_error );
}

TEST_F(SinkModuleTest, isRunning)
{
	SinkModuleOne m(2, 1);

	EXPECT_FALSE(m.isRunning());
	m.run();
	EXPECT_TRUE(m.isRunning());
	m.stop();
	EXPECT_FALSE(m.isRunning());

	EXPECT_FALSE(m.isRunning());
	m.run();
	EXPECT_TRUE(m.isRunning());
	m.run();
	EXPECT_TRUE(m.isRunning());
}

TEST_F(SinkModuleTest, checkIOcompatibility)
{
	SinkModuleOne m41(2, 1);

	{
		al::AudioIO dummyIO(256, 48000.0, dummyCB, NULL, 12, 0);
		EXPECT_NO_THROW( m41.checkIOcompatibility(dummyIO) );
	}

	{	
		al::AudioIO dummyIO(256, 48000.0, dummyCB, NULL, 2, 0, al::AudioIO::DUMMY);
		EXPECT_THROW( m41.checkIOcompatibility(dummyIO) , std::runtime_error);
	}

	{	
		al::AudioIO dummyIO(256, 44100.0, dummyCB, NULL, 12, 0, al::AudioIO::DUMMY);
		EXPECT_THROW( m41.checkIOcompatibility(dummyIO) , std::runtime_error);
	}

	{	
		al::AudioIO dummyIO(32, 48000.0, dummyCB, NULL, 12, 0, al::AudioIO::DUMMY);
		EXPECT_THROW( m41.checkIOcompatibility(dummyIO) , std::runtime_error);
	}
}