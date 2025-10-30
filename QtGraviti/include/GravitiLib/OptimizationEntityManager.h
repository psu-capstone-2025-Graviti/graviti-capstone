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
    OptimizationEntityManager(const OptimizationEntityManager&) = delete;
    OptimizationEntityManager& operator=(const OptimizationEntityManager&) = delete;

    static OptimizationEntityManager* getInstance();

    void addEntity(Entity& entity);

    std::vector<Entity> getAllEntities();

    //DANGER
	void loadTargetPoint(Vec3 targetPosition);
    void clearEntities();

private:
    //OptimizationEntityManager* entity_manager;
    long int m_nextID;


    std::vector<Entity> entities;

    // std::unordered_map<long int, std::shared_ptr<Entity>> planetoids; //TODO - eventually will need to split these out to make simulation faster
    // std::unordered_map<long int, std::shared_ptr<Entity>> satellites;

    //long int next_entity_id; //TODO - will we need entity ids?
};

#endif // OptimizationEntityManager_INCLUDE_GUARD_H