#pragma once

#include <QObject>
#include <QVector3D>
#include <QQmlListProperty>
#include <QString>
#include <QStringList>
#include <QList>
#include <vector>
#include <memory>
#include "GravitiLib/Entity.h"
#include "GravitiLib/EntityManager.h"
#include "GravitiLib/PhysicalState.h"

// Forward declare
class TrajectorySphere;
class EntitySphere;

class TrajectoryRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<TrajectorySphere> trajectorySpheres READ trajectorySpheres NOTIFY trajectorySpheresChanged)
    Q_PROPERTY(int count READ trajectorySphereCount NOTIFY trajectorySpheresChanged)
    Q_PROPERTY(QQmlListProperty<EntitySphere> entitySpheres READ entitySpheres NOTIFY entitySpheresChanged)
    Q_PROPERTY(int entitySphereCount READ entitySphereCount NOTIFY entitySpheresChanged)

public:
    explicit TrajectoryRenderer(QObject *parent = nullptr);
    ~TrajectoryRenderer();

    //TODO - this is rather hard coded at the moment, and is only rendering past trajectories.
    //Will need to update to do past and future trajectories, and support real time simulatin with trajectory updates
    void convertTrajectoriesToSpheres( int numSpheresToRender, float sphereScale);
    QQmlListProperty<TrajectorySphere> trajectorySpheres();
    QQmlListProperty<EntitySphere> entitySpheres();

    int trajectorySphereCount() const;
    int entitySphereCount() const;
    TrajectorySphere* trajectorySphereAt(int index) const;
    EntitySphere* entitySphereAt(int index) const;

    QString changeColor(const QString& baseColor, float timeProgress) const;

    void addEntityOrigins(float originScale);

signals:
    void trajectorySpheresChanged();
    void entitySpheresChanged();
private:
    QList<TrajectorySphere*> m_trajectorySpheres;
    QList<EntitySphere*> m_entitySpheres;
    void resetSpheres();
};

//TODO - we will need different classes for the different types of rendering. We will probably want a base
// RENDERABLE class or something like that that we can derive from
class TrajectorySphere : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QString materialColor READ materialColor WRITE setMaterialColor NOTIFY materialColorChanged)

public:
    explicit TrajectorySphere(QObject *parent = nullptr);
    TrajectorySphere(const QVector3D& position, const QVector3D& scale, 
                    const QString& entityName, float timestamp, 
                    const QString& materialColor = "#454eee", QObject *parent = nullptr);

    // Getters
    QVector3D position() const;
    QVector3D scale() const;
    QString materialColor() const;

    // Setters
    void setPosition(const QVector3D& position);
    void setScale(const QVector3D& scale);
    void setMaterialColor(const QString& materialColor);

signals:
    void positionChanged();
    void scaleChanged();
    void materialColorChanged();

private:
    QVector3D m_position;
    QVector3D m_scale;
    QString m_materialColor;
};

class EntitySphere : public TrajectorySphere
{
    Q_OBJECT
    Q_PROPERTY(QString texturePath READ texturePath WRITE setTexturePath NOTIFY texturePathChanged)

public:
    explicit EntitySphere(QObject *parent = nullptr);
    EntitySphere(const QVector3D& position, const QVector3D& scale,
                 const QString& entityName, float timestamp,
                 const QString& materialColor = "#ffffff",
                 const QString& texturePath = "", QObject *parent = nullptr);

    // Getters
    QString texturePath() const;

    // Setters
    void setTexturePath(const QString& texturePath);

signals:
    void texturePathChanged();

private:
    QString m_texturePath;
};