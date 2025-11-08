#include "SimulationControl.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"
#include "GravitiLib/EntityManager.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "OptimizationController.h"


SimulationController::SimulationController(QObject* parent)
    : QObject(parent),
	m_env()
{
    //m_entityList = new EntityListGUI(); //new? shared ptr?

}

SimulationController::~SimulationController()
{
}

void SimulationController::startSimulation(int numSteps, float tickDuration)
{
	m_env.run(numSteps, tickDuration);
}

void SimulationController::resetSimulation()
{
	m_env.resetSimulation();
}
void SimulationController::clearEntities()
{
	auto entityManager = EntityManager::getInstance();
	entityManager->clearEntities();
}

Entity SimulationController::optimizeTrajectory(Entity projectile, Vec3 targetPosition, int numSteps, float tickDuration)
{
	auto entitiesPtr = EntityManager::getInstance()->getAllEntities();
	const std::vector<Entity>& Entities = *entitiesPtr;

	OptimizationController optimizer;
	optimizer.LoadEntities(Entities);
	optimizer.LoadProjectile(projectile);
	optimizer.LoadTarget(targetPosition);

	int numberofIterations =1;

	optimizer.optimize(numSteps, tickDuration, numberofIterations);

	return optimizer.getBestEntity();


	// Placeholder for trajectory optimization logic
}

void SimulationController::createEntity(const std::string& name, float posX, float posY, float posZ, 
                                     float velX, float velY, float velZ, float mass)
{
	auto entityManager = EntityManager::getInstance();

	std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();
	Entity newEntity(physicsEngine);
	newEntity.setEntityName(name);

	PhysicalState entityState;
	entityState.setPosition(X, posX);
	entityState.setPosition(Y, posY);
	entityState.setPosition(Z, posZ);
	entityState.setVelocity(X, velX);
	entityState.setVelocity(Y, velY);
	entityState.setVelocity(Z, velZ);
	entityState.setMass(mass);
	entityState.setRadius(0.2f); // Default radius

	newEntity.setOrigin(entityState);
	entityManager->addEntity(newEntity);
}



void SimulationController::initialize_json_body(std::string filepathjsonPath)
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
	std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();
	rapidjson::Value::ConstValueIterator itr;

	for (auto itr = d.Begin(); itr != d.End(); ++itr) {


		physicsEngine = std::make_shared<NBodyPhysics>();
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
		jsonEntityState.setRadius(cleanFloat(obj["radius"].GetString()));

		// Set texture path if it exists in JSON
		if (obj.HasMember("texturePath") && obj["texturePath"].IsString()) {
			jsonEntity.setTexturePath(obj["texturePath"].GetString());
		}

		jsonEntity.setOrigin(jsonEntityState);
		entityManager->addEntity(jsonEntity);
	}
}

void SimulationController::initializeThreeBody()
{
	// Create entities
	auto entityManager = EntityManager::getInstance();

	std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();
	Entity earth(physicsEngine);
	earth.setEntityName("Earth");
	earth.getPhysicalState()->setMass(100.0e10f);
	earth.setTexturePath("qrc:/content/images/Blue_Marble_2002.jpg");
	entityManager->addEntity(earth);

	physicsEngine = std::make_shared<NBodyPhysics>();
	Entity moon(physicsEngine);
	moon.setEntityName("Moon");
	PhysicalState moonState;
	moonState.setPosition(X, 25);
	moonState.setVelocity(Z, 2);
	moonState.setVelocity(Y, 0.3);
	moonState.setMass(1.0e10f);
	moon.setOrigin(moonState);
	entityManager->addEntity(moon);

	physicsEngine = std::make_shared<NBodyPhysics>();
	Entity moon2(physicsEngine);
	moon2.setEntityName("moon2");
	PhysicalState moon2State;
	moon2State.setPosition(X, 250);
	moon2State.setVelocity(Y, 0.4);
	moon2State.setMass(1.0e10f);
	moon2.setOrigin(moon2State);
	entityManager->addEntity(moon2);
}



void SimulationController::saveEntitiesAsJson(std::string filepathjsonPath)
{
	// Create entities
	auto entityManager = EntityManager::getInstance();
	auto entities = entityManager->getAllEntities();

	// Create a JSON array
	rapidjson::Document document;
	document.SetArray();

	for (size_t i = 0; i < entities->size(); ++i) {
		auto& entity = (*entities)[i];
		PhysicalState state = entity.getOrigin();

		rapidjson::Value obj(rapidjson::kObjectType);

		obj.AddMember("name", rapidjson::Value().SetString(entity.getEntityName().c_str(), document.GetAllocator()), document.GetAllocator());
		obj.AddMember("positionX", rapidjson::Value().SetString(std::to_string(state.getPosition(X)).c_str(), document.GetAllocator()), document.GetAllocator());
		obj.AddMember("positionY", rapidjson::Value().SetString(std::to_string(state.getPosition(Y)).c_str(), document.GetAllocator()), document.GetAllocator());
		obj.AddMember("positionZ", rapidjson::Value().SetString(std::to_string(state.getPosition(Z)).c_str(), document.GetAllocator()), document.GetAllocator());
		obj.AddMember("velocityX", rapidjson::Value().SetString(std::to_string(state.getVelocity(X)).c_str(), document.GetAllocator()), document.GetAllocator());
		obj.AddMember("velocityY", rapidjson::Value().SetString(std::to_string(state.getVelocity(Y)).c_str(), document.GetAllocator()), document.GetAllocator());
		obj.AddMember("velocityZ", rapidjson::Value().SetString(std::to_string(state.getVelocity(Z)).c_str(), document.GetAllocator()), document.GetAllocator());
		obj.AddMember("mass", rapidjson::Value().SetString(std::to_string(state.getMass()).c_str(), document.GetAllocator()), document.GetAllocator());
		obj.AddMember("radius", rapidjson::Value().SetString(std::to_string(state.getRadius()).c_str(), document.GetAllocator()), document.GetAllocator());

		// Add texture path if it exists
		if (!entity.getTexturePath().empty()) {
			obj.AddMember("texturePath", rapidjson::Value().SetString(entity.getTexturePath().c_str(), document.GetAllocator()), document.GetAllocator());
		}

		document.PushBack(obj, document.GetAllocator());
	}

	// Write to file
	FILE* fp = fopen(filepathjsonPath.c_str(), "w");
	if (fp) {
		char writeBuffer[65536];
		rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
		rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
		document.Accept(writer);
		fclose(fp);
	}
}

float SimulationController::cleanFloat(std::string value)
{
	if (value.back() == 'f') {
		value.pop_back();
	}
	return std::strtof(value.c_str(), nullptr);
}