#ifndef Entity_INCLUDE_GUARD_H
#define Entity_INCLUDE_GUARD_H

#include "PhysicalState.h"
#include "IPhysicsEngine.h"
#include <vector>
#include <memory>

using namespace std::chrono;

class Entity
{
public:
	Entity(std::unique_ptr<IPhysicsEngine>& engine);
	~Entity();

	void setTickDuration(float tick_duration);
	void setTimestep(float timestep);
	float getTimestep();
	void setOrigin(PhysicalState origin);
	//void setPhysicsEngine(std::unique_ptr<IPhysicsEngine>& engine);

	PhysicalState* getPhysicalState();

	void setEntityName(std::string entity_name);
	std::string getEntityName();

	long int getEntityID() const;

	//How many time steps to simulate forward
	bool Simulate(int time_steps);

	//Tick forward one time step
	bool TickForward();

	void setID(long int newID);

private:
	std::string m_entity_name;


	std::shared_ptr<IPhysicsEngine> m_engine; //needs to be shared ptr?
	float m_timestep;
	std::vector<PhysicalState> m_past_trajectory;
	std::vector<PhysicalState> m_future_trajectory;
	PhysicalState m_current_state;

	long int m_entityid;
};

#endif // Entity_INCLUDE_GUARD_H