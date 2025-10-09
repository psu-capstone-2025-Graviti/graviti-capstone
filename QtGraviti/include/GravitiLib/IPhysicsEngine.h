#ifndef IPhysicsEngine_INCLUDE_GUARD_H
#define IPhysicsEngine_INCLUDE_GUARD_H

#include <chrono>
#include <memory>
#include <vector>

class Entity;

using namespace std::chrono;

/**
 * This interface defines the generic interface for physics engines
 * to be used in the simulation framework.
 * It is used to simulate the physics of the entities in the simulation.
 * The ciritcal function is the simulate function
 *     It takes the PhysicalState of the entity
 *     and stores the result in the futureTrajectory passed in as a shared pointer
 */
class IPhysicsEngine
{
public:
    virtual ~IPhysicsEngine() = default;
    /**
     * Calculates the forces over a duration
     * 
     * duration          The duraction of the time step
     * callingEntity     Reference to the entity that is being simulated
     */
    virtual void calculateForces(float duration, Entity& callingEntity) = 0;

    /**
     * Updates the position of the Entity
     *
     * duration          The duraction of the time step
     * callingEntity     Reference to the entity that is being simulated
     */
    virtual void updatePosition(float duration, Entity& callingEntity) = 0;

};

#endif // IPhysicsEngine_INCLUDE_GUARD_H