#include "GravitiLib/NBodyEngine.h"
#include <iostream>

bool debugCSV = false;
bool debugLog = false;


void NBodyPhysics::simulate(float duration, int timeSteps, PhysicalState& currentState, long int callingID, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory)
{
	auto entManager = EntityManager::getInstance();

	m_entities = entManager->getAllEntities();

    float currentTime = currentState.getTimestamp();


    for (int step = 0; step < timeSteps; step++) {
        //std::cout << "Current Time: " << currentTime << " seconds" << std::endl;

        Vec3 totalAcc = { 0.0, 0.0, 0.0 };
        for (auto jt = m_entities->begin(); jt != m_entities->end(); ++jt) {
            if (jt->getEntityID() != callingID) { // Avoid self-interaction
                
                auto other_state = jt->getPhysicalState();
                auto self_state = currentState;


                float dx = other_state->getPosition().x - self_state.getPosition().x;
                float dy = other_state->getPosition().y - self_state.getPosition().y;
                float dz = other_state->getPosition().z - self_state.getPosition().z;
                float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
                if (distance > std::numeric_limits<float>::epsilon()) { // Prevent division by zero
                    float accMagnitude = G * jt->getPhysicalState()->getMass() / (distance * distance);
                    totalAcc.x += (accMagnitude * (dx / distance));
                    totalAcc.y += (accMagnitude * (dy / distance));
                    totalAcc.z += (accMagnitude * (dz / distance));
                }
                else { // Prevent division by zero
                    totalAcc.x = 0;
                    totalAcc.y = 0;
                    totalAcc.z = 0;
                }
            }
        }
        currentState.setAcceleration(totalAcc);
        currentTime += timestep;

        if (debugLog){
            currentState.printPosition();
}
    }
	
}
