#include "GravitiLib/EntityManager.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <GravitiLib/NBodyEngine.h>

static EntityManager* entity_manager = nullptr;

EntityManager::EntityManager()
    : entities(std::make_shared<std::vector<Entity>>()),
    m_nextID(0)
{
}

EntityManager::~EntityManager()
{
    entities->clear();
}

EntityManager* EntityManager::getInstance()
{
    if(entity_manager == nullptr) {
        entity_manager = new EntityManager();
    }   
    return entity_manager;
}

void EntityManager::addEntity(Entity& entity)
{
    entity.setID(m_nextID++);
    entities->push_back(entity);
}


void EntityManager::addEntityFromJson(std::string filepathjsonPath, std::unique_ptr<IPhysicsEngine> physicsEngine)
{
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

    Entity jsonEntity = Entity(physicsEngine);

    jsonEntity.setEntityName(d["name"].GetString());
    PhysicalState jsonEntityState;
    jsonEntityState.setPosition(X, d["positionX"].GetFloat());
    jsonEntityState.setPosition(Y, d["positionY"].GetFloat());
    jsonEntityState.setPosition(Z, d["positionZ"].GetFloat());
    jsonEntityState.setMass(d["mass"].GetFloat());
    jsonEntity.setOrigin(jsonEntityState);

    jsonEntity.setID(m_nextID++);
    entities->push_back(jsonEntity);
}

std::shared_ptr<std::vector<Entity>> EntityManager::getAllEntities()
{
    return entities;
}
