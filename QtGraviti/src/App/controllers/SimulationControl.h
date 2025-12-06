#pragma once

#include <QObject>
#include <functional>
#include "GravitiLib/BatchSimEnvironment.h"
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

    void bathProcessFuture(int numSteps, float tickDuration);
    void resetBatch();

	Entity optimizeTrajectory(Entity Projectile, Vec3 targetPosition, int numSteps, float tickDuration, int numIter);


    void resetSimulation();
    void clearEntities();
    void createEntity(const std::string& name, float posX, float posY, float posZ,
                   float velX, float velY, float velZ, float mass, float radius = 1.0f, std::string tex = "");

    void setUpdateFunction(std::function<void()> updateFunc);

    void pauseSimulation();
    void resumeSimulation();

private:

    // Snapshot of entity states captured before batch simulate
    std::map<long, PhysicalState> m_batchStates;
    bool m_hasBatchSnapshot = false;

    float cleanFloat(std::string value);

    BatchSimEnvironment m_batchenv;
    RealTimeSimEnvironment m_env;

};