


#ifndef CSVHANDLER_H

#define CSVHANDLER_H

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

struct Timestep {
    int id;
    float time;
    float timestepSize;
    double position[3];     // x, y, z
    double velocity[3];     // x, y, z
    double acceleration[3]; // x, y, z
};

void writeTimestepsToCSV(const std::string& filename, const std::vector<Timestep>& timesteps) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Write header
    file << "ID,TimestepSize,"
        << "PosX,PosY,PosZ,"
        << "VelX,VelY,VelZ,"
        << "AccX,AccY,AccZ\n";

    // Write each timestep
    for (const auto& ts : timesteps) {
        file << ts.id << "," << ts.timestepSize << ","
            << ts.position[0] << "," << ts.position[1] << "," << ts.position[2] << ","
            << ts.velocity[0] << "," << ts.velocity[1] << "," << ts.velocity[2] << ","
            << ts.acceleration[0] << "," << ts.acceleration[1] << "," << ts.acceleration[2] << "\n";
    }

    file.close();
}

#endif // CSVHANDLER_H