// PositionCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream> // For file handling
#include <vector>
#include <string>

int main() {
    // Create and open a CSV file
    std::ofstream file("example.csv");

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    // Write headers to the CSV file
    file << "Name,Age,City\n";

    // Write data rows to the CSV file
    file << "Alice,30,New York\n";
    file << "Bob,25,Los Angeles\n";
    file << "Charlie,35,Chicago\n";

    // Close the file
    file.close();

    std::cout << "CSV file created successfully!" << std::endl;

    return 0;
}

class Particle {
    public:
        float position[3];
        float velocity[3];
		float acceleration[3];
		float nextPosition[3];
		float prevPosition[3];
		float timestep = 0.1f;
        Particle(float posx, float posy, float posz) {
            position[0] = posx;
            position[1] = posy;
            position[2] = posz;
        };
        float position[3] = { 0.0f, 0.0f, 0.0f };
        float velocity[3] = { 1.0f, 1.0f, 1.0f };
        float acceleration[3] = { 0.1f, 0.1f, 0.1f };

};
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
