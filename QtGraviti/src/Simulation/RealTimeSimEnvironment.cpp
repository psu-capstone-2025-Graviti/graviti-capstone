#include "GravitiLib/RealTimeSimEnvironment.h"

#include <thread>
#include <chrono>
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
	thread_enabled = true; //Enable thread before running or else it would immidiately end
	run_simulation = false;
	accumulated_time = std::chrono::milliseconds(0);

	time_step_size = std::chrono::milliseconds(100);
	simulation_time_scalar = 1.0f;

	update_function_ptr = nullptr;

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
			double elapsed_ms_double = std::chrono::duration<double, std::milli>(elapsed).count();
			if (elapsed_ms_double > 0.0)
			{
				auto add_ms = std::chrono::milliseconds(static_cast<long long>(elapsed_ms_double));
				if (add_ms.count() > 0)
				{
					auto current_accum = accumulated_time.load();
					accumulated_time.store(current_accum + add_ms);
				}
			}

			auto step = time_step_size.load();
			while (accumulated_time.load() >= step)
			{
				//Lock calculationg mutex
				std::scoped_lock m(calculating);
				if (update_function_ptr)
				{
					for (int i = 0; i < simulation_time_scalar.load(); i++)
					{
						//TODO - Simulation logic here
						update_function_ptr();
					}
				}
				auto current_accum2 = accumulated_time.load();
				accumulated_time.store(current_accum2 - step);
			}
		}

		//todo - dynamic sleeps so that we can call one sleep rather than semi-busy waiting
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	std::cout << "Real Time simulation completed!" << std::endl;

}

void RealTimeSimEnvironment::setUpdateFunction(void (*updatefunc)(void))
{
	//I *think* this is thread safe enough - but if need be I can always add a mutex
	update_function_ptr = updatefunc;
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