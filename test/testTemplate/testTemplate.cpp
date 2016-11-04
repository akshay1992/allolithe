#include "testTemplate.h"

TemplateTest::TemplateTest(void) {}

TemplateTest::~TemplateTest(void) {}

void TemplateTest::SetUp(void) {}

void TemplateTest::TearDown(void) {}

TEST_F(TemplateTest, removeThisTest)
{
    EXPECT_EQ(1, 2);
    lithe::SphericalAtlas atlas;
	lithe::Node n(2,2);

}
