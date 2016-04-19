#include "Body.hpp"
#include "Trajectory.hpp"
#include "Simulation.hpp"

Body::Body(Body* barycenter,std::vector<Body*>* bodies,Vector3D position,double mass,sf::Color color = sf::Color::White):Body(){
	this->barycenter = barycenter;
	this->bodies = bodies;
	this->position = position;
	this->mass = mass;
	this->identify = color;
}
Body::Body(Body* barycenter , std::vector<Body*>* bodies,Vector3D position,double mass,Vector3D velocity,sf::Color color = sf::Color::White):Body(barycenter,bodies,position,mass,color) {
	this->velocity = velocity;
}
Body::~Body(){
}

void Body::addForce(Vector3D & force){
	netForce += force;
}

void Body::update(double dt,bool eulerian,double runTime){
	if (eulerian) { 
		//Eulerian works by finding the area under a velocity time graph. gives an exact answer when a is constant, if a is not constant
		//then it's aproximation is crappy as it's linear.
		acceleration = getAcceleration(position, velocity, dt);
		velocity += acceleration*dt;
		position += velocity * dt;
	}
	else {
		//abridge explanation of how RK4 works
		//RK4 works by using the trapezium rule to provide a much better approximation of position
		//RK4 can be used to integrate any differential equation provided that:



		Derivative a, b, c, d;
		a = stepRK4(position, velocity, runTime, 0.0, Derivative()); // same as eulers approximation above 
		b = stepRK4(position, velocity, runTime, dt * 0.5, a);//starting from midpoint dt/5 
		c = stepRK4(position, velocity, runTime, dt *0.5, b);
		d = stepRK4(position, velocity, runTime, dt, c);//starting from end

		Vector3D dxdt =  (a.dx + (b.dx + c.dx)*2.0 + d.dx) * 1.0/6.0;
		Vector3D dvdt = (a.dv + (b.dv + c.dv) *2.0 + d.dv) * 1.0 / 6.0;

		position += dxdt * dt;
		velocity += dvdt * dt;
	}
	trajectory.updateTrajectory(dt);
}

Vector3D Body::getPos()
{
	return position;
}
void Body::setPos(Vector3D pos){
	this->position = pos;
}
Derivative Body::stepRK4(Vector3D & x, Vector3D & v, double t, double dt, Derivative& d){
	Vector3D stateP = x + d.dx*dt;
	Vector3D stateV = v + d.dv*dt;
	Derivative out; 
	out.dx = stateV;
	out.dv = getAcceleration(stateP,stateV, t + dt);
	return out;
}
Vector3D Body::getVelocity() {
	return velocity;
}
Vector3D Body::getAcceleration(Vector3D & x, Vector3D & v, double dt){
	Vector3D a;
	for (int i = 0; i < bodies->size(); i++) {
		if (bodies->at(i) == this) continue;
		Vector3D dir = (bodies->at(i)->getPos() - x).getNormalized();
		double forceMag = (GConst * bodies->at(i)->getMass()) / Vector3D::getDistanceSq(x, bodies->at(i)->getPos());
		Vector3D g = dir*forceMag;
		a += g;
	}
	return a;
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
