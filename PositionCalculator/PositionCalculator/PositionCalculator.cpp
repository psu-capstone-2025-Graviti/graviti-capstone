// PositionCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <fstream> // For file handling
#include <iostream>
//#include "csvHandler.h"
#include <cmath> 
#include <vector>
#include <string>
#include <list>

const int numberOfSteps = 100;

struct Timestep {
    int id;
    float time;
    float timestep;
    double position[3];     // x, y, z
    double velocity[3];     // x, y, z
    double acceleration[3]; // x, y, z
	
};




struct satellite {

    Timestep steps[numberOfSteps];

    satellite(float pos[3], float vel[3], float acc[3]) {
        steps[0].position[0] = pos[0];
        steps[0].position[1] = pos[1];
        steps[0].position[2] = pos[2];

        steps[0].velocity[0] = vel[0];
        steps[0].velocity[1] = vel[1];
        steps[0].velocity[2] = vel[2];

        steps[0].acceleration[0] = acc[0];
        steps[0].acceleration[1] = acc[1];
        steps[0].acceleration[2] = acc[2];

        steps[0].time = 0.0f;
        float timestep = 1.0f;

        for (int i = 1; i < numberOfSteps; i++) {
            steps[i].time = steps[i - 1].time + steps[0].timestep;
            steps[i].position[0] = (steps[i - 1].position[0]) + (steps[i - 1].velocity[0] * timestep) + (0.5 * steps[i - 1].acceleration[0] * pow(timestep, 2));
            steps[i].position[1] = (steps[i - 1].position[1]) + (steps[i - 1].velocity[1] * timestep) + (0.5 * steps[i - 1].acceleration[1] * pow(timestep, 2));
            steps[i].position[2] = (steps[i - 1].position[2]) + (steps[i - 1].velocity[2] * timestep) + (0.5 * steps[i - 1].acceleration[2] * pow(timestep, 2));

            steps[i].velocity[0] = (steps[i - 1].velocity[0]) + (steps[i - 1].acceleration[0] * timestep);
            steps[i].velocity[1] = (steps[i - 1].velocity[1]) + (steps[i - 1].acceleration[1] * timestep);
            steps[i].velocity[2] = (steps[i - 1].velocity[2]) + (steps[i - 1].acceleration[2] * timestep);

            steps[i].acceleration[0] = (steps[i - 1].acceleration[0]);
            steps[i].acceleration[1] = (steps[i - 1].acceleration[1]);
            steps[i].acceleration[2] = (steps[i - 1].acceleration[2]);

        };
    };

};
struct planetoid : satellite {
    float mass;          // in kg
    planetoid(float pos[3], float vel[3], float acc[3], float m = 0.0)
        : satellite(pos, vel, acc), mass(m) {}
};


void calculateAccelerations(std::list<satellite>& satellites, std::list<planetoid>& planetoids, const float G) {
    for (auto& sat : satellites) {
        for (int step = 0; step < numberOfSteps; step++) {
            float totalAcc[3] = { 0.0f, 0.0f, 0.0f };
            for (const auto& planet : planetoids) {
                float dx = planet.steps[step].position[0] - sat.steps[step].position[0];
                float dy = planet.steps[step].position[1] - sat.steps[step].position[1];
                float dz = planet.steps[step].position[2] - sat.steps[step].position[2];
                float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
                if (distance > 0) { // Prevent division by zero
                    float accMagnitude = G * planet.mass / (distance * distance);
                    totalAcc[0] += accMagnitude * (dx / distance);
                    totalAcc[1] += accMagnitude * (dy / distance);
                    totalAcc[2] += accMagnitude * (dz / distance);
                }
            }
            sat.steps[step].acceleration[0] = totalAcc[0];
            sat.steps[step].acceleration[1] = totalAcc[1];
            sat.steps[step].acceleration[2] = totalAcc[2];
            if (step < numberOfSteps-1) { // Update velocity and position for the next step
                float timestep = sat.steps[step].time;
                sat.steps[step + 1].velocity[0] = sat.steps[step].velocity[0] + sat.steps[step].acceleration[0] * timestep;
                sat.steps[step + 1].velocity[1] = sat.steps[step].velocity[1] + sat.steps[step].acceleration[1] * timestep;
                sat.steps[step + 1].velocity[2] = sat.steps[step].velocity[2] + sat.steps[step].acceleration[2] * timestep;
                
                sat.steps[step + 1].position[0] = sat.steps[step].position[0] + sat.steps[step].velocity[0] * timestep + 0.5 * sat.steps[step].acceleration[0] * timestep * timestep;
                sat.steps[step + 1].position[1] = sat.steps[step].position[1] + sat.steps[step].velocity[1] * timestep + 0.5 * sat.steps[step].acceleration[1] * timestep * timestep;
                sat.steps[step + 1].position[2] = sat.steps[step].position[2] + sat.steps[step].velocity[2] * timestep + 0.5 * sat.steps[step].acceleration[2] * timestep * timestep;
            };
        };
    };
};


int main() {
    //csvHandler obj;
    //writeCSV("example.csv", { {"Time", "PositionX", "PositionY", "PositionZ"} });


    float positions1[3] = { 10000.0f, 0.0f, 5.0f };
    float velocities1[3] = { 0.0f, 1000.0f, 0.0f };
    float accelerations1[3] = { 0.0f, 0.0f, 0.0f };

    float positionEarth[3] = { 0.0f, 0.0f, 0.0f };
    float velocitiesEarth[3] = { 0.0f, 0.0f, 0.0f };
    float accelerationsEarth[3] = { 0.0f, 0.0f, 0.0f };
    const float G = 6.67430e-11; // Gravitational constant in m^3 kg^-1 s^-2
	const float massEarth = 5.972e24; // Mass of Earth in kg
	const float massMoon = 7.342e22; // Mass of Moon in kg


    float positionMoon[3] = { 384400.0f, 0.0f, 0.0f };
    float velocitiesMoon[3] = { 0.0f, 0.0f, 0.0f };
    float accelerationsMoon[3] = { 0.0f, 0.0f, 0.0f };
    std::list<planetoid> planetoids = { };
    std::list<satellite> satellites = { };

    satellite sat(positions1, velocities1, accelerations1); // Initial position in km
    satellites.push_back(sat);


    planetoid earth(positionEarth, velocitiesEarth, accelerationsEarth, massEarth); // Initial position in km
    planetoid moon(positionMoon, velocitiesMoon, accelerationsMoon, massMoon); // Initial position in km
    planetoids.push_back(earth);
    planetoids.push_back(moon);
    // end initializations


	//calculateAccelerations(satellites, planetoids, G);

   

        for (int i = 0; i < numberOfSteps; i++) {
            std::cout << "" << satellites.front().steps[i].position[0] << ", " << satellites.front().steps[i].position[1] << ", " << satellites.front().steps[i].position[2] << "\n";
        };
        return 0;
    };
