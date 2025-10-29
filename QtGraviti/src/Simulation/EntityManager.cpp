#include "GravitiLib/EntityManager.h"
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

void EntityManager::clearEntities()
{
    entities->clear();
}

