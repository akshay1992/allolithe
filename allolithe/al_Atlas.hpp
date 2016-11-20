#ifndef AL_ATLAS_HPP
#define AL_ATLAS_HPP

#include "Lithe/Atlas.h"
#include "Lithe/SphericalAtlas.h"
#include "Lithe/Sample.h"

#include "allocore/math/al_Vec.hpp"
#include "allocore/spatial/al_DistAtten.hpp"

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

//-------------------------SPHERICAL ATLAS------------------------------------

/// @brief Used to scale radius differently for into/outof the spherical surface
class PiecewiseRadiusScaler
{
public:
	PiecewiseRadiusScaler(float nearClipMul=0.0, float farClipMul=2)
	: near(nearClipMul), far(farClipMul) {}

	float getMul(float d)
	{
		if( d == 0 ) return 1.0;
		else if( d < 0) 
			return (1-far)*d + 1.0;
		else if(d > 0)
			return (near-1)*d + 1.0;
	}

	float get_d(float mul)
	{
		if (mul == 1) return 0.0;
		else if( mul > 1)
			return (mul-1.0) / (1-far);
		else if ( mul < 1 )
			return (mul-1.0) / (near-1);
	}

	float near;
	float far;
};

class SphericalAtlas : public AtlasWrapper<lithe::SphericalAtlas>
{ 
public:
	SphericalAtlas( float radius=1, lithe::RangeT<float> u_range = lithe::RangeT<float>(-1,1), lithe::RangeT<float> v_range = lithe::RangeT<float>(-1, 1))
	{
		mAtlas = lithe::SphericalAtlas(radius, u_range, v_range);
	}

	virtual al::Vec3d getXYZ(float u, float v, float d) override 
	{
		return NormalVector(u, v) * -1 * scaler.getMul(d);
	}

	virtual al::Vec3d getXYZ(lithe::Sample& s) override 
	{
		return NormalVector(s.az, s.el) * -1 * scaler.getMul(s.d);
	}

	virtual al::Vec3d getUVD(float x, float y, float z) override
	{
		float mul = lithe::SphericalAtlas::radius(x, y, z) / mAtlas.getRadius(); 
		float x_mul = x / mul;
		float y_mul = y / mul;
		float z_mul = z / mul;
		return al::Vec3d( mAtlas.u_cart(x_mul, y_mul, z_mul), mAtlas.v_cart(x_mul, y_mul, z_mul), scaler.get_d(mul) );
	}
	
	virtual al::Vec3d getUVD(al::Vec3d& xyz) override 
	{
		return getUVD(xyz[0], xyz[1], xyz[2]);
	}

	virtual al::Vec3d NormalVector(float u, float v) override
	{
		// vector points towards Positive D (in this case, towards centre of sphere)
		return al::Vec3d( -1*mAtlas.x(u, v), -1*mAtlas.y(u, v), -1*mAtlas.z(u, v));
	}
	
	PiecewiseRadiusScaler scaler;
};


}; // namespace al


#endif // AL_ATLAS_HPP