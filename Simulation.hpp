#pragma once
#define AU 1.496e+11
#define GConst 0.00000000006674
#include <SFML\Graphics.hpp>
#include <vector>
#include <fstream>
#include "Body.hpp"
class Body;
class Simulation
{
public:
	Simulation();
	Simulation(std::ifstream& fileStream, std::string& filename);
	~Simulation();
	void addBody(Body* body);
	void onUpdate(double delta);
	void onDraw();
private:
	sf::View view;
	bool eulerian = false;
	double runTimeSeconds;
	double stepTime = 60 * 60 * 24 * 30;
	double scaleConst = 0.0000000003;
	void onStart(); // FUCK YOU C++ s
	void onSave();
	std::string filename;
	Vector3D toVector(std::string lineData,int offset);
	Body barycenter;
	std::vector<Body*> bodies;
	sf::RenderWindow window;
	sf::Clock simClock;
	sf::CircleShape baryPoint;
	sf::CircleShape trajectory;
	sf::CircleShape shape;
	double dt = 1.0/60.0;
	double frameTime;
};

