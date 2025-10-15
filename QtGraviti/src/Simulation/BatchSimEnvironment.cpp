#include "GravitiLib/BatchSimEnvironment.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"
#include "GravitiLib/EntityManager.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <memory>
#include <iostream>

BatchSimEnvironment::BatchSimEnvironment()
{
	m_origin_time = 0.0f;
}

void BatchSimEnvironment::run()
{
	auto entityManager = EntityManager::getInstance();
	auto entities = entityManager->getAllEntities();


	// Configure simulation parameters
	const int totalTimeSteps = 2000;  // Total number of simulation steps
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
			if (!entity.Simulate(timeStep)) {
				// Handle simulation error if needed
				std::cout << "Simulation failed for entity: " << entity.getEntityID() << std::endl;
			}
		}
		for (auto& entity : *entities) {
			entity.TickForward();
			entity.saveCurrentStateToCSV();

		}
		// Optional: Add progress output
		if (step % 200 == 0) {
			std::cout << "Simulation step: " << step << "/" << totalTimeSteps << " at time" << time << std::endl;
		}
		time += timeStep;
	}
	for (auto& entity : *entities) {
		entity.savePastTrajectoryToCSV();
	}
	std::cout << "Batch simulation completed!" << std::endl;
}