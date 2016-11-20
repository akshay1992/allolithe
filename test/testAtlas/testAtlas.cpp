#include "testAtlas.h"
#include "allolithe/al_Atlas.hpp"
#include <vector>

AtlasTest::AtlasTest(void) {}

AtlasTest::~AtlasTest(void) {}

void AtlasTest::SetUp(void) {}

void AtlasTest::TearDown(void) {}

TEST_F(AtlasTest, sphericalAtlas_constructor)
{
	/// TODO TEST THE RANGES TOO
	al::SphericalAtlas atlas;
	EXPECT_FLOAT_EQ(atlas.mAtlas.getRadius(), 1.0);

	atlas = al::SphericalAtlas(12);
	EXPECT_FLOAT_EQ(atlas.mAtlas.getRadius(), 12.0);

	EXPECT_THROW(	atlas = al::SphericalAtlas(-1), std::range_error);
	EXPECT_THROW(	atlas = al::SphericalAtlas(0), std::range_error);
}


TEST_F( AtlasTest, sphericalAtlas_RadiusScaler) 
{
	al::SphericalAtlas atlas;
	std::vector<double> inputs = {-1.  , -0.77, -0.54, -0.31, -0.08,  0.15,  0.38,  0.61,  0.84};
	std::vector<double> outputs = {2.0, 1.77, 1.54, 1.3100000000000001, 1.0800000000000001, 0.85000000000000009, 0.62000000000000011, 0.39000000000000012, 0.16000000000000014};

	for(int i=0; i<inputs.size(); ++i)
	{
		EXPECT_FLOAT_EQ( atlas.scaler.getMul(inputs[i]), outputs[i]);
	}
}

TEST_F( AtlasTest, sphericalAtlas_getXYZ)
{
	al::SphericalAtlas atlas;
	// TODO
}
