#include "Entity.h"
#include <algorithm>
#include <memory>

Entity::Entity(std::unique_ptr<IPhysicsEngine> engine)
    : m_current_state(),
    m_entity_name("TEST"),
    m_future_trajectory(),
    m_past_trajectory()
{
    m_engine = std::move(engine);
}

Entity::~Entity()
{

}

void Entity::setEntityName(std::string entity_name)
{
    m_entity_name = entity_name;
}

//long int Entity::getEntityId() const
//{
//    return entity_id;
//}

void Entity::setTickDuration(float tick_duration)
{
    m_timestep = tick_duration;
}

void Entity::setTimestep(float timestep)
{
    m_timestep = timestep;
}

void Entity::setOrigin(PhysicalState origin)
{
    m_current_state = origin;
    
    // Clear trajectories when setting new origin
    m_past_trajectory.clear();
    m_future_trajectory.clear();
}

void Entity::setPhysicsEngine(std::unique_ptr<IPhysicsEngine> engine)
{
    m_engine = std::move(engine);
}

bool Entity::Simulate(int time_steps)
{
    if (!m_engine) {
        // No physics engine set, cannot simulate
        return false;
    }
    
    m_past_trajectory.push_back(m_current_state);//Make copy

    m_future_trajectory.clear(); //TODO - we can use the future trajectory if its still valid

    // Use the physics engine to simulate one timestep
    m_engine->simulate(m_timestep, time_steps, m_current_state, std::make_shared<std::vector<PhysicalState>>(m_future_trajectory));
    return true;
}

bool Entity::TickForward()
{
    if(m_future_trajectory.size() < 1) {
        if(!this->Simulate(1)) {
            return false;
        }
    }
    m_current_state = m_future_trajectory[0];
    m_future_trajectory.erase(m_future_trajectory.begin());
    return true;
}

void Entity::setID(long int newID)
{
    m_entityid = newID;
}

long int Entity::getEntityID() const
{
    return m_entityid;
}