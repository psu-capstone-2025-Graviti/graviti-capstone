#include "GravitiLib/OptimizationEntityManager.h"
#include <GravitiLib/NBodyEngine.h>
#include "GravitiLib/IPhysicsEngine.h"


OptimizationEntityManager::OptimizationEntityManager()
{
    std::vector<Entity> entities;
	Vec3 minimumDistancePoint = { 0.0f, 0.0f, 0.0f };
	m_nextID = 0;
	targetPoint = { 0.0f, 0.0f, 0.0f };
	

}


void OptimizationEntityManager::addEntity(Entity& entity)
{
    entity.setID(m_nextID++);
    entities.push_back(entity);
}

void OptimizationEntityManager::addTargetEntity(Entity& entity)
{
	entity.setID(m_nextID++);
	entities.push_back(entity);
	targetEntity = entity;
}



std::vector<Entity> OptimizationEntityManager::getAllEntities()
{
    return entities;
}

void OptimizationEntityManager::loadTargetPoint(Vec3 targetPosition)
{
	targetPoint = targetPosition;
}

void OptimizationEntityManager::clearEntities()
{
    entities.clear();
}

int OptimizationEntityManager::entityManagerCount()
{
    return entities.size();
}

void OptimizationEntityManager::run(const int totalTimeSteps, const float timeStep)
{
	


	// Configure simulation parameters
	float time = 0.0f;
	// Set timestep for all entities
	for (auto& entity : entities) {
		entity.setTimestep(time);
	}

	// Run the simulation for the specified number of time steps
	for (int step = 0; step < totalTimeSteps; step++) {
		// Simulate all entities for one time step
		for (auto& entity : entities) {
			if (!entity.Simulate(timeStep)) {
				// Handle simulation error if needed
				std::cout << "Simulation failed for entity: " << entity.getEntityID() << std::endl;
			}
		}
		for (auto& entity : entities) {
			entity.TickForward();
			//entity.saveCurrentStateToCSV();
		}
		// Optional: Add progress output
		if (step % 200 == 0) {
			std::cout << "Simulation step: " << step << "/" << totalTimeSteps << " at time" << time << std::endl;
		}
		time += timeStep;
	}
	std::cout << "Batch simulation completed!" << std::endl;
}

Vec3 OptimizationEntityManager::DetermineMinimumDistancePoint()
{


	return minimumDistancePoint;
}
