#include "TrajectoryRenderer.h"
#include <QList>

TrajectoryRenderer::TrajectoryRenderer(QObject *parent)
    : QObject(parent)
{
}

TrajectoryRenderer::~TrajectoryRenderer()
{
}

void TrajectoryRenderer::convertTrajectoriesToSpheres(int numSpheresToRender,float sphereScale)
{
    EntityManager* entityManager = EntityManager::getInstance();
    m_trajectorySpheres.clear();

    auto entities = entityManager->getAllEntities();
    if (!entities) {
        qWarning() << "TrajectoryRenderer: No entities found";
        return;
    }

    // TODO - these colors are hard coded at the moment
    QStringList entityColors = {"#ff0000", "#00ffff", "#ff00ff"};
    int colorIndex = 0;

    for (auto& entity : *entities) {
        QString entityName = QString::fromStdString(entity.getEntityName());
        QString materialColor = entityColors[colorIndex % entityColors.size()];
        colorIndex++;

        const std::vector<PhysicalState>& pastTrajectory = entity.getPastTrajectory();
        
        if (pastTrajectory.empty() || numSpheresToRender == 0) {
            //printf("TrajectoryRenderer: No past trajectory found for entity %s", entity.getEntityName());
            continue;
        }

        int timeStepInterval = pastTrajectory.size() / numSpheresToRender;
        //We probably dont want to create a sphere at every tick
        for (size_t i = 0; i < pastTrajectory.size(); i += timeStepInterval) {
            const PhysicalState& state = pastTrajectory[i];
            Vec3 position = state.getPosition();

            
            int scale_up = 1; //mostly for debugging - allows us to scale up the position rendering  - Keep at 1
            QVector3D QspherePosition = QVector3D(position.x * scale_up, position.y * scale_up, position.z * scale_up);
            QVector3D QsphereScale = QVector3D(sphereScale, sphereScale, sphereScale);

            // Calculate time-based color gradient (lighter to darker over time)
            float timeProgress = static_cast<float>(i) / static_cast<float>(pastTrajectory.size() - 1);
            timeProgress = qMax(0.0f, qMin(1.0f, timeProgress)); // Clamp between 0 and 1
            
            // Create gradient from light to dark
            QString gradientColor = changeColor(materialColor, timeProgress);

            auto sphere = new TrajectorySphere(
                QspherePosition,
                QsphereScale,
                entityName,
                state.getTimestamp(),
                gradientColor,
                this  // Set parent to this TrajectoryRenderer
            );

            m_trajectorySpheres.append(sphere);
        }
    }
    emit trajectorySpheresChanged();
}

void TrajectoryRenderer::resetSpheres()
{
    m_trajectorySpheres.clear();
    emit trajectorySpheresChanged();
}

void TrajectoryRenderer::addEntityOrigins(float originScale)
{
    EntityManager* entityManager = EntityManager::getInstance();
    m_entitySpheres.clear();

    auto entities = entityManager->getAllEntities();
    if (!entities) {
        emit entitySpheresChanged();
        return;
    }

    // TODO - these colors are hard coded at the moment
    QStringList entityColors = { "#6f0000", "#006f6f", "#6f006f" };
    int colorIndex = 0;

    for (auto& entity : *entities) {
        QString materialColor = entityColors[colorIndex % entityColors.size()];
        colorIndex++;
        auto state = entity.getOrigin();
        Vec3 position = state.getPosition();


        int scale_up = 1; //mostly for debugging - allows us to scale up the position rendering  - Keep at 1
        QVector3D QspherePosition = QVector3D(position.x * scale_up, position.y * scale_up, position.z * scale_up);

        auto new_scale = originScale;// *entity.getPhysicalState()->getMass();

        float downscale = 0.2;
        float rad = (entity.getPhysicalState()->getRadius())* downscale;
        QVector3D QsphereScale = QVector3D(rad, rad, rad);

        // Create gradient from light to dark
        auto texturePath = entity.getTexturePath();

        if (texturePath != "")
        {
            materialColor = "#ffffff"; //Dont tint the textures
        }

        auto sphere = new EntitySphere(
            QspherePosition,
            QsphereScale,
            QString::fromStdString(entity.getEntityName()),
            state.getTimestamp(),
            materialColor,
            QString::fromStdString(texturePath),  // texturePath - empty for now
            this  // Set parent to this TrajectoryRenderer
        );

        
        m_entitySpheres.append(sphere);
    }
    emit entitySpheresChanged();
};

QQmlListProperty<TrajectorySphere> TrajectoryRenderer::trajectorySpheres()
{
    return QQmlListProperty<TrajectorySphere>(this, &m_trajectorySpheres);
}

int TrajectoryRenderer::trajectorySphereCount() const
{
    return m_trajectorySpheres.size();
}

TrajectorySphere* TrajectoryRenderer::trajectorySphereAt(int index) const
{
    if (index >= 0 && index < m_trajectorySpheres.size()) {
        return m_trajectorySpheres[index];
    }
    return nullptr;
}

QQmlListProperty<EntitySphere> TrajectoryRenderer::entitySpheres()
{
    return QQmlListProperty<EntitySphere>(this, &m_entitySpheres);
}

int TrajectoryRenderer::entitySphereCount() const
{
    return m_entitySpheres.size();
}

EntitySphere* TrajectoryRenderer::entitySphereAt(int index) const
{
    if (index >= 0 && index < m_entitySpheres.size()) {
        return m_entitySpheres[index];
    }
    return nullptr;
}

TrajectorySphere::TrajectorySphere(QObject *parent)
    : QObject(parent)
    , m_position(0, 0, 0)
    , m_scale(1, 1, 1)
    , m_entityName("Unknown")
    , m_timestamp(0.0f)
    , m_materialColor("#ffffff")
{
}

TrajectorySphere::TrajectorySphere(const QVector3D& position, const QVector3D& scale, 
                                  const QString& entityName, float timestamp, 
                                  const QString& materialColor, QObject *parent)
    : QObject(parent)
    , m_position(position)
    , m_scale(scale)
    , m_entityName(entityName)
    , m_timestamp(timestamp)
    , m_materialColor(materialColor)
{
}

QVector3D TrajectorySphere::position() const
{
    return m_position;
}

QVector3D TrajectorySphere::scale() const
{
    return m_scale;
}

QString TrajectorySphere::entityName() const
{
    return m_entityName;
}

float TrajectorySphere::timestamp() const
{
    return m_timestamp;
}

QString TrajectorySphere::materialColor() const
{
    return m_materialColor;
}

void TrajectorySphere::setPosition(const QVector3D& position)
{
    if (m_position != position) {
        m_position = position;
        emit positionChanged();
    }
}

void TrajectorySphere::setScale(const QVector3D& scale)
{
    if (m_scale != scale) {
        m_scale = scale;
        emit scaleChanged();
    }
}



void TrajectorySphere::setMaterialColor(const QString& materialColor)
{
    if (m_materialColor != materialColor) {
        m_materialColor = materialColor;
        emit materialColorChanged();
    }
}

QString TrajectoryRenderer::changeColor(const QString& baseColor, float timeProgress) const
{
    QString color = baseColor;
    if (color.startsWith("#")) {
        color = color.mid(1);
    }

    bool success;
    int r = color.mid(0, 2).toInt(&success, 16);
    int g = color.mid(2, 2).toInt(&success, 16);
    int b = color.mid(4, 2).toInt(&success, 16);
    
    if (!success) {
        return baseColor;
    }
    
    float lightFactor = 1.0f - (timeProgress * 0.7f);
    lightFactor = qMax(0.1f, lightFactor);
    
    int newR = static_cast<int>(r * lightFactor);
    int newG = static_cast<int>(g * lightFactor);
    int newB = static_cast<int>(b * lightFactor);
    
    newR = qMax(0, qMin(255, newR));
    newG = qMax(0, qMin(255, newG));
    newB = qMax(0, qMin(255, newB));
    
    return QString("#%1%2%3").arg(newR, 2, 16, QChar('0')).arg(newG, 2, 16, QChar('0')).arg(newB, 2, 16, QChar('0'));
}

// EntitySphere implementation
EntitySphere::EntitySphere(QObject *parent)
    : TrajectorySphere(parent)
    , m_texturePath("")
{
}

EntitySphere::EntitySphere(const QVector3D& position, const QVector3D& scale,
                           const QString& entityName, float timestamp,
                           const QString& materialColor,
                           const QString& texturePath, QObject *parent)
    : TrajectorySphere(position, scale, entityName, timestamp, materialColor, parent)
    , m_texturePath(texturePath)
{
}

QString EntitySphere::texturePath() const
{
    return m_texturePath;
}

void EntitySphere::setTexturePath(const QString& texturePath)
{
    if (m_texturePath != texturePath) {
        m_texturePath = texturePath;
        emit texturePathChanged();
    }
}




