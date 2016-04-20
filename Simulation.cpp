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
				if (lineData.find("}") != std::string::npos) {
					if (mass == 0) {
						//BAD BAD BAD BAD BAD BAD BAD BAD BAD
						std::cout << "Something BAD BAD BAD" << std::endl;
						break;
					}
					addBody(new Body(&barycenter,&bodies, pos, mass, velocity, color));
					std::cout << "Body created at " << pos.toString() << std::endl;
					break;
				}
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

void Simulation::onUpdate(double delta){
	barycenter.setPos(Vector3D(0, 0, 0));
	barycenter.setMass(0);
	for each(Body* b in bodies) {
		b->update(delta, eulerian, runTimeSeconds);
		barycenter.setPos((barycenter.getPos() * barycenter.getMass() + b->getPos()*b->getMass()) / (barycenter.getMass() + b->getMass()));
		barycenter.setMass(barycenter.getMass() + b->getMass());
	} 
}

void Simulation::onDraw(){	
	for (int i = 0; i < bodies.size(); i++) {
		//update bary
		bodies[i]->getTrajectory().drawTrajectory(&window,scaleConst);
		shape.setFillColor(bodies[i]->identify);
		double radius = std::log10(bodies[i]->getMass()) -20;
		shape.setRadius(radius);
		shape.setOrigin(radius, radius);
		shape.setPosition(sf::Vector2f(
			((bodies[i]->getPos().x - barycenter.getPos().x) * scaleConst) + 400,
			((bodies[i]->getPos().y - barycenter.getPos().y) * scaleConst) + 240
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
	view.setSize(800, 480);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	while (window.isOpen()) {
		frameTime = simClock.restart().asSeconds();
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
			if (e.type == sf::Event::MouseWheelMoved) {
				std::cout << e.mouseWheel.delta << std::endl;
				scaleConst *= (e.mouseWheel.delta > 0) ? 5 : 0.2;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			view.move(sf::Vector2f(50*frameTime,0));
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			view.move(sf::Vector2f(-50*frameTime, 0));

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			view.move(sf::Vector2f(0, 50*frameTime));

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			view.move(sf::Vector2f(0, -50*frameTime));

		}
		//Step this on a fixed time step...
		while (frameTime > 0.0) {
			double delta = std::min(frameTime,dt );//is the fps above or below target 
			onUpdate(delta*stepTime);
			frameTime -= delta;
			runTimeSeconds += delta*stepTime;
		}
		window.setView(view);
		window.clear(sf::Color::Black);
		onDraw();
		window.display();
		runTimeSeconds += dt * stepTime;
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
