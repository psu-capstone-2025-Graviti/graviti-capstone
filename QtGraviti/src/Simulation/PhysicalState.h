#pragma once
#include <chrono>

using namespace std::chrono;

#define X 0
#define Y 1
#define Z 2

struct Vec3 {
    float x;
    float y;
    float z;
};

class PhysicalState
{
private:
    // Position in 3D space (x, y, z coordinates)
    Vec3 m_position;
    
    // Velocity in 3D space (vx, vy, vz components)
    Vec3 m_velocity;
    
    // Acceleration in 3D space (ax, ay, az components)
	Vec3 m_acceleration;
    
    // Timestamp for this PhysicalState data
    float m_timestamp;

    float m_mass;

    float m_radius;

public:
    // Default constructor
    PhysicalState();
    
    // Constructor with initial values
    PhysicalState(Vec3 pos, Vec3 vel, Vec3 acc, float mass, float radius);
    
    // Destructor
    ~PhysicalState();
    
    // Getters 
    float getPosition(int index) const;
	Vec3 getPosition() const;
    float getVelocity(int index) const;
	Vec3 getVelocity() const;
    float getAcceleration(int index) const;
	Vec3 getAcceleration() const;
    float getTimestamp() const;
    
    // Setters 
    void setPosition(Vec3 pos);
    void setPosition(int index, float value);
    void setVelocity(Vec3 vel);
    void setVelocity(int index, float value);
    void setAcceleration(Vec3 acc);
    void setAcceleration(int index, float value);
    void setTimestamp(const float timestamp);
    //void setTimestampNow();
    
    // Reset all values to zero
    void reset();
    
    // Copy constructor
    PhysicalState(const PhysicalState& other);
    
    // Assignment operator
    PhysicalState& operator=(const PhysicalState& other);
};

