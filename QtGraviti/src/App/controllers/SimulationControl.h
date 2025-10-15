#pragma once

#include <QObject>

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

    void initializeThreeBody();
    void loadJson(std::string filepathjsonPath);

private:


    float cleanFloat(std::string value);

    //EntityListGUI m_entityList;
    //AddEntityGUI  m_addEntity;
    //SimControlGUI m_simControl;

};