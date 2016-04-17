#define _USE_MATH_DEFINES
#include <cmath>
#include "Vector3D.hpp"

Vector3D::Vector3D(double x,double y,double z){
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3D::~Vector3D()
{
}

Vector3D Vector3D::getNormalized()
{
	return Vector3D(x / getMagnitude(), y / getMagnitude(),z/getMagnitude());
}

double Vector3D::dotProduct(Vector3D a, Vector3D b)
{
	return a.x * b.x + a.y *b.y + a.z * b.z;
}

double Vector3D::getAccuteAngle(Vector3D a, Vector3D b)
{
	double angle = std::acos(dotProduct(a, b) / (a.getMagnitude() * b.getMagnitude()));
	return (angle > M_PI_2) ? (angle - M_PI_2) : angle;
}

Vector3D Vector3D::getCrossProduct(Vector3D a, Vector3D b)
{
	return Vector3D(a.y * b.z - a.z*b.y, -(a.x*b.z - b.x * a.z), a.x * b.y - b.x*a.y);
}

double Vector3D::getDistanceSq(Vector3D a, Vector3D b)
{
	return Vector3D(b - a).getMagnitudeSq();
}

double Vector3D::getDistance(Vector3D a, Vector3D b)
{
	return sqrt(getDistanceSq(a, b));
}

std::string Vector3D::toString()
{
	return "X: " + std::to_string(x) + " Y: " + std::to_string(y)+ " Z: " + std::to_string(z);
}


double Vector3D::getMagnitude()
{
	return sqrt(getMagnitudeSq());
}

double Vector3D::getMagnitudeSq()
{
	return pow(x, 2) + pow(y, 2) + pow(z,2);
}

Vector3D operator/(Vector3D & a, double b)
{
	return Vector3D(a.x/b,a.y/b,a.z/b);
}

Vector3D operator*(Vector3D & a, double  b)
{
	return Vector3D(a.x * b, a.y * b,a.z * b);
}

Vector3D operator+(Vector3D & a, Vector3D & b)
{
	return Vector3D(a.x + b.x,a.y+b.y,a.z + b.z);
}
Vector3D Vector3D::operator+=(Vector3D & b)
{
	this->x += b.x;
	this->y += b.y;
	this->z += b.z;
	return *this;
}
Vector3D operator-(Vector3D& a, Vector3D& b) {
	return Vector3D(a.x - b.x, a.y - b.y,a.z-b.z);
}
