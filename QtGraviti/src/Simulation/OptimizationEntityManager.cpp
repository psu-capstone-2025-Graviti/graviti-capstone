#include "OptimizationEntityManager.h"
#include "GravitiLib/OptimizationEntityManager.h"
#include <GravitiLib/NBodyEngine.h>
#include "GravitiLib/IPhysicsEngine.h"


OptimizationEntityManager::OptimizationEntityManager()
    : entities(std::make_shared<std::vector<Entity>>()),
    m_nextID(0)
{
}

OptimizationEntityManager::~OptimizationEntityManager()
{
    entities->clear();
}

OptimizationEntityManager OptimizationEntityManager::getInstance()
{
    if(entity_manager == nullptr) {
        entity_manager = new OptimizationEntityManager();
    }   
    return entity_manager;
}

void OptimizationEntityManager::addEntity(Entity& entity)
{
    entity.setID(m_nextID++);
    entities->push_back(entity);
}


std::shared_ptr<std::vector<Entity>> OptimizationEntityManager::getAllEntities()
{
    return entities;
}

void OptimizationEntityManager::loadTargetPoint(Vec3 targetPosition)
{
	targetPoint = targetPosition;
}

void OptimizationEntityManager::clearEntities()
{
    entities->clear();
}

