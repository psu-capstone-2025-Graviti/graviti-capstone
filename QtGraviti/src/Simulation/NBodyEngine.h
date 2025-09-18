#pragma once
#include "IPhysicsEngine.h"

class NBodyPhysics : IPhysicsEngine
{
public:
	void simulate(float duration, int timeSteps, PhysicalState& currentState, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory);
};

