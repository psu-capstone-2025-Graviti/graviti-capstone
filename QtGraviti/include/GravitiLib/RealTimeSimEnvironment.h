#ifndef RealTimeSimEnvironment_INCLUDE_GUARD_H
#define RealTimeSimEnvironment_INCLUDE_GUARD_H

#include "EntityManager.h"
#include <atomic>

using namespace std::chrono;

class RealTimeSimEnvironment
{
public:
	RealTimeSimEnvironment();
	~RealTimeSimEnvironment();

	void run();   //Sets m_run = true
	void pause(); //Sets m_run = false

	void setUpdateFunction(void (*updatefunc)(void));

	void resetSimulation(); //Resets time back to origin, resets entities to origins, pauses sim

	double getOriginTime() const;


private:
	//Actual thread function - starts running on construction
	void run_realtimeSim();
	//Sim values
	double m_origin_time;
	std::thread sim_thread;
	//Thread communication fields
	std::atomic<bool> thread_enabled;
	std::atomic<bool> run_simulation;

	std::mutex calculating;


	std::atomic<int> simulation_time_scalar;

	std::atomic<std::chrono::milliseconds> time_step_size;
	std::atomic<std::chrono::milliseconds> accumulated_time;
	

	//Simulation needs to be able to flag the GUI to update
	//Easiest way to do this is to set a function pointer that creates the QT message
	//When the sim finishes a tick, it calls that function and generates the update
	void (*update_function_ptr)(void);

};

#endif // BatchSimEnvironment_INCLUDE_GUARD_H