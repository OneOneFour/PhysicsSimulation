#pragma once
#include <string>
#include <SFML\Graphics.hpp>
class Vector3D
{
public:
	Vector3D(double x,double y,double z);
	Vector3D() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	~Vector3D();
	Vector3D getNormalized();
	static double dotProduct(Vector3D a, Vector3D b);//Dot Product of Vector
	static double getAccuteAngle(Vector3D a, Vector3D b);//Angle in Radians
	static Vector3D getCrossProduct(Vector3D a, Vector3D b);
	static double getDistanceSq(Vector3D a, Vector3D b);
	static double getDistance(Vector3D a, Vector3D b);
	std::string toString();
	double getMagnitude();
	double getMagnitudeSq();
	double x;
	double y;
	double z;
	friend Vector3D operator/(Vector3D& a, double b);
	friend Vector3D operator*(Vector3D& a, double b);
	friend Vector3D operator+(Vector3D& a,Vector3D& b);
	Vector3D operator+=(Vector3D& b);
	friend Vector3D operator-(Vector3D& a, Vector3D& b);
};

