#include "Simulation.hpp"
#include "Body.hpp"

Simulation::Simulation():barycenter(){
	window.create(sf::VideoMode(800, 480), "Physics Simulation");
	addBody(new Body(&barycenter, Vector3D(0, 0, 0), 1.989*pow(10, 30), Vector3D(0, 0, 0),sf::Color::Yellow));
	addBody(new Body(&barycenter, Vector3D(1.49 * pow(10, 11), 0, 0), 5.972 *pow(10, 24), Vector3D(0, 30000, 0),sf::Color::Blue));
	addBody(new Body(&barycenter, Vector3D(1.08 * pow(10, 11), 0, 0), 4.86 *pow(10, 24), Vector3D(0, 35000, 0), sf::Color::Magenta));
	addBody(new Body(&barycenter, Vector3D(2.27 * pow(10, 11), 0, 0), 6.39 *pow(10, 23), Vector3D(0, 24100, 0),sf::Color::Red));
	addBody(new Body(&barycenter, Vector3D(7.78 * pow(10, 11), 0, 0), 1.8 *pow(10, 27), Vector3D(0, 13100, 0), sf::Color::White));
	simClock.restart();
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(0);
	while (window.isOpen()) {
		dt = simClock.restart().asSeconds() * StepTime;
		if (dt == 0) dt = 1.0 / 1000.0; //prevent divion by zero
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}
		onUpdate();
		window.clear(sf::Color::Black);
		onDraw();
		window.display();
	}
}


Simulation::~Simulation(){
	while (!bodies.empty()) {
		delete bodies.back();
		bodies.pop_back();
	}
}

void Simulation::addBody(Body* body){
	bodies.push_back(body);

	//update barycenter

}

void Simulation::onUpdate(){
	barycenter.setPos(Vector3D(0, 0, 0));
	barycenter.setMass(0);
	for (int i = 0; i < bodies.size(); i++) {
		barycenter.setPos((barycenter.getPos() * barycenter.getMass() + bodies[i]->getPos() * bodies[i]->getMass()) / (barycenter.getMass() + bodies[i]->getMass()));
		barycenter.setMass(barycenter.getMass() + bodies[i]->getMass());
		for (int j = 0; j < bodies.size(); j++) {
			if (i == j) continue;
			Vector3D dir = (bodies[j]->getPos() - bodies[i]->getPos()).getNormalized();
			double forceMag = (GConst * bodies[j]->getMass() * bodies[i]->getMass()) / Vector3D::getDistanceSq(bodies[i]->getPos(), bodies[j]->getPos());
			bodies[i]->addForce(dir * forceMag);
		}
	}
}

void Simulation::onDraw(){	
	for (int i = 0; i < bodies.size(); i++) {
		bodies[i]->update(dt);
		//update bary
		bodies[i]->getTrajectory().drawTrajectory(&window);
		shape.setFillColor(bodies[i]->identify);
		shape.setRadius(5);
		shape.setOrigin(5, 5);
		shape.setPosition(sf::Vector2f(
			((bodies[i]->getPos().x - barycenter.getPos().x) * ScaleConst) + 400,
			((bodies[i]->getPos().y - barycenter.getPos().y) * ScaleConst) + 240
			));
		window.draw(shape);
		
	}
	baryPoint = sf::CircleShape(5, 3);
	baryPoint.setOrigin(5, 5);
	baryPoint.setPosition(sf::Vector2f(
		 400,
		 240
		));
	baryPoint.setFillColor(sf::Color::Red);
	window.draw(baryPoint);
}
