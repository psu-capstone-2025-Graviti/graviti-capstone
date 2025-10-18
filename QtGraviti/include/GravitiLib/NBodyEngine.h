#ifndef NBodyEngine_INCLUDE_GUARD_H
#define NBodyEngine_INCLUDE_GUARD_H

#include "IPhysicsEngine.h"
#include "EntityManager.h"
#include "Entity.h"

class NBodyPhysics : public IPhysicsEngine
{
private:
	float G = 6.67430e-11f; // Gravitational constant in km^3 kg^-1 s^-2
public:
	Vec3 gravitationalAcceleration(Entity& A, Entity& B);
	void calculateForces(float duration, Entity& callingEntity);
	void updatePositionRk4(float duration, Entity& callingEntity);
	void calculateForces2(float duration, Entity& callingEntity);
	void updatePosition(float duration, Entity& callingEntity);
	void rk4Step(Entity& entityA, Entity& entityB, float dt);
};

#endif // NBodyEngine_INCLUDE_GUARD_H