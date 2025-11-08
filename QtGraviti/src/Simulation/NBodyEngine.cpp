#include "GravitiLib/NBodyEngine.h"
#include <iostream>



void NBodyPhysics::calculateForces(float duration, Entity& callingEntity)
{
	auto entManager = EntityManager::getInstance();

	auto m_entities = entManager->getAllEntities();

    long int callingID = callingEntity.getEntityID();
    float currentTime = callingEntity.getPhysicalState()->getTimestamp();

    Vec3 totalAcc = { 0.0, 0.0, 0.0 };
    auto self_state = callingEntity.getPhysicalState();

    for (auto jt = m_entities->begin(); jt != m_entities->end(); ++jt) {
        if (jt->getEntityID() != callingID) { // Avoid self-interaction
            if (jt->isunAlive())
            {
                continue; //Don't calculate forces from a dead entity
            }

            auto other_state = jt->getPhysicalState();

            float dx = other_state->getPosition().x - self_state->getPosition().x;
            float dy = other_state->getPosition().y - self_state->getPosition().y;
            float dz = other_state->getPosition().z - self_state->getPosition().z;

            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

            if (distance < (other_state->getRadius() + self_state->getRadius()))
            {
                if (other_state->getMass() >= self_state->getMass())
                { //Other is bigger, we unalive
                    callingEntity.setEntityForDistruction();
                }
                else
                { //We're bigger, other unalives
                    jt->setEntityForDistruction();
                }
            }

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
    self_state->setAcceleration(totalAcc);
}



void NBodyPhysics::updatePosition(float duration, Entity& callingEntity)
{
    callingEntity.updateEntityDistruction();
    if (callingEntity.isunAlive())
    {
        return;
    }

    auto state = callingEntity.getPhysicalState();
    auto oldPosition = state->getPosition();
    auto vel = state->getVelocity();
    auto acc = state->getAcceleration();

    //Constant acceleration integration - Need RK4 to be stable

    float newX = oldPosition.x + ((vel.x * duration) + (0.5 * acc.x * (duration * duration)));
    float newY = oldPosition.y + ((vel.y * duration) + (0.5 * acc.y * (duration * duration)));
    float newZ = oldPosition.z + ((vel.z * duration) + (0.5 * acc.z * (duration * duration)));

    state->setPosition({ newX,
                         newY,
                         newZ});

    state->setVelocity({ vel.x + acc.x * duration,
                         vel.y + acc.y * duration,
                         vel.z + acc.z * duration });

    
}

