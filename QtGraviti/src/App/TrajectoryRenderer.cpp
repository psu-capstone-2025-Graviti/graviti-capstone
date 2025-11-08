#include "TrajectoryRenderer.h"
#include <QList>

const double RENDER_DOWNSCALE = 10000.0f;

//Determined by trial and error. The default sphere in QT seems to be much larger than 2 units accross
const float downScale = 0.0205f;

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

            
            QVector3D QspherePosition = QVector3D(position.x / RENDER_DOWNSCALE, position.y / RENDER_DOWNSCALE, position.z / RENDER_DOWNSCALE);
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

        QVector3D QspherePosition = QVector3D(position.x, position.y, position.z);

        float rad = (entity.getPhysicalState()->getRadius())* downScale;
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
            QString::fromStdString(texturePath),
            1.0f,
            this
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
    , m_materialColor("#ffffff")
{
}

TrajectorySphere::TrajectorySphere(const QVector3D& position, const QVector3D& scale, 
                                  const QString& entityName, float timestamp, 
                                  const QString& materialColor, QObject *parent)
    : QObject(parent)
    , m_position(position)
    , m_scale(scale)
    , m_materialColor(materialColor)
    , m_entityName(entityName)
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

QString TrajectorySphere::materialColor() const
{
    return m_materialColor;
}

QString TrajectorySphere::entityName() const
{
    return m_entityName;
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
                           const QString& texturePath,
                           const qfloat16& opacity, QObject *parent)
    : TrajectorySphere(position, scale, entityName, timestamp, materialColor, parent)
    , m_texturePath(texturePath), m_opacity(opacity)
{
}

void TrajectoryRenderer::lockCameraEntity(std::string EntityName)
{
    if (locked_entity != EntityName)
    {
        locked_entity = EntityName;
    }
}

void TrajectoryRenderer::clearCameraEntity()
{
    locked_entity = "";
}

QVector3D TrajectoryRenderer::camPosition()
{
    return m_camPosition;
}

void TrajectoryRenderer::updateEntitySpheres()
{
    EntityManager* entityManager = EntityManager::getInstance();
    auto entities = entityManager->getAllEntities();

    //If the entity vector is empty
    if (!entities) {
        if (!m_entitySpheres.isEmpty()) {
            m_entitySpheres.clear();
            emit entitySpheresChanged();
        }
        return;
    }

    // Colors are still basic defaults
    QStringList entityColors = { "#6f0000", "#006f6f", "#6f006f" };
    int colorIndex = 0;

    bool createdAny = false;
    //If we go over the whole list, and don't find the entity, reset the posotion so that control can be returned to the keyboard.
    bool camEntityFound = false;

    for (auto& entity : *entities) {
        std::string ename = entity.getEntityName();
        const QString qname = QString::fromStdString(ename);

        // Compute position and scale from current state
        PhysicalState* statePtr = entity.getPhysicalState();
        Vec3 position = statePtr ? statePtr->getPosition() : entity.getOrigin().getPosition();

        float rad = ((entity.getPhysicalState()->getRadius()) * downScale) / RENDER_DOWNSCALE;

        if (rad < 0.001) //Nothing smaller than 1 meter across
        {
            rad = 0.001f;
        }
        QVector3D qpos(position.x / RENDER_DOWNSCALE, position.y / RENDER_DOWNSCALE, position.z / RENDER_DOWNSCALE);

        if (ename == locked_entity)
        {
            m_camPosition = qpos;
            camEntityFound = true;
            emit camPositionChanged();
        }

        QVector3D qscale(rad, rad, rad);

        // Try to find existing sphere by name
        EntitySphere* found = nullptr;
        for (auto* s : m_entitySpheres) {
            if (s && s->entityName() == qname) { found = s; break; }
        }

        if (found) {
            // Update position and (if changed) scale
            found->setPosition(qpos);
            found->setScale(qscale);
            // Update texture if needed
            const auto texturePath = QString::fromStdString(entity.getTexturePath());
            if (!texturePath.isEmpty()) {
                found->setMaterialColor("#ffffff");
                found->setTexturePath(texturePath);
            }
            // Update opacity based on alive state
            found->setOpacity(entity.isunAlive() ? 0.5f : 1.0f);
        } else {
            // Create new sphere for this entity
            QString materialColor = entityColors[colorIndex % entityColors.size()];
            colorIndex++;
            const auto texturePath = QString::fromStdString(entity.getTexturePath());
            if (!texturePath.isEmpty()) {
                materialColor = "#ffffff";
            }

            auto* sphere = new EntitySphere(
                qpos,
                qscale,
                qname,
                statePtr ? statePtr->getTimestamp() : entity.getOrigin().getTimestamp(),
                materialColor,
                texturePath,
                entity.isunAlive() ? 0.5f : 1.0f,
                this
            );
            m_entitySpheres.append(sphere);
            createdAny = true;
        }
    }

    if (createdAny) {
        emit entitySpheresChanged();
    }

    if (!camEntityFound)
    {
        m_camPosition = QVector3D(0, 0, 0);
    }

}

void TrajectoryRenderer::clearEntitySpheres()
{
    if (m_entitySpheres.isEmpty()) {
        return;
    }
    m_entitySpheres.clear();
    emit entitySpheresChanged();
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

qreal EntitySphere::opacity() const
{
    return static_cast<qreal>(m_opacity);
}

void EntitySphere::setOpacity(qreal opacity)
{
    // Clamp between 0 and 1
    if (opacity < 0.0)
        opacity = 0.0;
    else if (opacity > 1.0)
        opacity = 1.0;

    const qreal current = static_cast<qreal>(m_opacity);
    if (current == opacity)
        return;

    m_opacity = static_cast<qfloat16>(opacity);
    emit opacityChanged();
}




