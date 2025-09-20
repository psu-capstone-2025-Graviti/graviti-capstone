#include "NBodyEngine.h"
#include <iostream>

bool debugCSV = true;
bool debugLog = true;


void NBodyPhysics::simulate(float duration, int timeSteps, PhysicalState& currentState, long int callingID, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory)
{
	auto entManager = EntityManager::getInstance();

	m_entities = entManager->getAllEntities();

    float currentTime = currentState.getTimestamp();

    for (int step = 0; step < timeSteps; step++) {
        //std::cout << "Current Time: " << currentTime << " seconds" << std::endl;

        float totalAcc[3] = { 0.0f, 0.0f, 0.0f };
        for (auto jt = m_entities->begin(); jt != m_entities->end(); ++jt) {
            if (jt->getEntityID() != callingID) { // Avoid self-interaction
                
                auto other_state = jt->getPhysicalState();
                auto self_state = currentState;
                

                float dx = other_state->getPosition().x - self_state.getPosition().x;
                float dy = other_state->getPosition().y - self_state.getPosition().y;
                float dz = other_state->getPosition().z - self_state.getPosition().z;
                float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
                if (distance > 1e-5f) { // Prevent division by zero
                    float accMagnitude = G * jt->getPhysicalState()->getMass() / (distance * distance);
                    totalAcc[X] += (accMagnitude * (dx / distance));
                    totalAcc[Y] += (accMagnitude * (dy / distance));
                    totalAcc[Z] += (accMagnitude * (dz / distance));
                }
                else { // Prevent division by zero
                    totalAcc[X] = 0;
                    totalAcc[Y] = 0;
                    totalAcc[Z] = 0;
                }
            }
        }
        Vec3 sendTotalAcc = { totalAcc[X], totalAcc[Y], totalAcc[Z] };

        currentState.addAcceleration(sendTotalAcc);
        currentTime += timestep;
    }
	
}

//class simulation {
//    float currentTime = 0.0f;
//    float timestep = 1.0f;
//    float endTime = 10.0f;
//    //auto planetoids = entities;
//
//    void step() {
//        // Calculate gravitational forces and update positions
//
//        calcGravForces();
//
//        for (auto it = m_entities->begin(); it != m_entities->end(); ++it) {
//            // Update position based on velocity and acceleration
//
//            it->PhysicalState::integrate(timestep);
//            if (debugLog)
//            {
//                it->PhysicalState::printPosition();
//            }
//
//            if (debugCSV) {
//                it->PhysicalState::writepositionCSV();
//            }
//        }
//    }
//    void calcGravForces() {
//        for (auto it = entities.begin(); it != entities.end(); ++it) {
//            float[3] totalAcc = { 0.0f, 0.0f, 0.0f };
//            for (auto jt = entities.begin(); jt != entities.end(); ++jt) {
//                if (it != jt) { // Avoid self-interaction
//                    float dx = jt->PhysicalState::getPosition(currentTime)[X] - it->PhysicalState::getPosition(currentTime)[X];
//                    float dy = jt->PhysicalState::getPosition(currentTime)[Y] - it->PhysicalState::getPosition(currentTime)[Y];
//                    float dz = jt->PhysicalState::getPosition(currentTime)[Z] - it->PhysicalState::getPosition(currentTime)[Z];
//                    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
//                    if (distance > 1e-5f) { // Prevent division by zero
//                        float accMagnitude = G * jt->PhysicalState::getMass() / (distance * distance);
//                        totalAcc[X] += (accMagnitude * (dx / distance));
//                        totalAcc[Y] += (accMagnitude * (dy / distance));
//                        totalAcc[Z] += (accMagnitude * (dz / distance));
//                    }
//                    else { // Prevent division by zero
//                        totalAcc[X] = 0;
//                        totalAcc[Y] = 0;
//                        totalAcc[Z] = 0;
//                    }
//                }
//            }
//            Vec3 sendTotalAcc = { totalAcc[X], totalAcc[Y], totalAcc[Z] };
//            it->PhysicalState::setAcceleration(sendTotalAcc);
//        }
//
//
//    }
//
//
//public:
//
//    // getter method to access planetoids
//    std::vector<Entity>> getPlanetoids() {
//        return planetoids;
//    }
//
//    simulation(float ct, float ts, float et) {
//
//    }
//
//
//    void addObject(Entity obj) {
//        planetoids.push_back(obj);
//    }
//
//    void solve() {
//
//    }
//}