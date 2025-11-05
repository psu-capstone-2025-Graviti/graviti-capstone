#pragma once

#include <QObject>
#include "GravitiLib/OptimizationEntityManager.h"
#include "GravitiLib/BatchSimEnvironment.h"
#include "GravitiLib/Entity.h"


class OptimizationController : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QQmlListProperty<TrajectorySphere> trajectorySpheres READ trajectorySpheres NOTIFY trajectorySpheresChanged)
    //Q_PROPERTY(int count READ trajectorySphereCount NOTIFY trajectorySpheresChanged)
    //Q_PROPERTY(QQmlListProperty<EntitySphere> entitySpheres READ entitySpheres NOTIFY entitySpheresChanged)
    //Q_PROPERTY(int entitySphereCount READ entitySphereCount NOTIFY entitySpheresChanged)

public:
    OptimizationController(QObject* parent = nullptr);
    ~OptimizationController();
    //Entity initialEntity;


    int entityManagerCount();

    void initialize_json_body(std::string filepathjsonPath);
    void saveEntitiesAsJson(std::string filepathjsonPath);

    void initializeThreeBody();

    void startSimulation(int numSteps, float tickDuration);
    void resetSimulation();
    void clearEntities(OptimizationEntityManager ToClear);
    void createEntity(const std::string& name, float posX, float posY, float posZ,
                   float velX, float velY, float velZ, float mass);


    void LoadEntities(const std::vector<Entity>& entities, int iterations);
    void LoadProjectile(Entity projectile);
    void LoadTarget(Vec3 targetPosition);

    void optimize();

    Entity initialEntity = Entity();
    Vec3 targetPoint = { 0.0f, 0.0f, 0.0f };

private:
    BatchSimEnvironment m_env;


    std::vector<OptimizationEntityManager> EntityManagers;

    float cleanFloat(std::string value);
};