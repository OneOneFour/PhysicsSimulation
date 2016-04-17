#pragma once
#include "Vector3D.hpp"
#include "Trajectory.hpp"
class Trajectory;
class Body
{
public:
	Body* barycenter;
	Body():trajectory(this) {}
	Body(Body* barycenter,Vector3D position,double mass,sf::Color color);
	Body(Body* barycenter,Vector3D position, double mass,Vector3D velocity,sf::Color color);
	~Body();
	void addForce(Vector3D& force);
	void update(double dt);
	Vector3D getPos();
	Vector3D getVelocity();
	void setPos(Vector3D pos);
	void setMass(double mass);
	double getMass();
	sf::Color identify;
	Trajectory& getTrajectory();
private:
	Trajectory trajectory;
	double mass;

	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;
	Vector3D netForce;
};

