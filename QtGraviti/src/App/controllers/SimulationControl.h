#pragma once

#include <QObject>
#include <functional>
//#include "GravitiLib/BatchSimEnvironment.h"
#include "GravitiLib/RealTimeSimEnvironment.h"

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

    void startSimulation(int numSteps, float tickDuration, int simulationScalar = 1);
	Entity optimizeTrajectory(Entity Projectile, Vec3 targetPosition, int numSteps, float tickDuration, int numIter);

    void resetSimulation();
    void clearEntities();
    void createEntity(const std::string& name, float posX, float posY, float posZ,
                   float velX, float velY, float velZ, float mass);

    void setUpdateFunction(std::function<void()> updateFunc);

    void pauseSimulation();
    void resumeSimulation();

private:


    float cleanFloat(std::string value);

    //BatchSimEnvironment m_env;
    RealTimeSimEnvironment m_env;

};