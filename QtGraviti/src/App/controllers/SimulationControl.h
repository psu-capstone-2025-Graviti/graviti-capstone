#pragma once

#include <QObject>
#include "GravitiLib/BatchSimEnvironment.h"

class SimulationController : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QQmlListProperty<TrajectorySphere> trajectorySpheres READ trajectorySpheres NOTIFY trajectorySpheresChanged)
    //Q_PROPERTY(int count READ trajectorySphereCount NOTIFY trajectorySpheresChanged)
    //Q_PROPERTY(QQmlListProperty<EntitySphere> entitySpheres READ entitySpheres NOTIFY entitySpheresChanged)
    //Q_PROPERTY(int entitySphereCount READ entitySphereCount NOTIFY entitySpheresChanged)

public:
    explicit SimulationController(QObject* parent = nullptr);
    ~SimulationController();



    void initialize_json_body(std::string filepathjsonPath);
    void saveEntitiesAsJson(std::string filepathjsonPath);

    void initializeThreeBody();

    void startSimulation(int numSteps, float tickDuration);
    void resetSimulation();
    void clearEntities();
    void createEntity(const std::string& name, float posX, float posY, float posZ,
                   float velX, float velY, float velZ, float mass);

private:


    float cleanFloat(std::string value);

    BatchSimEnvironment m_env;

};