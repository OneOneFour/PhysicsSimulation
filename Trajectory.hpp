#pragma once
#include "Vector3D.hpp"
#include <SFML\Graphics.hpp>
class Body;
class Trajectory
{
public:
	Trajectory(Body* body);
	~Trajectory();
	void updateTrajectory(double dt);
	void drawTrajectory(sf::RenderWindow* window);
private:
	std::vector<sf::Vertex> vertices;
	Body* body;
	Vector3D angularMomentum;
	double eccentricity;
	double semimajoraxis;
	double semiMinoraxis;
	double reducedMass;
	double inclination;

};

