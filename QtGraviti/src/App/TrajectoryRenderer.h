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
class FlatEntity;

class TrajectoryRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<TrajectorySphere> trajectorySpheres READ trajectorySpheres NOTIFY trajectorySpheresChanged)
    Q_PROPERTY(int count READ trajectorySphereCount NOTIFY trajectorySpheresChanged)
    Q_PROPERTY(QQmlListProperty<EntitySphere> entitySpheres READ entitySpheres NOTIFY entitySpheresChanged)
    Q_PROPERTY(int entitySphereCount READ entitySphereCount NOTIFY entitySpheresChanged)
    Q_PROPERTY(QQmlListProperty<FlatEntity> flatEntities READ flatEntities NOTIFY flatEntitiesChanged)
    Q_PROPERTY(int flatEntityCount READ flatEntityCount NOTIFY flatEntitiesChanged)
    Q_PROPERTY(QVector3D camPosition READ camPosition NOTIFY camPositionChanged)

public:
    explicit TrajectoryRenderer(QObject *parent = nullptr);
    ~TrajectoryRenderer();

    //TODO - this is rather hard coded at the moment, and is only rendering past trajectories.
    //Will need to update to do past and future trajectories, and support real time simulatin with trajectory updates
    void convertTrajectoriesToSpheres( int numSpheresToRender, float sphereScale);
    QQmlListProperty<TrajectorySphere> trajectorySpheres();
    QQmlListProperty<EntitySphere> entitySpheres();
    QQmlListProperty<FlatEntity> flatEntities();

    void resetSpheres(); //resets trajectory spheres

    int trajectorySphereCount() const;
    int entitySphereCount() const;
    int flatEntityCount() const;

    TrajectorySphere* trajectorySphereAt(int index) const;
    EntitySphere* entitySphereAt(int index) const;

    QString changeColor(const QString& baseColor, float timeProgress) const;

    void addEntityOrigins(float originScale);

    void lockCameraEntity(std::string EntityName);
    QVector3D camPosition();

    // Create a sphere per entity if missing; otherwise update its position/scale
    void updateEntitySpheres();
    // Clear all dynamically created entity spheres and notify QML
    void clearEntitySpheres();


signals:
    void trajectorySpheresChanged();
    void entitySpheresChanged();
    void camPositionChanged();
    void flatEntitiesChanged();

private:
    QList<TrajectorySphere*> m_trajectorySpheres;
    QList<EntitySphere*> m_entitySpheres;
    QList<FlatEntity*> m_flatEntities;
    std::string locked_entity;
    QVector3D m_camPosition;
    bool resetCameraOnce = false;



    void clearCameraEntity();

};

class FlatEntity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale NOTIFY scaleChanged)
    Q_PROPERTY(QString texturePath READ texturePath NOTIFY texturePathChanged)
    Q_PROPERTY(qreal opacity READ opacity NOTIFY opacityChaged)

public:
    FlatEntity(const QVector3D& position, const QVector3D& scale,
        QString texturePath, const QString& entityName, const qreal opacity,
        QObject* parent = nullptr);

    // Getters
    QVector3D position() const;
    QVector3D scale() const;
    QString texturePath() const;
    QString entityName() const;
    qreal opacity() const;

    // Setters
    void setPosition(QVector3D newPos);
    void setScale(QVector3D newScale);
    void setTexturePath(QString newPath);
    void setOpacity(float opacity);

signals:
    void positionChanged();
    void scaleChanged();
    void texturePathChanged();
    void opacityChaged();

private:
    QVector3D m_position;
    QVector3D m_scale;
    QString m_path;
    QString m_entityName;
    qreal m_opacity;
};

//TODO - we will need different classes for the different types of rendering. We will probably want a base
// RENDERABLE class or something like that that we can derive from
class TrajectorySphere : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QString materialColor READ materialColor WRITE setMaterialColor NOTIFY materialColorChanged)
    Q_PROPERTY(QString entityName READ entityName CONSTANT)

public:
    explicit TrajectorySphere(QObject *parent = nullptr);
    TrajectorySphere(const QVector3D& position, const QVector3D& scale, 
                    const QString& entityName, float timestamp, 
                    const QString& materialColor = "#454eee", QObject *parent = nullptr);

    // Getters
    QVector3D position() const;
    QVector3D scale() const;
    QString materialColor() const;
    QString entityName() const;
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
    QString m_entityName;
};

class EntitySphere : public TrajectorySphere
{
    Q_OBJECT
    Q_PROPERTY(QString texturePath READ texturePath WRITE setTexturePath NOTIFY texturePathChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)

public:
    explicit EntitySphere(QObject *parent = nullptr);
    EntitySphere(const QVector3D& position, const QVector3D& scale,
                 const QString& entityName, float timestamp,
                 const QString& materialColor = "#ffffff",
                 const QString& texturePath = "", const qfloat16& opacity = 1.0f, QObject *parent = nullptr);

    // Getters
    QString texturePath() const;
    qreal opacity() const;

    // Setters
    void setTexturePath(const QString& texturePath);
    void setOpacity(qreal opacity);

signals:
    void texturePathChanged();
    void opacityChanged();

private:
    QString m_texturePath;
    qfloat16 m_opacity;
};