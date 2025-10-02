#include "GravitiLib/EntityManager.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <GravitiLib/NBodyEngine.h>
#include "GravitiLib/IPhysicsEngine.h"

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

std::shared_ptr<std::vector<Entity>> EntityManager::getAllEntities()
{
    return entities;
}

void EntityManager::addEntityFromJson(std::string filepathjsonPath)
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
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<NBodyPhysics>();
    rapidjson::Value::ConstValueIterator itr;

    for (auto itr = d.Begin(); itr != d.End(); ++itr) {
       
            
        physicsEngine = std::make_unique<NBodyPhysics>();
        Entity jsonEntity = Entity(physicsEngine);
        PhysicalState jsonEntityState;
        auto obj = itr->GetObject();
        jsonEntity.setEntityName(obj["name"].GetString());
        

        jsonEntityState.setPosition(X, obj["positionX"].GetFloat());
        jsonEntityState.setPosition(Y, obj["positionY"].GetFloat());
        jsonEntityState.setPosition(Z, obj["positionZ"].GetFloat());
        const char* massStr = obj["mass"].GetString();

        // Remove trailing 'f' if present
        std::string cleaned = massStr;
        if (cleaned.back() == 'f') {
            cleaned.pop_back();
        }

        // Convert to float
        float mass = std::strtof(cleaned.c_str(), nullptr);
        jsonEntityState.setMass(mass);

        jsonEntity.setOrigin(jsonEntityState);
        jsonEntity.setID(m_nextID++);
        entities->push_back(jsonEntity);
        
    }


}

