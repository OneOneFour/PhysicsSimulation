#include <fstream>
#include <iostream>
#include "Simulation.hpp"
int main() {
	while (true) {
		std::cout << "Enter file name to load " << std::endl;
		std::string filename;	
		std::cin >> filename;
		if (filename == "q") break;
		std::ifstream fileStream;
		fileStream.open(filename.c_str());
		if (!fileStream) {
			std::cout << "An error occured opening file " << filename << std::endl;
			continue;
		}
		Simulation sim(fileStream,filename);
	}
	return 0;
}