#include "GravitiLib/OptimizationEntityManager.h"
#include <GravitiLib/NBodyEngine.h>
#include "GravitiLib/IPhysicsEngine.h"


OptimizationEntityManager::OptimizationEntityManager()
{
    std::vector<Entity> entities;
    minimumDistancePoint = { 0.0f, 0.0f, 0.0f };
    float ShortestMagnitude = 0;
    m_nextID = 0;
    targetPoint = { 0.0f, 0.0f, 0.0f };
	

}


void OptimizationEntityManager::addEntity(Entity& entity)
{
    entity.setID(m_nextID++);
    entities.push_back(entity);
}

void OptimizationEntityManager::addTargetEntity(Entity entity)
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
			//std::cout << "Simulation step: " << step << "/" << totalTimeSteps << " at time " << time << std::endl;
		}
		time += timeStep;
	}

	for (auto i : entities) 
	{
		if (i.getEntityID() == targetEntity.getEntityID())
		{
			targetEntity = i;
		}
	}
	std::cout << "Batch simulation completed!" << std::endl;
}

float OptimizationEntityManager::calculateDistance(Vec3 a, Vec3 b)
{
	Vec3 distance;
	distance.x = b.x - a.x;
	distance.y = b.y - a.y;
	distance.z = b.z - a.z;
	float magnitude = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);

	return magnitude;
}


Vec3 OptimizationEntityManager::DetermineMinimumDistancePoint()
{
	std::vector<PhysicalState> history = targetEntity.getPastTrajectory();
	for (auto& state : history)
	{
		Vec3 position = state.getPosition();
		float distance = calculateDistance(position, targetPoint);
		Vec3 currentMinimum = minimumDistancePoint;
		float currentMinDistance = calculateDistance(currentMinimum, targetPoint);
		if (distance < currentMinDistance)
		{
			minimumDistancePoint = position;
		}
	}
	ShortestMagnitude = calculateDistance(minimumDistancePoint, targetPoint);
	return minimumDistancePoint;
}
