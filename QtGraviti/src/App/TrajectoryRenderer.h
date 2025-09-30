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

class TrajectoryRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<TrajectorySphere> trajectorySpheres READ trajectorySpheres NOTIFY trajectorySpheresChanged)
    Q_PROPERTY(int count READ trajectorySphereCount NOTIFY trajectorySpheresChanged)

public:
    explicit TrajectoryRenderer(QObject *parent = nullptr);
    ~TrajectoryRenderer();

    //TODO - this is rather hard coded at the moment, and is only rendering past trajectories.
    //Will need to update to do past and future trajectories, and support real time simulatin with trajectory updates
    void convertTrajectoriesToSpheres( int timeStepInterval, float sphereScale);
    QQmlListProperty<TrajectorySphere> trajectorySpheres();

    int trajectorySphereCount() const;
    TrajectorySphere* trajectorySphereAt(int index) const;

    QString changeColor(const QString& baseColor, float timeProgress) const;


signals:
    void trajectorySpheresChanged();

private:
    QList<TrajectorySphere*> m_trajectorySpheres;
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
    QString entityName() const;
    float timestamp() const;
    QString materialColor() const;

    // Setters
    void setPosition(const QVector3D& position);
    void setScale(const QVector3D& scale);
    void setEntityName(const QString& entityName);
    void setTimestamp(float timestamp);
    void setMaterialColor(const QString& materialColor);

signals:
    void positionChanged();
    void scaleChanged();
    void materialColorChanged();

private:
    QVector3D m_position;
    QVector3D m_scale;
    QString m_entityName;
    float m_timestamp;
    QString m_materialColor;
};

