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
	Entity targetEntity = Entity();
    
	OptimizationEntityManager();
    int entityManagerCount();

    void addEntity(Entity& entity);
    void addTargetEntity(Entity& entity);

    std::vector<Entity> getAllEntities();

	void loadTargetPoint(Vec3 targetPosition);
    void clearEntities();
    void run(const int totalTimeSteps, const float timeStep);
	
	Vec3 DetermineMinimumDistancePoint();
private:
    //OptimizationEntityManager* entity_manager;
    long int m_nextID;


    std::vector<Entity> entities;

};

#endif // OptimizationEntityManager_INCLUDE_GUARD_H