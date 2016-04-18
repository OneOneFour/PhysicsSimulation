#include "Simulation.hpp"
#include "Body.hpp"
#include <iostream>
Simulation::Simulation():barycenter(){
	onStart();
	filename = "";
}

Simulation::Simulation(std::ifstream & fileStream, std::string & filename):barycenter(){
	//check file is okay again
	if (!fileStream) {
		std::cout << "Something bad happened when creating a simulation" << std::endl;
		delete this;//SUICICDE
		return;
	}
	std::string lineData;
	while (std::getline(fileStream, lineData)) {
		lineData.erase(std::remove_if(lineData.begin(), lineData.end(), isspace), lineData.end()); //die whitespace
		if (lineData.find("Body{") != std::string::npos) {
			double mass = 0;
			Vector3D pos, velocity;
			sf::Color color;
			while (std::getline(fileStream, lineData)) {
				lineData.erase(std::remove_if(lineData.begin(), lineData.end(), isspace), lineData.end());
				if (lineData.find("mass") != std::string::npos) {
					mass = std::atof(lineData.substr(5).c_str());
					continue;
				}
				if (lineData.find("pos") != std::string::npos) {
					pos = toVector(lineData, 4);
					continue;
				}
				if (lineData.find("vel") != std::string::npos) {
					velocity = toVector(lineData, 4);
					continue;
				}
				if (lineData.find("color") != std::string::npos) {
					Vector3D colorVector = toVector(lineData, 6);
					color = sf::Color((int)colorVector.x, (int)colorVector.y, (int)colorVector.z);
					continue;
				}
				if (mass == 0) {
					//BAD BAD BAD BAD BAD BAD BAD BAD BAD
					std::cout << "Something BAD BAD BAD" << std::endl;
					break;
				}
				addBody(new Body(&barycenter, pos, mass, velocity, color));
				std::cout << "Body created at " << pos.toString() << std::endl;
				if (lineData.find("}") != std::string::npos) break;
			}
		}
	}
	this->filename = filename.substr(0, filename.size() - 4);
	fileStream.close();
	onStart();
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

void Simulation::onStart(){
	window.create(sf::VideoMode(800, 480), filename + " Physics Simulation");
	simClock.restart();
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(0);
	while (window.isOpen()) {
		dt = simClock.restart().asSeconds() * StepTime;
		if (dt == 0) dt = 1.0 / 1000.0; //prevent divion by zero
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				std::cout << "Save the file(Y/N)?" << std::endl;
				std::string response;
				std::cin >> response;
				if (response == ("Y")) {
					std::cout << "Enter file name. Press * to overwrite current file" << std::endl;
					std::cin >> response;
					if (response !="*") {
						this->filename = response;
					}
					onSave();
				}
				window.close();
			}
		}
		onUpdate();
		window.clear(sf::Color::Black);
		onDraw();
		window.display();
	}
}

void Simulation::onSave(){
	std::ofstream saveFile;
	saveFile.open(filename + ".sim",std::ios::out|std::ios::trunc);
	for (int i = 0; i < bodies.size(); i++) {
		saveFile << "Body{\n";
		saveFile << "mass=" << bodies[i]->getMass() << "\n";
		saveFile << "pos=" << bodies[i]->getPos().x << "," << bodies[i]->getPos().y << "," << bodies[i]->getPos().z << "\n";
		saveFile << "vel=" << bodies[i]->getVelocity().x <<","<<bodies[i]->getVelocity().y<<","<<bodies[i]->getVelocity().z <<"\n";
		saveFile << "color="<<(int)bodies[i]->identify.r <<","<<(int)bodies[i]->identify.g<<","<<(int)bodies[i]->identify.b <<"\n";
		saveFile << "}\n";
	}
	saveFile.flush();
	saveFile.close();
}

Vector3D Simulation::toVector(std::string lineData,int offset = 0)
{
	size_t cPos = lineData.find(",");
	if (cPos == std::string::npos) {
		//SOMETHIGN BAD HAPPENED
	}
	double x = std::atof(lineData.substr(offset, cPos).c_str());
	size_t scPos = lineData.find(",", cPos + 1);
	double y = std::atof(lineData.substr(cPos+1, scPos).c_str());
	double z = std::atof(lineData.substr(scPos+1).c_str());
	return Vector3D(x, y, z);
}
