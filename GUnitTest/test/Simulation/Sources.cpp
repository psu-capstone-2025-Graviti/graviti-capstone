/*
	Solution for now. We need to recompile our cpp files for GTest since QTGraviti is compiling to .exe
	If each unit test tries to include the cpp file, we get linker errors due to potentially including 
	the cpp file multiple times. Most straight forward solution is to include all the cpp files we want
	tested here, then write the individual unit tests against the headers.
	Potential fix to avoid this badness is to restrucre the Simulation folder inside QTGraviti to compile to a lib
*/
#include "src/Simulation/Entity.cpp"
#include "src/Simulation/NBodyEngine.cpp"
#include "src/Simulation/PhysicalState.cpp"
#include "src/Simulation/EntityManager.cpp"