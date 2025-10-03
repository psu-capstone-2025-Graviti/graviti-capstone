#ifndef NBodyEngine_INCLUDE_GUARD_H
#define NBodyEngine_INCLUDE_GUARD_H

#include "IPhysicsEngine.h"
#include "EntityManager.h"

class NBodyPhysics : public IPhysicsEngine
{
private:
	//not sure if we set current time and end time here vs in the cpp file
	float G = 6.67430e-11f; // Gravitational constant in km^3 kg^-1 s^-2

	float ScalingFactor = 10; // Scale distances to make simulation more manageable
	float D = 1; // Duration of one day in seconds 1 day = 86400 seconds	| replace Time
	float S = 1; // Mass of the the sun in AU 1.989e+30 kg					| replace mass
	float AU = 1; // Distance from the sun to the earth in 1.496e+8 km = 1 AU| replace meter
	// 6.67*10^-11 N*M^2/kg^2 = 39.5 AU^3/(year^2*m) = 2.959e-4 AU^3/(day^2*m) 
	float k = 2.959e-4f; // AU^3/(day^2*m) gravitational constant in		| replace G


	float timestep = 1.0f; // Time step in seconds
	float currentTime = 0.0f;
	float endTime = 365; //one year
	std::shared_ptr<std::vector<Entity>> m_entities;

public:
	void simulate(float duration, int timeSteps, PhysicalState& currentState, long int callingID, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory);
};

#endif // NBodyEngine_INCLUDE_GUARD_H