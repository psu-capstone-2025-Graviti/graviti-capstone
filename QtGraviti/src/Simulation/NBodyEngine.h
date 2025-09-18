#pragma once
#include "IPhysicsEngine.h"

class NBodyPhysics : IPhysicsEngine
{
	//not sure if we set current time and end time here vs in the cpp file
	float G = 6.67430e-11f; // Gravitational constant in m^3 kg^-1 s^-2
	float timestep = 1.0f; // Time step in seconds
	float currentTime = 0.0f;
	float endTime = 100.0f;
	
public:
	void simulate(float duration, int timeSteps, PhysicalState& currentState, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory);
};

