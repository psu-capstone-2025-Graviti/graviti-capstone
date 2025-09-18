#include "NBodyEngine.h"
#include "EntityManager.h"

void NBodyPhysics::simulate(float duration, int timeSteps, PhysicalState& currentState, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory)
{
	auto entManager = EntityManager::getInstance();

	auto entities = entManager->getAllEntities();
	//TODO
}