#ifndef OptimizationEntityManager_INCLUDE_GUARD_H
#define OptimizationEntityManager_INCLUDE_GUARD_H


#include "Entity.h"
#include <memory>
#include <mutex>
#include <vector>
#include <string>


class OptimizationEntityManager
{
public:
	Vec3 targetPoint;
	Vec3 minimumDistancePoint;
    
	OptimizationEntityManager();
    int entityManagerCount();

    void addEntity(Entity& entity);

    std::vector<Entity> getAllEntities();

	void loadTargetPoint(Vec3 targetPosition);
    void clearEntities();

private:
    //OptimizationEntityManager* entity_manager;
    long int m_nextID;


    std::vector<Entity> entities;

};

#endif // OptimizationEntityManager_INCLUDE_GUARD_H