#include "GravitiLib/OptimizationEntityManager.h"
#include <GravitiLib/NBodyEngine.h>
#include "GravitiLib/IPhysicsEngine.h"


OptimizationEntityManager::OptimizationEntityManager()
{
    std::vector<Entity> entities;
	minimumDistancePoint = { 0.0f, 0.0f, 0.0f };
	m_nextID = 0;
	targetPoint = { 0.0f, 0.0f, 0.0f };

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

int OptimizationEntityManager::entityManagerCount()
{
    return entities.size();
}

