#include "Body.hpp"
#include "Trajectory.hpp"


Body::Body(Body* barycenter,Vector3D position,double mass,sf::Color color):Body(){
	this->barycenter = barycenter;
	this->position = position;
	this->mass = mass;
	this->identify = color;
}
Body::Body(Body* barycenter ,Vector3D position,double mass,Vector3D velocity,sf::Color color):Body(barycenter,position,mass,color) {
	this->velocity = velocity;
}
Body::~Body(){
}

void Body::addForce(Vector3D & force){
	netForce += force;
}

void Body::update(double dt){
	acceleration = netForce / mass;
	velocity += acceleration*dt;
	position += velocity * dt;
	netForce = Vector3D(0, 0,0);
	trajectory.updateTrajectory(dt);
}

Vector3D Body::getPos()
{
	return position;
}
void Body::setPos(Vector3D pos){
	this->position = pos;
}
Vector3D Body::getVelocity() {
	return velocity;
}
void Body::setMass(double mass){
	this->mass = mass;
}
double Body::getMass() {
	return mass;
}

Trajectory & Body::getTrajectory()
{
	return trajectory;
}
