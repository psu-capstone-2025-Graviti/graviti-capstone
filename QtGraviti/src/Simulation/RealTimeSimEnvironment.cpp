#include "GravitiLib/RealTimeSimEnvironment.h"

#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>
/*
Current thinking is that the raltimesim will create a thread on construction
All public functions will "send signals" to that thread

Functions required to control the thread

start sim
pause sim
reset to origin

Add Entity
Remove Entity


Thread run function

run
	 - Runs on a while loop (while sim enabled) - or some other flag
	 - Uses std chrono to determine how much time has passed
		- Accumulates time, then runs a sim tick to consume a tick of time

		Runs same basic logic as batch sime environment
		Should calculate ticks into the future - future trajectory should be used to determine future path
		Entity probably needs new functions for calculating into future


		Should entity carry the trajectories? or should the environment have the trajectories
			Can the trajectory have a hash map of calculated future trajectories?
			Does it make more sense to have the sim essentially calculated well into the future, and the rendering
			just moves the entities states into queues? we pop off the queues?

*/
RealTimeSimEnvironment::RealTimeSimEnvironment()
{
	m_origin_time = 0.0f;
	m_accumulated_time = 0.0f;
	thread_enabled = true; //Enable thread before running or else it would immidiately end
	run_simulation = false;
	accumulated_time = std::chrono::milliseconds(0); //No accumulated time at the start

	time_step_size = std::chrono::milliseconds(20);//20 updates per second
	simulation_time_scalar = 1.0f;

    update_function = {};

	sim_thread = std::thread(&RealTimeSimEnvironment::run_realtimeSim, this);
}

RealTimeSimEnvironment::~RealTimeSimEnvironment()
{
	//Set flags to stop calculation
	run_simulation = false;
	thread_enabled = false;
	//Join the thread
	sim_thread.join();
}

void RealTimeSimEnvironment::run_realtimeSim()
{
	auto last_time = std::chrono::steady_clock::now();
	while (thread_enabled)
	{
		//Figure out how much time has passed since last time we checked
		auto now = std::chrono::steady_clock::now();
		auto elapsed = now - last_time;
		last_time = now;

		if (run_simulation)
		{
			// Accumulate real elapsed time (unscaled)
			double elapsed_ms = elapsed.count();
			if (elapsed_ms > 1)
			{
				auto current_accum = accumulated_time.load();
				accumulated_time.store(current_accum + elapsed);
			}

			auto step = time_step_size.load();
			while (accumulated_time.load() >= step)
			{
				{
					// Lock calculating mutex while mutating simulation state
					std::scoped_lock m(calculating);

					// Use a for loop here to run multiple ticks if we've selected 2x, 4x, etc time scalar
					for (int i = 0; i < simulation_time_scalar.load(); i++)
					{
						if (!run_simulation)
						{
							//early exit
							break;
						}
						auto ms = step.count();
						double simTime = (double)ms / 1000.0f;
						step_simulation(simTime);
						m_accumulated_time += simTime;
						std::cout << "Scalar = " << simulation_time_scalar << std::endl;
						std::cout << "accumulated time = " << m_accumulated_time << std::endl;
					}
				}

				// Decrement time by the step we consumed
				auto current_accum = accumulated_time.load();
				accumulated_time.store(current_accum - step);
				// Signal GUI that the contents have updated (no sim lock held)
				if (update_function)
				{
					update_function();
				}
			}
		}

		//todo - dynamic sleeps so that we can call one sleep rather than semi-busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	std::cout << "Real Time simulation completed!" << std::endl;

}

void RealTimeSimEnvironment::step_simulation(double simTime)
{
	auto entities = EntityManager::getInstance()->getAllEntities();
	for (auto& entity : *entities) {
		if (!entity.Simulate(simTime)) {
			// Handle simulation error if needed
			std::cout << "Simulation failed for entity: " << entity.getEntityID() << std::endl;
		}
	}
	for (auto& entity : *entities) {
		entity.TickForward();
	}
}

void RealTimeSimEnvironment::setUpdateFunction(std::function<void()> updatefunc)
{
    // I think this is thread-safe enough; add a mutex if needed
    update_function = std::move(updatefunc);
}

void RealTimeSimEnvironment::run()
{
	run_simulation = true;
}

void RealTimeSimEnvironment::pause()
{
	run_simulation = false;
}

void RealTimeSimEnvironment::resetSimulation()
{
	//acquire calculating mutex
	std::scoped_lock m(calculating);
	auto entityManager = EntityManager::getInstance();
	auto entities = entityManager->getAllEntities();
	for (auto& entity : *entities) {
		entity.resetToOrigin(); //Reset its current state back to origin
	}

}

void RealTimeSimEnvironment::setTimestepSize(double timeStepSize)
{
	double ms = timeStepSize * 1000.0f; //Convert seconds to milliseconds
	int asInt = int(std::round(ms));
	if (asInt < 1)
	{
		asInt = 1;
	}
	time_step_size.store(std::chrono::milliseconds(asInt));
}

void RealTimeSimEnvironment::setSimulationScalar(int scalar)
{
	simulation_time_scalar.store(scalar);
}