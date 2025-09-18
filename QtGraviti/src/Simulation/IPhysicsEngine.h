#pragma once

#include "PhysicalState.h"
#include <chrono>
#include <memory>
#include <vector>

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
     * Runs the simulation for the given number of steps
     * 
     * duration          The duraction of each time step
     * timeSteps         The total number of time steps to simulate
     * currentState      The current state of the entity
     * futureTrajectory  Pointer to where to store the future trajectory of the entity
     */
    virtual void simulate(float duration, int timeSteps, PhysicalState& currentState, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory) = 0;

};