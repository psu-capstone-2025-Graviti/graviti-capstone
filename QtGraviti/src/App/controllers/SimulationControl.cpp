#include "SimulationControl.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"
#include "GravitiLib/EntityManager.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"


SimulationController::SimulationController(QObject* parent)
    : QObject(parent)
{
    //m_entityList = new EntityListGUI(); //new? shared ptr?

}

SimulationController::~SimulationController()
{
}

void SimulationController::loadJson(std::string filepathjsonPath)
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

float SimulationController::cleanFloat(std::string value)
{
	if (value.back() == 'f') {
		value.pop_back();
	}
	return std::strtof(value.c_str(), nullptr);
}