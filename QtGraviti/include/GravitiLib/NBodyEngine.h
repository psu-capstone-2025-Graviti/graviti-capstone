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
	void calculateForces(float duration, Entity& callingEntity);
	void updatePosition(float duration, Entity& callingEntity);
};

#endif // NBodyEngine_INCLUDE_GUARD_H