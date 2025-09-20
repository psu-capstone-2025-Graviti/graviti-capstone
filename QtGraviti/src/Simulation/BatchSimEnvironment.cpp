#include "BatchSimEnvironment.h"
#include "Entity.h"
#include "NBodyEngine.h"
#include "EntityManager.h"
#include <memory>
#include <iostream>

BatchSimEnvironment::BatchSimEnvironment()
{
	m_origin_time = 0.0f;
}

void BatchSimEnvironment::initialize()
{
	// Create entities
	auto entityManager = EntityManager::getInstance();

	/*
	
    float posE[3] = { 0.0f,0.0f,0.0f };     // x, y, z
    float velE[3] = { 0.0f,0.0f,0.0f };     // x, y, z
    float massofEarth = 100.0e10f; // in kg

    auto earth = orbitObject(massofEarth, 100.0f, posE, velE, "Earth");
    float posM[3] = { 100.0f,0.0f,0.0f };     // x, y, z
    float velM[3] = { 0.0f,0.0f,0.0f };     // x, y, z
    float massofMoon = 1.0e10f; // in kg

    auto moon = orbitObject(massofMoon, 50.0f, posM, velM, "Moon");
	*/
	std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<NBodyPhysics>();
	Entity earth = Entity(physicsEngine);
	earth.setEntityName("Earth");
	earth.getPhysicalState()->setMass(100.0e10f);
	entityManager->addEntity(earth);

	physicsEngine = std::make_unique<NBodyPhysics>();
	Entity moon = Entity(physicsEngine);
	moon.setEntityName("Moon");
	PhysicalState moonState;
	moonState.setPosition(X, 100.0f);
	moonState.setMass(1.0e10f);
	moon.setOrigin(moonState);
	entityManager->addEntity(moon);

}


void BatchSimEnvironment::run()
{
	auto entityManager = EntityManager::getInstance();
	auto entities = entityManager->getAllEntities();

	// Configure simulation parameters
	const int totalTimeSteps = 1000;  // Total number of simulation steps
	const float timeStep = 1.0f;      // Time step in seconds
	float time = 0.0f;
	// Set timestep for all entities
	for (auto& entity : *entities) {
		entity.setTimestep(time);
	}

	// Run the simulation for the specified number of time steps
	for (int step = 0; step < totalTimeSteps; step++) {
		// Simulate all entities for one time step
		for (auto& entity : *entities) {
			if (!entity.Simulate(1)) {
				// Handle simulation error if needed
				std::cout << "Simulation failed for entity: " << entity.getEntityID() << std::endl;
			}
		}
		for (auto& entity : *entities) {
			entity.getPhysicalState()->integrate(timeStep);
			entity.getPhysicalState()->writepositionCSV(entity.getEntityName());
		}
		// Optional: Add progress output
		if (step % 10 == 0) {
			std::cout << "Simulation step: " << step << "/" << totalTimeSteps << " at time" << time << std::endl;
		}
		time += timeStep;
	}
	
	std::cout << "Batch simulation completed!" << std::endl;
}