#include "pch.h"
#include <QGuiApplication>
#include <QVector3D>
#include <QString>
#include <QObject>
#include <QSignalSpy>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <algorithm>
#include <cstdio>
#include <string>
#include "GravitiLib/RealTimeSimEnvironment.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/EntityManager.h"
#include "App/controllers/SimulationControl.h"
#include <memory>
#include <cmath>

class RealTimeTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		
		env = new RealTimeSimEnvironment();
		controller = new SimulationController();

		m_mangager = EntityManager::getInstance();
		m_mangager->clearEntities();
	}

	void TearDown() override
	{
		delete env;
		delete controller;
		m_mangager->clearEntities();
	}

	RealTimeSimEnvironment* env;
	SimulationController* controller;
	EntityManager* m_mangager;
};

TEST_F(RealTimeTest, DefaultSim)
{
	controller->initializeThreeBody();
	auto entities = m_mangager->getAllEntities();
	ASSERT_EQ(entities->size(), 3);

	bool hasEarth = false;
	bool hasMoon = false;
	bool hasMoon2 = false;
	float moon2X = -1.0f;
	std::string earthTexture;
	if (entities->at(0).getEntityName() == "Earth")
	{
		hasEarth = true;
		earthTexture = entities->at(0).getTexturePath();
	}

	if (entities->at(1).getEntityName() == "Moon")
	{
		hasMoon = true;
	}

	if (entities->at(2).getEntityName() == "moon2")
	{
		hasMoon2 = true;
		moon2X = entities->at(2).getOrigin().getPosition().x;
	}
	EXPECT_TRUE(hasEarth);
	EXPECT_TRUE(hasMoon);
	EXPECT_TRUE(hasMoon2);
	EXPECT_FLOAT_EQ(moon2X, 250.0f);
	EXPECT_FALSE(earthTexture.empty());

	//simulation should already have its own thread since it was created when the env was created
	//Now we can just unpause the sim
	env->setUpdateFunction([]() -> void {printf("Simulation tick!\n"); });

	env->run();
	std::this_thread::sleep_for(std::chrono::seconds(10));
};