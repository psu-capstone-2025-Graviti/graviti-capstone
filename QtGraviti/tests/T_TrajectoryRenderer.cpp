#include <gtest/gtest.h>
#include <QGuiApplication>
#include <QVector3D>
#include <QString>
#include <QObject>
#include <QSignalSpy>
#include <QCoreApplication>
#include "App/TrajectoryRenderer.h"

class TrajectoryRendererTest : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    TrajectoryRenderer* m_renderer;
};

void TrajectoryRendererTest::SetUp()
{
    m_renderer = new TrajectoryRenderer();
}

void TrajectoryRendererTest::TearDown()
{
    delete m_renderer;
}

TEST_F(TrajectoryRendererTest, RendererConstructor)
{
    TrajectoryRenderer renderer;
    EXPECT_EQ(renderer.parent(), nullptr);
    EXPECT_EQ(renderer.trajectorySphereCount(), 0);
    EXPECT_EQ(renderer.entitySphereCount(), 0);
}

TEST_F(TrajectoryRendererTest, ChangeColorMethod)
{
    QString baseColor = "#ff0000";
    QString baseLight = "#ff0000";
    QString baseDark  = "#4c0000";
    QString baseMed   = "#a50000";

    // Start should be bright
    QString light = m_renderer->changeColor(baseColor, 0.0f);
    EXPECT_EQ(baseLight.toStdString(), light.toStdString());

    //At the end the value should have been reduced by 70%
    //30% of ff rounds down to 4c
    QString dark = m_renderer->changeColor(baseColor, 1.0f);
    EXPECT_EQ(baseDark.toStdString(), dark.toStdString());

    //Medium is 1-(0.7 * 0.5) which is 65%, rounds down to a5
    QString medium = m_renderer->changeColor(baseColor, 0.5f);
    EXPECT_EQ(baseMed.toStdString(), medium.toStdString());

    QString invalidColor = "INVALID";
    QString resultInvalid = m_renderer->changeColor(invalidColor, 0.5f);
    EXPECT_EQ(resultInvalid.toStdString(), invalidColor.toStdString());
}

TEST_F(TrajectoryRendererTest, TrajectorySpheresChangedSignal)
{
    QSignalSpy spy(m_renderer, &TrajectoryRenderer::trajectorySpheresChanged);
    EXPECT_TRUE(spy.isValid());
    EXPECT_EQ(spy.count(), 0);
}

TEST_F(TrajectoryRendererTest, TrajectorySphereConstructor)
{
    TrajectorySphere sphere;
    EXPECT_EQ(sphere.position(), QVector3D(0, 0, 0));
    EXPECT_EQ(sphere.scale(), QVector3D(1, 1, 1));
    EXPECT_EQ(sphere.materialColor(), QString("#ffffff"));
}

TEST_F(TrajectoryRendererTest, TrajectorySphereParameterizedConstructor)
{
    QVector3D position(1.0f, 2.0f, 3.0f);
    QVector3D scale(0.5f, 0.5f, 0.5f);
    QString entityName("TestEntity");
    float timestamp = 123.45f;
    QString materialColor("#ff0000");

    TrajectorySphere sphere(position, scale, entityName, timestamp, materialColor);

    EXPECT_EQ(sphere.position(), position);
    EXPECT_EQ(sphere.scale(), scale);
    EXPECT_EQ(sphere.materialColor(), materialColor);
}

TEST_F(TrajectoryRendererTest, TrajectorySphereGetters)
{
    QVector3D position(1.0f, 2.0f, 3.0f);
    QVector3D scale(0.5f, 0.5f, 0.5f);
    QString entityName("TestEntity");
    float timestamp = 123.45f;
    QString materialColor("#ff0000");

    TrajectorySphere sphere(position, scale, entityName, timestamp, materialColor);
    EXPECT_EQ(sphere.position(), position);
    EXPECT_EQ(sphere.scale(), scale);
    EXPECT_EQ(sphere.materialColor(), materialColor);
}

TEST_F(TrajectoryRendererTest, TrajectorySphereSetters)
{
    TrajectorySphere sphere;

    QVector3D newPosition(5.0f, 6.0f, 7.0f);
    sphere.setPosition(newPosition);
    EXPECT_EQ(sphere.position(), newPosition);

    QVector3D newScale(2.0f, 2.0f, 2.0f);
    sphere.setScale(newScale);
    EXPECT_EQ(sphere.scale(), newScale);

    QString newMaterialColor("#00ff00");
    sphere.setMaterialColor(newMaterialColor);
    EXPECT_EQ(sphere.materialColor(), newMaterialColor);
}

TEST_F(TrajectoryRendererTest, TrajectorySphereSignals)
{
    TrajectorySphere tsphere;

    QSignalSpy positionSpy(&tsphere, &TrajectorySphere::positionChanged);
    QVector3D newPosition(1.0f, 2.0f, 3.0f);
    tsphere.setPosition(newPosition);
    EXPECT_EQ(positionSpy.count(), 1);

    QSignalSpy scaleSpy(&tsphere, &TrajectorySphere::scaleChanged);
    QVector3D newScale(2.0f, 2.0f, 2.0f);
    tsphere.setScale(newScale);
    EXPECT_EQ(scaleSpy.count(), 1);

    QSignalSpy colorSpy(&tsphere, &TrajectorySphere::materialColorChanged);
    QString newColor("#ff0000");
    tsphere.setMaterialColor(newColor);
    EXPECT_EQ(colorSpy.count(), 1);

    //Setting the spheres to the same vlue doesnt iterate the spy
    tsphere.setPosition(newPosition);
    tsphere.setScale(newScale);
    tsphere.setMaterialColor(newColor);

    EXPECT_EQ(positionSpy.count(), 1);
    EXPECT_EQ(scaleSpy.count(), 1);
    EXPECT_EQ(colorSpy.count(), 1);
}

TEST_F(TrajectoryRendererTest, EntitySphereConstructor)
{
    EntitySphere sphere;
    //default values
    EXPECT_EQ(sphere.position(), QVector3D(0, 0, 0));
    EXPECT_EQ(sphere.scale(), QVector3D(1, 1, 1));
    EXPECT_EQ(sphere.materialColor(), QString("#ffffff"));
    EXPECT_EQ(sphere.texturePath(), "");
}

TEST_F(TrajectoryRendererTest, EntitySpherSecondConstructor)
{
    QVector3D position(1.0f, 2.0f, 3.0f);
    QVector3D scale(4.0f, 5.0f, 6.0f);
    QString entityName("TestEntity");
    float timestamp = 123.45f;
    QString materialColor("#ff0000");

    EntitySphere sphere(position, scale, entityName, timestamp, materialColor, "");

    EXPECT_EQ(sphere.position(), position);
    EXPECT_EQ(sphere.scale(), scale);
    EXPECT_EQ(sphere.materialColor(), materialColor);
    EXPECT_EQ(sphere.texturePath(), "");
}

TEST_F(TrajectoryRendererTest, EntitySphereTexturePath)
{
    EntitySphere sphere;
    QString newTexturePath = "qrc:/content/images/earth_8k.jpg";
    QSignalSpy texturePathSpy(&sphere, &EntitySphere::texturePathChanged);
    sphere.setTexturePath(newTexturePath);
    EXPECT_EQ(sphere.texturePath(), newTexturePath);
    EXPECT_EQ(texturePathSpy.count(), 1);
}