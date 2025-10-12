#include "GravitiLib/NBodyEngine.h"
#include <iostream>


Vec3 NBodyPhysics::gravitationalAcceleration(Entity A, Entity B) {
    Vec3 totalAcc = { 0.0, 0.0, 0.0 };

    float dx = A.getPhysicalState()->getPosition().x - B.getPhysicalState()->getPosition().x;
    float dy = A.getPhysicalState()->getPosition().y - B.getPhysicalState()->getPosition().y;
    float dz = A.getPhysicalState()->getPosition().z - B.getPhysicalState()->getPosition().z;

    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (distance > std::numeric_limits<float>::epsilon()) { // Prevent division by zero
        float accMagnitude = G * A.getPhysicalState()->getMass() / (distance * distance);
        totalAcc.x += (accMagnitude * (dx / distance));
        totalAcc.y += (accMagnitude * (dy / distance));
        totalAcc.z += (accMagnitude * (dz / distance));
    }
    else { // Prevent division by zero
        totalAcc.x = 0;
        totalAcc.y = 0;
        totalAcc.z = 0;
    }

	return totalAcc;
}

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
                
            auto other_state = jt->getPhysicalState();

            float dx = other_state->getPosition().x - self_state->getPosition().x;
            float dy = other_state->getPosition().y - self_state->getPosition().y;
            float dz = other_state->getPosition().z - self_state->getPosition().z;

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
    self_state->setAcceleration(totalAcc);
}




void NBodyPhysics::updatePositionRk4(float duration, Entity& callingEntity)
{
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
                         newZ });

    state->setVelocity({ vel.x + acc.x * duration,
                         vel.y + acc.y * duration,
                         vel.z + acc.z * duration });
}



void NBodyPhysics::updatePosition(float duration, Entity& callingEntity)
{
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

void NBodyPhysics::rk4Step(Entity& entityA, Entity& entityB, double dt)
{
    Vec3 position = entityA.getPhysicalState()->getPosition();
    Vec3 velocity = entityA.getPhysicalState()->getVelocity();

    // k1
    Vec3 k1_v = gravitationalAcceleration(entityA, entityB);
    Vec3 k1_p = velocity;

    // k2
    Vec3 tempPos_k2 = { position.x + k1_p.x * (dt / 2.0), position.y + k1_p.y * (dt / 2.0), position.z + k1_p.z * (dt / 2.0) };
    Vec3 tempVel_k2 = { velocity.x + k1_v.x * (dt / 2.0), velocity.y + k1_v.y * (dt / 2.0), velocity.z + k1_v.z * (dt / 2.0) };
    // Create temporary PhysicalState for k2
    PhysicalState tempState_k2(tempPos_k2, tempVel_k2, {0,0,0}, entityA.getPhysicalState()->getMass(), entityA.getPhysicalState()->getRadius(), entityA.getPhysicalState()->getTimestamp());
    Entity tempEntityA_k2 = entityA; // Copy entityA
    tempEntityA_k2.setOrigin(tempState_k2);
    Vec3 k2_v = gravitationalAcceleration(tempEntityA_k2, entityB);
    Vec3 k2_p = tempVel_k2;

    // k3
    Vec3 tempPos_k3 = { position.x + k2_p.x * (dt / 2.0), position.y + k2_p.y * (dt / 2.0), position.z + k2_p.z * (dt / 2.0) };
    Vec3 tempVel_k3 = { velocity.x + k2_v.x * (dt / 2.0), velocity.y + k2_v.y * (dt / 2.0), velocity.z + k2_v.z * (dt / 2.0) };
    PhysicalState tempState_k3(tempPos_k3, tempVel_k3, {0,0,0}, entityA.getPhysicalState()->getMass(), entityA.getPhysicalState()->getRadius(), entityA.getPhysicalState()->getTimestamp());
    Entity tempEntityA_k3 = entityA;
    tempEntityA_k3.setOrigin(tempState_k3);
    Vec3 k3_v = gravitationalAcceleration(tempEntityA_k3, entityB);
    Vec3 k3_p = tempVel_k3;

    // k4
    Vec3 tempPos_k4 = { position.x + k3_p.x * dt, position.y + k3_p.y * dt, position.z + k3_p.z * dt };
    Vec3 tempVel_k4 = { velocity.x + k3_v.x * dt, velocity.y + k3_v.y * dt, velocity.z + k3_v.z * dt };
    PhysicalState tempState_k4(tempPos_k4, tempVel_k4, {0,0,0}, entityA.getPhysicalState()->getMass(), entityA.getPhysicalState()->getRadius(), entityA.getPhysicalState()->getTimestamp());
    Entity tempEntityA_k4 = entityA;
    tempEntityA_k4.setOrigin(tempState_k4);
    Vec3 k4_v = gravitationalAcceleration(tempEntityA_k4, entityB);
    Vec3 k4_p = tempVel_k4;

    // Update position and velocity
    position.x += (k1_p.x + 2.0 * k2_p.x + 2.0 * k3_p.x + k4_p.x) * (dt / 6.0);
    position.y += (k1_p.y + 2.0 * k2_p.y + 2.0 * k3_p.y + k4_p.y) * (dt / 6.0);
    position.z += (k1_p.z + 2.0 * k2_p.z + 2.0 * k3_p.z + k4_p.z) * (dt / 6.0);

    velocity.x += (k1_v.x + 2.0 * k2_v.x + 2.0 * k3_v.x + k4_v.x) * (dt / 6.0);
    velocity.y += (k1_v.y + 2.0 * k2_v.y + 2.0 * k3_v.y + k4_v.y) * (dt / 6.0);
    velocity.z += (k1_v.z + 2.0 * k2_v.z + 2.0 * k3_v.z + k4_v.z) * (dt / 6.0);

    entityA.getPhysicalState()->setPosition(position);
    entityA.getPhysicalState()->setVelocity(velocity);
}