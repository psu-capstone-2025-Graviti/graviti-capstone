#include "GravitiLib/Entity.h"
#include <algorithm>
#include <memory>
#include <string>


Entity::Entity(std::unique_ptr<IPhysicsEngine>& engine)
    : m_current_state(),
    m_entity_name("TEST"),
    m_future_trajectory(),
    m_past_trajectory(),
    m_entityid(0),
    m_timestep(0.0f),
    m_file(nullptr)
{
    m_engine = std::move(engine);

}

Entity::~Entity()
{
    if (m_file) {
        m_file->close();
        delete m_file;
    }
}

void Entity::setEntityName(std::string entity_name)
{
    m_entity_name = entity_name;
}
std::string Entity::getEntityName()
{
    return m_entity_name;
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

float Entity::getTimestep()
{
    return m_timestep;
};

void Entity::setOrigin(PhysicalState origin)
{
    m_current_state = origin;
    
    // Clear trajectories when setting new origin
    m_past_trajectory.clear();
    m_future_trajectory.clear();
}

//void Entity::setPhysicsEngine()
//{
//    m_engine = std::move(engine);
//}

PhysicalState* Entity::getPhysicalState()
{
    return &m_current_state;
}

bool Entity::Simulate(float duration)
{
    if (!m_engine) {
        // No physics engine set, cannot simulate
        return false;
    }
    
    // Store current state in past trajectory before simulation
    m_past_trajectory.push_back(m_current_state);//Make copy

    m_future_trajectory.clear(); //TODO - we can use the future trajectory if its still valid

    // Use the physics engine to simulate one timestep
    m_engine->calculateForces(duration, *this);

    return true;
}

bool Entity::TickForward()
{
    //if(m_future_trajectory.size() < 1) {
    //    return false;
    //}
    //m_current_state = m_future_trajectory[0];
    //m_future_trajectory.erase(m_future_trajectory.begin());
    m_engine->updatePosition(m_timestep, *this);
    return true;
}

void Entity::setID(long int newID)
{
    m_entityid = newID;
}

std::vector<PhysicalState>& Entity::getFutureTrajectory()
{
    return m_future_trajectory;
}

std::vector<PhysicalState>& Entity::getPastTrajectory()
{
    return m_past_trajectory;
}

long int Entity::getEntityID() const
{
    return m_entityid;
}

void Entity::savePastTrajectoryToCSV(void)
{
    std::string filename = m_entity_name + ".csv";

    if (!m_file) {
        m_file = new std::ofstream(filename);
        if (m_file->is_open()) {
            *m_file << "x,y,z\n";
        }
    }

    for (const auto& state : m_past_trajectory) {
        *m_file << state.getPosition(X) << ","
                << state.getPosition(Y) << ","
                << state.getPosition(Z) << "\n";
    }
    m_file->flush();
}

void Entity::saveCurrentStateToCSV(void)
{
    //return;
    // if "name.csv" file does not exist, create it and add header
    // else, append to it
    std::string filename = m_entity_name + ".csv";

    if (!m_file) {
        m_file = new std::ofstream(filename);
        if (m_file->is_open()) {
            *m_file << "x,y,z\n";
        }
    }

    if (m_file && m_file->is_open()) {
        *m_file << getPhysicalState()->getPosition(X) << ","
                << getPhysicalState()->getPosition(Y) << ","
                << getPhysicalState()->getPosition(Z) << "\n";
        m_file->flush();
    }
}