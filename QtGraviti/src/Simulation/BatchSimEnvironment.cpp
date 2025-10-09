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

void BatchSimEnvironment::initialize_two_body()
{
	// Create entities
	auto entityManager = EntityManager::getInstance();


	std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<NBodyPhysics>();
	Entity earth = Entity(physicsEngine);
	earth.setEntityName("Earth");
	earth.getPhysicalState()->setMass(100.0e10f);
	entityManager->addEntity(earth);

	physicsEngine = std::make_unique<NBodyPhysics>();
	Entity moon = Entity(physicsEngine);
	moon.setEntityName("Moon");
	PhysicalState moonState;
	moonState.setPosition(X, 25);
	moonState.setVelocity(Z, 2);
	moonState.setVelocity(Y, 0.3);
	moonState.setMass(1.0e10f);
	moon.setOrigin(moonState);
	entityManager->addEntity(moon);


}

void BatchSimEnvironment::initialize_three_body()
{
	// Create entities
	auto entityManager = EntityManager::getInstance();


	std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<NBodyPhysics>();
	Entity earth = Entity(physicsEngine);
	earth.setEntityName("Earth");
	earth.getPhysicalState()->setMass(100.0e10f);
	entityManager->addEntity(earth);

	physicsEngine = std::make_unique<NBodyPhysics>();
	Entity moon = Entity(physicsEngine);
	moon.setEntityName("Moon");
	PhysicalState moonState;
	moonState.setPosition(X, 25);
	moonState.setVelocity(Z, 2);
	moonState.setVelocity(Y, 0.3);
	moonState.setMass(1.0e10f);
	moon.setOrigin(moonState);
	entityManager->addEntity(moon);

	physicsEngine = std::make_unique<NBodyPhysics>();
	Entity moon2 = Entity(physicsEngine);
	moon2.setEntityName("moon2");
	PhysicalState moon2State;
	moon2State.setPosition(X, 250);
	moon2State.setVelocity(Y, 0.4);
	moon2State.setMass(1.0e10f);
	moon2.setOrigin(moon2State);
	entityManager->addEntity(moon2);
}

float BatchSimEnvironment::cleanFloat(std::string value)
{
	if (value.back() == 'f') {
		value.pop_back();
	}
	return std::strtof(value.c_str(), nullptr);
}

void BatchSimEnvironment::initialize_json_body(std::string filepathjsonPath)
{
	// Create entities
	auto entityManager = EntityManager::getInstance();
	auto entities = entityManager->getAllEntities();

	FILE* fp = fopen(filepathjsonPath.c_str(), "r");
	if (!fp) {
		// Handle file open error (optional: log or throw)
		return;
	}

	// Move the large buffer to the heap to avoid stack overflow
	std::unique_ptr<char[]> readBuffer(new char[65536]);
	rapidjson::FileReadStream is(fp, readBuffer.get(), 65536);

	rapidjson::Document d;
	d.ParseStream(is);

	fclose(fp);
	std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<NBodyPhysics>();
	rapidjson::Value::ConstValueIterator itr;

	for (auto itr = d.Begin(); itr != d.End(); ++itr) {


		physicsEngine = std::make_unique<NBodyPhysics>();
		Entity jsonEntity = Entity(physicsEngine);
		PhysicalState jsonEntityState;
		auto obj = itr->GetObject();
		jsonEntity.setEntityName(obj["name"].GetString());


		jsonEntityState.setPosition(X, cleanFloat(obj["positionX"].GetString()));
		jsonEntityState.setPosition(Y, cleanFloat(obj["positionY"].GetString()));
		jsonEntityState.setPosition(Z, cleanFloat(obj["positionZ"].GetString()));

		jsonEntityState.setVelocity(X, cleanFloat(obj["velocityX"].GetString()));
		jsonEntityState.setVelocity(Y, cleanFloat(obj["velocityY"].GetString()));
		jsonEntityState.setVelocity(Z, cleanFloat(obj["velocityZ"].GetString()));
		jsonEntityState.setMass(cleanFloat(obj["mass"].GetString()));


		jsonEntity.setOrigin(jsonEntityState);
		entityManager->addEntity(jsonEntity);


	}


	
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
		if (step % 10 == 0) {
			std::cout << "Simulation step: " << step << "/" << totalTimeSteps << " at time" << time << std::endl;
		}
		time += timeStep;
	}
	for (auto& entity : *entities) {
		entity.savePastTrajectoryToCSV();
	}
	std::cout << "Batch simulation completed!" << std::endl;
}