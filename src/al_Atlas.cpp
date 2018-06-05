#include "../allolithe/al_Atlas.hpp"

namespace al {

PiecewiseLinearScaler::PiecewiseLinearScaler(float nearClipMul, float farClipMul)
	: near(nearClipMul), far(farClipMul) 
{
	// ..crickets	
}

float PiecewiseLinearScaler::getMul(float d)
{
	if( d == 0 ) 
		return 1.0;
	else if( d < 0) 
		return (1-far)*d + 1.0;
	else// (d > 0)
		return (near-1)*d + 1.0;
}

float PiecewiseLinearScaler::get_d(float mul)
{
	if (mul == 1) return 0.0;
	else if( mul > 1)
		return (mul-1.0) / (1-far);
	else// ( mul < 1 )
		return (mul-1.0) / (near-1);
}


SphericalAtlas::SphericalAtlas( float radius, lithe::RangeT<float> u_range, lithe::RangeT<float> v_range)
{
	mAtlas = lithe::SphericalAtlas(radius, u_range, v_range);
}

al::Vec3d SphericalAtlas::getXYZ(float u, float v, float d) 
{
	return NormalVector(u, v) * -1 * scaler.getMul(d);
}

al::Vec3d SphericalAtlas::getXYZ(lithe::Sample& s) 
{
	return NormalVector(s.az, s.el) * -1 * scaler.getMul(s.d);
}

al::Vec3d SphericalAtlas::getUVD(float x, float y, float z)
{
	float mul = lithe::SphericalAtlas::radius(x, y, z) / mAtlas.getRadius(); 
	float x_mul = x / mul;
	float y_mul = y / mul;
	float z_mul = z / mul;
	return al::Vec3d( mAtlas.u_cart(x_mul, y_mul, z_mul), mAtlas.v_cart(x_mul, y_mul, z_mul), scaler.get_d(mul) );
}

al::Vec3d SphericalAtlas::getUVD(al::Vec3d& xyz) 
{
	return getUVD(xyz[0], xyz[1], xyz[2]);
}

al::Vec3d SphericalAtlas::NormalVector(float u, float v)
{
	// vector points towards Positive D (in this case, towards centre of sphere)
	return al::Vec3d( -1*mAtlas.x(u, v), -1*mAtlas.y(u, v), -1*mAtlas.z(u, v));
}

ToricAtlas::ToricAtlas( float R, float r, lithe::RangeT<float> u_range, lithe::RangeT<float> v_range)
{
	mAtlas = lithe::ToricAtlas(R, r, u_range, v_range);
}

al::Vec3d ToricAtlas::getXYZ(float u, float v, float d) 
{
	// return NormalVector(u, v) * -1 * scaler.getMul(d);
	return al::Vec3d(mAtlas.x(u, v), mAtlas.y(u, v), mAtlas.z(u, v) );
}

al::Vec3d ToricAtlas::getXYZ(lithe::Sample& s) 
{
	// return NormalVector(s.az, s.el) * -1 * scaler.getMul(s.d);
	return al::Vec3d(mAtlas.x(s[1], s[2]), mAtlas.y(s[1], s[2]), mAtlas.z(s[1], s[2]) );
}

al::Vec3d ToricAtlas::getUVD(float x, float y, float z)
{
	// float mul = lithe::SphericalAtlas::radius(x, y, z) / mAtlas.getRadius(); 
	// float x_mul = x / mul;
	// float y_mul = y / mul;
	// float z_mul = z / mul;
	// return al::Vec3d( mAtlas.u_cart(x_mul, y_mul, z_mul), mAtlas.v_cart(x_mul, y_mul, z_mul), scaler.get_d(mul) );
	return al::Vec3d();
}

al::Vec3d ToricAtlas::getUVD(al::Vec3d& xyz) 
{
	// return getUVD(xyz[0], xyz[1], xyz[2]);
	return al::Vec3d();
}

al::Vec3d ToricAtlas::NormalVector(float u, float v)
{
	// vector points towards Positive D (in this case, towards centre of sphere)
	return al::Vec3d( -1*mAtlas.x(u, v), -1*mAtlas.y(u, v), -1*mAtlas.z(u, v));
}


};