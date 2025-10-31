#ifndef RealTimeSimEnvironment_INCLUDE_GUARD_H
#define RealTimeSimEnvironment_INCLUDE_GUARD_H

#include "EntityManager.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdint>
#include <functional>

 

class RealTimeSimEnvironment
{
public:
	RealTimeSimEnvironment();
	~RealTimeSimEnvironment();

	void run();   //Sets m_run = true
	void pause(); //Sets m_run = false

    void setUpdateFunction(std::function<void()> updatefunc);

	void resetSimulation(); //Resets time back to origin, resets entities to origins, pauses sim

	double getOriginTime() const;

	double setTimestepSize(/*TODO - chrono or double?*/);

private:
	//Actual thread function - starts running on construction
	void run_realtimeSim();
	void step_simulation(double simTime);
	//Sim values
	double m_origin_time;
	double m_accumulated_time;
	std::thread sim_thread;
	//Thread communication fields
	std::atomic<bool> thread_enabled;
	std::atomic<bool> run_simulation;

	std::mutex calculating;


	std::atomic<int> simulation_time_scalar;

	std::atomic<std::chrono::milliseconds> time_step_size;

	std::atomic<std::chrono::nanoseconds> accumulated_time;
	

    // Simulation needs to be able to flag the GUI to update
    // When the sim finishes a tick, it calls this function and generates the update
    std::function<void()> update_function;

};

#endif // BatchSimEnvironment_INCLUDE_GUARD_H