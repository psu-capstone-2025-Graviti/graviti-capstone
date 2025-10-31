#include "GravitiLib/OptimizationEntityManager.h"
#include <GravitiLib/NBodyEngine.h>
#include "GravitiLib/IPhysicsEngine.h"


OptimizationEntityManager::OptimizationEntityManager()
{
    std::vector<Entity> entities;

}


void OptimizationEntityManager::addEntity(Entity& entity)
{
    entity.setID(m_nextID++);
    entities.push_back(entity);
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

