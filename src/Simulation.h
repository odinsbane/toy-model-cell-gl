

#ifndef _MESH_OBJECT
#define _MESH_OBJECT

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class Simulation{
	std::map<std::string, float> valuemap;
	float getValue(const char* key);
	
	public:
		Simulation(std::map<std::string, float> values);
		double PNOT, SPRING, DT, DRAG, ACTIVE_TENSION;
		int SUB_SAMPLE, STEPS_PER_FRAME;
		static Simulation* loadSimulation(const std::string &confFileName);
	
};

#endif
