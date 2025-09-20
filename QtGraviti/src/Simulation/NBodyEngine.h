#pragma once
#include "IPhysicsEngine.h"
#include "EntityManager.h"

class NBodyPhysics : public IPhysicsEngine
{
private:
	//not sure if we set current time and end time here vs in the cpp file
	float G = 6.67430e-11f; // Gravitational constant in m^3 kg^-1 s^-2
	float timestep = 1.0f; // Time step in seconds
	float currentTime = 0.0f;
	float endTime = 100.0f;
	std::shared_ptr<std::vector<Entity>> m_entities;

public:
	void simulate(float duration, int timeSteps, PhysicalState& currentState, long int callingID, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory);
};

