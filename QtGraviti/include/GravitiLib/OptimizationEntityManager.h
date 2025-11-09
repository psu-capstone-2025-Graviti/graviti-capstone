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
	Entity targetEntity = Entity();
    
	OptimizationEntityManager();
    int entityManagerCount();

    void addEntity(Entity& entity);
    void addTargetEntity(Entity entity);

    std::vector<Entity> getAllEntities();

	void loadTargetPoint(Vec3 targetPosition);
    void clearEntities();
    void run(const int totalTimeSteps, const float timeStep);
	
	Vec3 DetermineMinimumDistancePoint();

	double calculateDistance(Vec3 a, Vec3 b);
	double getShortestMagnitude() const { return ShortestMagnitude; }
private:
    //OptimizationEntityManager* entity_manager;
    long int m_nextID;
    double ShortestMagnitude = 0;
    Vec3 minimumDistancePoint;

    std::vector<Entity> entities;

};

#endif // OptimizationEntityManager_INCLUDE_GUARD_H