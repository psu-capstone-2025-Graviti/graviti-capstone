#ifndef Entity_INCLUDE_GUARD_H
#define Entity_INCLUDE_GUARD_H

#include "PhysicalState.h"
#include "IPhysicsEngine.h"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

using namespace std::chrono;


class Entity
{
public:
	Entity(std::shared_ptr<IPhysicsEngine>& engine);
	~Entity();

	void setTickDuration(float tick_duration);
	void setTimestep(float timestep);
	float getTimestep();
	void setOrigin(PhysicalState origin);
	PhysicalState getOrigin() const;
	void resetToOrigin();
	//void setPhysicsEngine(std::unique_ptr<IPhysicsEngine>& engine);

	PhysicalState* getPhysicalState();

	void setEntityName(std::string entity_name);
	std::string getEntityName();

	long int getEntityID() const;

	//How many time steps to simulate forward
	bool Simulate(float duration);

	//Tick forward one time step
	bool TickForward();

	void setID(long int newID);

	void saveCurrentStateToCSV(void);    //These two are not really compatible with one another, we need to decide which to use
	void savePastTrajectoryToCSV(void);  //These two are not really compatible with one another, we need to decide which to use

	// Get trajectories for trajectory rendering
	std::vector<PhysicalState>& getFutureTrajectory();
	std::vector<PhysicalState>& getPastTrajectory();

	void setTexturePath(const std::string& texturePath);
	std::string getTexturePath() const;

	//void setFlatIconRepr(std::shared_ptr< FlatIcon > flatIcon);
	//std::shared_ptr< FlatIcon > setFlatIconRepr();
private:
	std::ofstream* m_file;
	std::string m_entity_name;

	std::string m_texturePath;
	//std::shared_ptr< FlatIcon > m_fi_repr;

	std::shared_ptr<IPhysicsEngine> m_engine; //needs to be shared ptr?
	float m_timestep;
	std::vector<PhysicalState> m_past_trajectory;
	std::vector<PhysicalState> m_future_trajectory;
	PhysicalState m_current_state;


	PhysicalState m_origin;
	bool m_origin_set;
	long int m_entityid;
};

#endif // Entity_INCLUDE_GUARD_H