// calculatePositions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>




class orbitObject {
    float mass = 0.0f;
    float size = 0.0f;

    float position[3] = {0.0f,0.0f,0.0f};     // x, y, z
    float velocity[3] = { 0.0f,0.0f,0.0f };     // x, y, z
    float acceleration[3] = { 0.0f,0.0f,0.0f }; // x, y, z

public:
    orbitObject(float m, float s, float pos[3], float vel[3])    {
        mass = m;
        size = s;
        position[0] = pos[0];
        position[1] = pos[1];
        position[2] = pos[2];
        velocity[0] = vel[0];
        velocity[1] = vel[1];
        velocity[2] = vel[2];

    }

    void* printPosition() {
        std::cout << "Current position: (" << position[0] << ", " << position[1] << ", " << position[2] << ")" << std::endl;
        return 0;
    }
    float* getPosition() {
        return position;
	}
    float* getVelocity() {
        return velocity;
	}
    float* getAcceleration() {
        return acceleration;
    }
    float getMass() {
        return mass;
    }
    float getSize() {
        return size;
	}
    void setPosition(float pos[3]) {
        position[0] = pos[0];
        position[1] = pos[1];
        position[2] = pos[2];
	}
    void setVelocity(float vel[3]) {
        velocity[0] = vel[0];
        velocity[1] = vel[1];
        velocity[2] = vel[2];
    }
    void setAcceleration(float acc[3]) {
        acceleration[0] = acc[0];
        acceleration[1] = acc[1];
        acceleration[2] = acc[2];
	}
    void integrate( float timestep) {
        
        // Simple Euler integration
        position[0] = position[0] + velocity[0] * timestep;
        position[1] = position[1] + velocity[1] * timestep;
        position[2] = position[2] + velocity[2] * timestep;
        
        velocity[0] = velocity[0] + acceleration[0] * timestep;
        velocity[1] = velocity[1] + acceleration[1] * timestep;
        velocity[2] = velocity[2] + acceleration[2] * timestep;
        
        
    }
    
};


class simulation {
    float G = 6.67430e-11f; // Gravitational constant in m^3 kg^-1 s^-2
    float timestep = 1.0f; // Time step in seconds
    float currentTime = 0.0f;
    float endTime = 100.0f;

    std::list<orbitObject> planetoids = { };

    void step() {
        // Calculate gravitational forces and update positions

        calcGravForces();

        for (auto it = planetoids.begin(); it != planetoids.end(); ++it) {
            // Placeholder for gravitational force calculation
            // Update position based on velocity and acceleration
            
			it->integrate(timestep);

			it->printPosition();

            

        }
    }
    void calcGravForces() {
        for (auto it = planetoids.begin(); it != planetoids.end(); ++it) {
            float totalAcc[3] = { 0.0f, 0.0f, 0.0f };
            for (auto jt = planetoids.begin(); jt != planetoids.end(); ++jt) {
                if (it != jt) { // Avoid self-interaction
                    float dx = jt->getPosition()[0] - it->getPosition()[0];
                    float dy = jt->getPosition()[1] - it->getPosition()[1];
                    float dz = jt->getPosition()[2] - it->getPosition()[2];
                    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
                    if (distance > 1e-5f) { // Prevent division by zero
                        float accMagnitude = G * jt->getMass() / (distance * distance);
                        totalAcc[0] += (accMagnitude * (dx / distance));
                        totalAcc[1] += (accMagnitude * (dy / distance));
                        totalAcc[2] += (accMagnitude * (dz / distance));
                    }
                    else { // Prevent division by zero
                        totalAcc[0] = 0;
                        totalAcc[1] = 0;
                        totalAcc[2] = 0;
                    }
                }
            }
            it->setAcceleration(totalAcc);
        }

    
    }


    

public:

    // getter method to access planetoids
    std::list<orbitObject> getPlanetoids() {
        return planetoids;
    }

    simulation(float ts, float et) {
        currentTime = 0.0f;
		timestep = ts;
		endTime = et;
	}


    void addObject(orbitObject obj) {
        planetoids.push_back(obj);
    }

    void run() {
        while (currentTime < endTime) {
            std::cout << "Current Time: " << currentTime << " seconds" << std::endl;
            step();
            currentTime += timestep;

            
            //for (const auto& obj : planetoids) {
            //    const float* pos = obj.getPosition(); // Use pointer directly
            //    std::cout << "Current position: (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")" << std::endl;
            //}
        }
	}


};

int main()
{

 //   float posE[3] = { 0.0f,0.0f,0.0f };     // x, y, z
 //   float velE[3] = { 0.0f,0.0f,0.0f };     // x, y, z
	//float massofEarth = 5.972e24f; // in kg
	//
	//auto earth = orbitObject(massofEarth, 100.0f, posE, velE);
 //   float posM[3] = { 384400.0f,0.0f,0.0f };     // x, y, z
 //   float velM[3] = { 0.0f,0.0f,0.0f };     // x, y, z
	//float massofMoon = 7.346e22f; // in kg
 //   
	//auto moon = orbitObject(massofMoon, 50.0f, posM, velM);

    float posE[3] = { 0.0f,0.0f,0.0f };     // x, y, z
    float velE[3] = { 0.0f,0.0f,0.0f };     // x, y, z
    float massofEarth = 100.0e10f; // in kg

    auto earth = orbitObject(massofEarth, 100.0f, posE, velE);
    float posM[3] = { 100.0f,0.0f,0.0f };     // x, y, z
    float velM[3] = { 0.0f,0.0f,0.0f };     // x, y, z
    float massofMoon = 1.0e10f; // in kg

    auto moon = orbitObject(massofMoon, 50.0f, posM, velM);

	simulation sim(1.0f,300.0f);
    sim.addObject(earth);
    sim.addObject(moon);
	sim.run();
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
