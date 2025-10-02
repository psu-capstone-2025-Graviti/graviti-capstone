#ifndef EntityManager_INCLUDE_GUARD_H
#define EntityManager_INCLUDE_GUARD_H


#include "Entity.h"
#include <memory>
#include <mutex>
#include <vector>
#include <string>


class EntityManager
{
public:
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    static EntityManager* getInstance();

    void addEntity(Entity& entity);

    void addEntityFromJson(std::string jsonPath);

    std::shared_ptr<std::vector<Entity>> getAllEntities();

private:
    //EntityManager* entity_manager;
    long int m_nextID;
    EntityManager();
    ~EntityManager();

    std::shared_ptr<std::vector<Entity>> entities;

    // std::unordered_map<long int, std::shared_ptr<Entity>> planetoids; //TODO - eventually will need to split these out to make simulation faster
    // std::unordered_map<long int, std::shared_ptr<Entity>> satellites;

    //long int next_entity_id; //TODO - will we need entity ids?
};

#endif // EntityManager_INCLUDE_GUARD_H