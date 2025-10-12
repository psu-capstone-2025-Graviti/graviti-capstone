#include "GravitiLib/PhysicalState.h"
#include <iostream>
#include <fstream>

using namespace std::chrono;


PhysicalState::PhysicalState() 
    : m_position{0.0, 0.0, 0.0}
    , m_velocity{0.0, 0.0, 0.0}
    , m_acceleration{0.0, 0.0, 0.0}
    , m_timestamp(0.0) //todo - make epoch
    , m_mass(0.0)
    , m_radius(0.0)
{
}


PhysicalState::PhysicalState(Vec3 pos, Vec3 vel, Vec3 acc, float mass, float radius, float timestamp)
{
    m_position = pos;
    m_velocity = vel;
    m_acceleration = acc;
    m_mass = mass;
    m_radius = radius;
    m_timestamp = timestamp;
}


PhysicalState::~PhysicalState()
{
}

float PhysicalState::getPosition(int index) const
{
    switch (index) {
        case X: return m_position.x;
        case Y: return m_position.y;
        case Z: return m_position.z;
        default: return 0.0;
    }
}

Vec3 PhysicalState::getPosition() const
{
    return m_position;
}


float PhysicalState::getVelocity(int index) const
{
    switch (index) {
        case X: return m_velocity.x;
        case Y: return m_velocity.y;
        case Z: return m_velocity.z;
        default: return 0.0;
    }
}

Vec3 PhysicalState::getVelocity() const
{
    return m_velocity;
}

float PhysicalState::getAcceleration(int index) const
{
    switch (index) {
        case X: return m_acceleration.x;
        case Y: return m_acceleration.y;
        case Z: return m_acceleration.z;
        default: return 0.0;
    }
}

Vec3 PhysicalState::getAcceleration() const
{
    return m_acceleration;
}

float PhysicalState::getRadius() const
{
    return m_radius;
}

float PhysicalState::getTimestamp() const
{
    return m_timestamp;
}

void PhysicalState::setMass(float mass)
{
    m_mass = mass;
}

float PhysicalState::getMass() const
{
    return m_mass;
}

void PhysicalState::setPosition(Vec3 pos)
{
    m_position = pos;
}

void PhysicalState::setPosition(int index, float value)
{
    switch (index) {
        case X: m_position.x = value; break;
        case Y: m_position.y = value; break;
        case Z: m_position.z = value; break;
        default: break;
    }
}

void PhysicalState::setVelocity(Vec3 vel)
{
    m_velocity = vel;
}

void PhysicalState::setVelocity(int index, float value)
{
    switch (index) {
        case X: m_velocity.x = value; break;
        case Y: m_velocity.y = value; break;
        case Z: m_velocity.z = value; break;
        default: break;
    }
}

void PhysicalState::setAcceleration(Vec3 acc)
{
    m_acceleration = acc;
}

void PhysicalState::setAcceleration(int index, float value)
{
    switch (index) {
        case X: m_acceleration.x = value; break;
        case Y: m_acceleration.y = value; break;
        case Z: m_acceleration.z = value; break;
        default: break;
    }
}

void PhysicalState::addAcceleration(Vec3 additional)
{
    m_acceleration.x += additional.x;
    m_acceleration.y += additional.y;
    m_acceleration.z += additional.z;
}

void PhysicalState::setTimestamp(const float timestamp)
{
    m_timestamp = timestamp;
}

void PhysicalState::eulerintegrate(float timestep)
{
	auto oldPosition = getPosition();
    auto vel = getVelocity();
    auto acc = getAcceleration();

    setPosition({oldPosition.x + vel.x * timestep,
                 oldPosition.y + vel.y * timestep,
				 oldPosition.z + vel.z * timestep });
    
    setVelocity({vel.x + acc.x * timestep,
				 vel.y + acc.y * timestep,
		         vel.z + acc.z * timestep });

}

void PhysicalState::integrate(float timestep)
{
    auto oldPosition = getPosition();
    auto vel = getVelocity();
    auto acc = getAcceleration();

    setPosition({ oldPosition.x + vel.x * timestep,
                 oldPosition.y + vel.y * timestep,
                 oldPosition.z + vel.z * timestep });

    setVelocity({ vel.x + acc.x * timestep,
                 vel.y + acc.y * timestep,
                 vel.z + acc.z * timestep });

}

void PhysicalState::printPosition() const
{
    std::cout << "Current position: (" << m_position.x << ", " << m_position.y << ", " << m_position.z << ")" << std::endl;
}

void PhysicalState::writepositionCSV(const std::string& name)
{

        // if "name.csv" file does not exist, create it and add header
        // else, append to it
        std::string filename = name + ".csv";
        std::ofstream file{};

        if (!file_opened) {
            file.open(filename, std::ios::out);
            file << "x,y,z\n";
        }
        else {
            file.open(filename, std::ios::app);
            file_opened = true;
        }
        file << m_position.x << "," << m_position.y << "," << m_position.z << "\n";
        file.close();

    
}

bool PhysicalState::fileExists(const std::string& filename) const
{
    std::ifstream file(filename);

    if (file) {
        return true;
        file.close();
    }
    else {
        return false;
    }

}

//void PhysicalState::setTimestampNow()
//{
//    m_timestamp = high_resolution_clock::now();
//}


void PhysicalState::reset()
{
    m_position = {0.0, 0.0, 0.0};
    m_velocity = {0.0, 0.0, 0.0};
    m_acceleration = {0.0, 0.0, 0.0};
    m_timestamp = 0.0;
}

PhysicalState::PhysicalState(const PhysicalState& other)
    : m_position(other.m_position)
    , m_velocity(other.m_velocity)
    , m_acceleration(other.m_acceleration)
    , m_timestamp(other.m_timestamp)
    , m_mass(other.m_mass)
    , m_radius(other.m_radius)
{
}

PhysicalState& PhysicalState::operator=(const PhysicalState& other)
{
    m_position = other.m_position;
    m_velocity = other.m_velocity;
    m_acceleration = other.m_acceleration;
    m_timestamp = other.m_timestamp;
    m_radius = other.m_radius;
    m_mass = other.m_mass;
    return *this;
}
