#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <iostream>
#include "Simulation.hpp"
#include "Trajectory.hpp"
#include "Body.hpp"

Trajectory::Trajectory(Body* body) {
	this->body = body;
}

Trajectory::~Trajectory()
{
}

void Trajectory::updateTrajectory(double dt) {
	//double distanceToBary = Vector3D::getDistance(body->barycenter->getPos(), body->getPos());
	//Vector3D orbitalPosition = body->getPos() - body->barycenter->getPos();
	//semimajoraxis = std::cbrt((GConst*(body->getMass()+body->barycenter->getMass()) * pow(distanceToBary,2)) / (body->getVelocity().getMagnitudeSq()));
	//reducedMass = (body->barycenter->getMass() * body->getMass()) / (body->barycenter->getMass() + body->getMass());	
	//angularMomentum = Vector3D::getCrossProduct(body->getVelocity(), orbitalPosition);
	//eccentricity = body->barycenter->getPos().getMagnitude() / semimajoraxis;
	//semiMinoraxis = sqrt(pow(semimajoraxis, 2)*(1 - pow(eccentricity, 2)));
	//inclination = Vector3D::getAccuteAngle(angularMomentum, Vector3D(0, 1, 0));

}

void Trajectory::drawTrajectory(sf::RenderWindow * window, double scaleConstant,int xoffset,int yoffset) {
	if (scaleConstant != scalec) {
		for (int i = 0; i < pastP.size(); i++) {
			pastP[i].position = sf::Vector2f((pastP[i].position.x - 400 -x)*(scaleConstant / scalec) + 400 + x,
				(pastP[i].position.y - 240 - y)*(scaleConstant / scalec) + 240 + y);
		}
		this->scalec = scaleConstant;
	}
	if (xoffset != x || yoffset != y) {
		for (int i = 0; i < pastP.size(); i++) {
			pastP[i].position.x += (xoffset - x);
			pastP[i].position.y += (yoffset - y);
		}
		this->x = xoffset;
		this->y = yoffset;
	}
	pastP.push_back(sf::Vertex(sf::Vector2f(
		(body->getPos().x - body->barycenter->getPos().x)*scalec + 400 +x,
		(body->getPos().y - body->barycenter->getPos().y)*scalec + 240 + y)));

	if (pastP.size() > 10000) pastP.erase(pastP.begin());
	window->draw(&pastP[0], pastP.size(), sf::LinesStrip);
}
