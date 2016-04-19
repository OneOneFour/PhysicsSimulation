#pragma once
#include "Vector3D.hpp"
#include "Trajectory.hpp"
class Trajectory;
struct Derivative{
	Vector3D dx;
	Vector3D dv;
};
class Body
{
public:
	Body* barycenter;
	Body():trajectory(this) {}
	Body(Body* barycenter, std::vector<Body*>* bodies,Vector3D position,double mass,sf::Color color);
	Body(Body* barycenter, std::vector<Body*>* bodies,Vector3D position, double mass,Vector3D velocity,sf::Color color);
	~Body();
	void addForce(Vector3D& force);
	void update(double dt, bool eulerian, double runTime);
	Vector3D getPos();
	Vector3D getVelocity();
	void setPos(Vector3D pos);
	Derivative stepRK4(Vector3D& x, Vector3D& v, double t, double dt, Derivative& d);
	Vector3D getAcceleration(Vector3D& x, Vector3D& v, double dt);
	void setMass(double mass);
	double getMass();
	sf::Color identify;
	Trajectory& getTrajectory();
private:
	std::vector<Body*>* bodies;

	Trajectory trajectory;
	double mass;

	Vector3D dVel;
	Vector3D dPos;

	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;
	Vector3D netForce;
};

