#ifndef BatchSimEnvironment_INCLUDE_GUARD_H
#define BatchSimEnvironment_INCLUDE_GUARD_H

#include "EntityManager.h"

using namespace std::chrono;

class BatchSimEnvironment
{
public:
	BatchSimEnvironment();
	void initialize_two_body();
	void initialize_three_body();
	void initialize_json_body();

	//After run is called, we cant add more stuff to environment
	void run();

private:
	float m_origin_time;
};

#endif // BatchSimEnvironment_INCLUDE_GUARD_H