#ifndef AL_ATLAS_HPP
#define AL_ATLAS_HPP

#include "LitheCore.h"
#include "al/core.hpp"

namespace al{

/** @brief Wraps a Lithe atlas to implement projection along surface of atlas

	This allows for mapping of co-ordinates projected on either side of the
	manifold. Requires implementation of a function to compute the normal vector.
*/
template< typename ATLAS > 
class AtlasWrapper
{
public:
	ATLAS mAtlas;
	virtual al::Vec3d getXYZ(float u, float v, float d)=0;
	virtual al::Vec3d getXYZ(lithe::Sample& s)=0;

	virtual al::Vec3d getUVD(float x, float y, float z)=0;
	virtual al::Vec3d getUVD(al::Vec3d& xyz)=0;

	virtual al::Vec3d NormalVector(float u, float v)=0;
};


/** @brief Used to scale radius based on a distance parameter ranging [-1, 1]

	@param [nearClipMul] is the multiplier returned at d = +1
	@param [farClipMul] is the multiplier returned at d = -1

	Returns a multiplier 1.0 at d = 0, and nearClipMul and farClipMul at d = +1, and d = -1 respectively
	This is mapped as two line segments, one for d > 0 and one for d < 0
*/
class PiecewiseLinearScaler
{
public:
	PiecewiseLinearScaler(float nearClipMul=0.0, float farClipMul=2);

	float getMul(float d);

	float get_d(float mul);

private:
	float near;
	float far;
};

//-------------------------SPHERICAL ATLAS------------------------------------

/** @brief A spherical atlas that maps distance as projected on the surface

*/ 

class SphericalAtlas : public AtlasWrapper<lithe::SphericalAtlas>
{ 
public:
	SphericalAtlas( float radius=1, lithe::RangeT<float> u_range = lithe::RangeT<float>(-1,1), lithe::RangeT<float> v_range = lithe::RangeT<float>(-1, 1));

	virtual al::Vec3d getXYZ(float u, float v, float d) override;

	virtual al::Vec3d getXYZ(lithe::Sample& s) override;

	virtual al::Vec3d getUVD(float x, float y, float z) override;
	
	virtual al::Vec3d getUVD(al::Vec3d& xyz) override;

	/// @brief returns the normal vector to the surface of the sphere at that point (pointing towards the centre of the sphere)
	virtual al::Vec3d NormalVector(float u, float v) override;

private:	
	/// @brief used to modulate the distance based on the d signal
	PiecewiseLinearScaler scaler;
};



//-------------------------TORIC ATLAS------------------------------------

class ToricAtlas : public AtlasWrapper<lithe::ToricAtlas>
{ 
public:
	ToricAtlas( float R, float r, lithe::RangeT<float> u_range = lithe::RangeT<float>(-1,1), lithe::RangeT<float> v_range = lithe::RangeT<float>(-1, 1));

	virtual al::Vec3d getXYZ(float u, float v, float d) override;

	virtual al::Vec3d getXYZ(lithe::Sample& s) override;

	virtual al::Vec3d getUVD(float x, float y, float z) override;
	
	virtual al::Vec3d getUVD(al::Vec3d& xyz) override;

	/// @brief returns the normal vector to the surface of the sphere at that point (pointing towards the centre of the sphere)
	virtual al::Vec3d NormalVector(float u, float v) override;

private:	
	/// @brief used to modulate the distance based on the d signal
	PiecewiseLinearScaler scaler;
};

}; // namespace al


#endif // AL_ATLAS_HPP