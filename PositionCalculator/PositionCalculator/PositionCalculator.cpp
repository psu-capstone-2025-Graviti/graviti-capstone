// PositionCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <fstream> // For file handling
#include <iostream>
//#include "csvHandler.h"
#include <vector>
#include <string>
class step {
public:
    float time;
    float timestep;
    float position[3];
    float velocity[3];
    float acceleration[3];
    step(float posx, float posy, float posz, float time, float timestep) {
        time = 0.0f;
        this->position[0] = posx;
        this->position[1] = posy;
        this->position[2] = posz;
        this->velocity[0] = 30;
        this->velocity[1] = 0;
        this->velocity[2] = 0;
        this->acceleration[0] = 0;
        this->acceleration[1] = 0;
        this->acceleration[2] = 0;
        this->timestep = timestep;
        this->time = time;
    };

    step() = default;
};
class satellite {
public:
    step steps[5];

    satellite(float posx, float posy, float posz) {
        steps[0].position[0] = posx;
        steps[0].position[1] = posy;
        steps[0].position[2] = posz;
        steps[0].time = 0.0f;
        steps[0].timestep = 0.1f;

        for (int i = 1; i < 5; i++) {
            steps[i].position[0] = steps[i - 1].position[0] + steps[i - 1].velocity[0] * steps[i - 1].timestep;
            steps[i].position[1] = steps[i - 1].position[1] + steps[i - 1].velocity[1] * steps[i - 1].timestep;
            steps[i].position[2] = steps[i - 1].position[2] + steps[i - 1].velocity[2] * steps[i - 1].timestep;
        };
    };

};

//class planetoid {
//public:
//    step steps[5];
//    float position[3];
//    float velocity[3];
//    float acceleration[3];
//    float nextPosition[3];
//    float prevPosition[3];
//    float timestep = 0.1f;
//    float currentTime = 0.0f;
//	float mass = 1000.0f; // in kg
//    planetoid(float posx, float posy, float posz, float time, float timestep) {
//        position[0] = posx;
//        position[1] = posy;
//        position[2] = posz;
//    };
//
//
//};


int main() {
    //csvHandler obj;
    //writeCSV("example.csv", { {"Time", "PositionX", "PositionY", "PositionZ"} });



    satellite sat(7000.0f, 0.0f, 0.0f); // Initial position in km

    for (int i = 0; i < sizeof(sat.steps->position); i++) {
        std::cout << "Position: (" << sat.steps[i].position[0] << ", " << sat.steps[i].position[1] << ", " << sat.steps[i].position[2] << ")\n";
    };
    //planetoid earth(0.0f, 0.0f, 0.0f); // Initial position in km
    //planetoid moon(384400.0f, 0.0f, 0.0f); // Initial position in km
    return 0;
}