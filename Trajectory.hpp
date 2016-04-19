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
	void drawTrajectory(sf::RenderWindow* window,double scaleConstant,int xoffset,int yoffset);
private:
	
	std::vector<sf::Vertex> pastP;
	double scalec;
	int x, y;
	Body* body;
	Vector3D angularMomentum;
	double eccentricity;
	double semimajoraxis;
	double semiMinoraxis;
	double reducedMass;
	double inclination;

};

