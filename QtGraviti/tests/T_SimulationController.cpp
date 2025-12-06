#include <gtest/gtest.h>
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

#include "App/controllers/SimulationControl.h"
#include "GravitiLib/EntityManager.h"
#include "GravitiLib/Entity.h"
#include "rapidjson/document.h"

class SimControllerTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		EntityManager::getInstance()->clearEntities();
		controller = new SimulationController();

		m_mangager = EntityManager::getInstance();
	}

	void TearDown() override
	{
		delete controller;
		controller = nullptr;
		EntityManager::getInstance()->clearEntities();
	}

	SimulationController* controller;
	EntityManager* m_mangager;
};

TEST_F(SimControllerTest, InitializeThreeBody)
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
}

TEST_F(SimControllerTest, CreateEntity)
{
	m_mangager->clearEntities(); //Clear before adding

	controller->createEntity("TestEntity", 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);
	auto entities = m_mangager->getAllEntities();
	ASSERT_FALSE(entities->empty());
	Entity& e = entities->back();
	PhysicalState origin = e.getOrigin();
	EXPECT_EQ(e.getEntityName(), "TestEntity");
	EXPECT_FLOAT_EQ(origin.getPosition().x, 1.0f);
	EXPECT_FLOAT_EQ(origin.getPosition().y, 2.0f);
	EXPECT_FLOAT_EQ(origin.getPosition().z, 3.0f);
	EXPECT_FLOAT_EQ(origin.getVelocity().x, 4.0f);
	EXPECT_FLOAT_EQ(origin.getVelocity().y, 5.0f);
	EXPECT_FLOAT_EQ(origin.getVelocity().z, 6.0f);
	EXPECT_FLOAT_EQ(origin.getMass(), 7.0f);
	EXPECT_FLOAT_EQ(origin.getRadius(), 1.0f);

	controller->createEntity("TestEntity2", 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f);
	ASSERT_FALSE(entities->empty());
	Entity& f = entities->back();
	PhysicalState origin2 = f.getOrigin();
	EXPECT_EQ(f.getEntityName(), "TestEntity2");
	EXPECT_FLOAT_EQ(origin2.getPosition().x,  8.0f);
	EXPECT_FLOAT_EQ(origin2.getPosition().y,  9.0f);
	EXPECT_FLOAT_EQ(origin2.getPosition().z, 10.0f);
	EXPECT_FLOAT_EQ(origin2.getVelocity().x, 11.0f);
	EXPECT_FLOAT_EQ(origin2.getVelocity().y, 12.0f);
	EXPECT_FLOAT_EQ(origin2.getVelocity().z, 13.0f);
	EXPECT_FLOAT_EQ(origin2.getMass(), 14.0f);
	EXPECT_FLOAT_EQ(origin2.getRadius(), 1.0f);
}

TEST_F(SimControllerTest, StartSimulation)
{
	m_mangager->clearEntities();
	controller->createEntity("Runner", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	controller->startSimulation(5, 0.01f);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	controller->pauseSimulation();
	auto entities = m_mangager->getAllEntities();
	ASSERT_FALSE(entities->empty());
	Entity& e = entities->back();
	EXPECT_GT(e.getPastTrajectory().size(), 0);
	EXPECT_LT(e.getPastTrajectory().size(), 150);
	EXPECT_FLOAT_EQ(e.getTimestep(), 0.01f);
}

TEST_F(SimControllerTest, ResetSimulation)
{
	m_mangager->clearEntities();
	controller->createEntity("SimulateEntity", 10.0f, 0.0f, 0.0f, 2.0f, 4.0f, 6.0f, 1.0f);
	auto entities = m_mangager->getAllEntities();
	Entity& e = entities->back();
	PhysicalState originBefore = e.getOrigin();
	controller->startSimulation(3, 0.01f);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	controller->pauseSimulation();
	ASSERT_GT(e.getPastTrajectory().size(), 0);
	ASSERT_LT(e.getPastTrajectory().size(), 150);
	EXPECT_NE(e.getPhysicalState()->getPosition().x, originBefore.getPosition().x);
	EXPECT_NE(e.getPhysicalState()->getPosition().y, originBefore.getPosition().y);
	EXPECT_NE(e.getPhysicalState()->getPosition().z, originBefore.getPosition().z);
	controller->resetSimulation();
	//After resetting, the position and trajectory should be empty, and the origin
	EXPECT_EQ(e.getPastTrajectory().size(), 0);
	EXPECT_FLOAT_EQ(e.getPhysicalState()->getPosition().x, originBefore.getPosition().x);
	EXPECT_FLOAT_EQ(e.getPhysicalState()->getPosition().y, originBefore.getPosition().y);
	EXPECT_FLOAT_EQ(e.getPhysicalState()->getPosition().z, originBefore.getPosition().z);
}

TEST_F(SimControllerTest, ClearSimulation)
{
	controller->createEntity("A", 0, 0, 0, 0, 0, 0, 1);
	controller->createEntity("B", 1, 0, 0, 0, 0, 0, 1);
	auto entities = m_mangager->getAllEntities();
	ASSERT_GE(entities->size(), 2);
	controller->clearEntities();
	EXPECT_EQ(m_mangager->getAllEntities()->size(), 0);
}

TEST_F(SimControllerTest, BatchSimulation)
{
	m_mangager->clearEntities();
	controller->createEntity("SimulateEntity", 10.0f, 0.0f, 0.0f, 2.0f, 4.0f, 6.0f, 1.0f);
	auto entities = m_mangager->getAllEntities();
	Entity& e = entities->back();
	PhysicalState originBefore = e.getOrigin();
	controller->bathProcessFuture(3, 0.01f);
	ASSERT_EQ(e.getPastTrajectory().size(), 3);
	EXPECT_NE(e.getPhysicalState()->getPosition().x, originBefore.getPosition().x);
	EXPECT_NE(e.getPhysicalState()->getPosition().y, originBefore.getPosition().y);
	EXPECT_NE(e.getPhysicalState()->getPosition().z, originBefore.getPosition().z);
}

TEST_F(SimControllerTest, BatchClearSimulation)
{
	m_mangager->clearEntities();
	controller->createEntity("SimulateEntity", 10.0f, 0.0f, 0.0f, 2.0f, 4.0f, 6.0f, 1.0f);
	auto entities = m_mangager->getAllEntities();
	Entity& e = entities->back();
	PhysicalState originBefore = e.getOrigin();
	controller->bathProcessFuture(3, 0.01f);
	ASSERT_EQ(e.getPastTrajectory().size(), 3);
	EXPECT_NE(e.getPhysicalState()->getPosition().x, originBefore.getPosition().x);
	EXPECT_NE(e.getPhysicalState()->getPosition().y, originBefore.getPosition().y);
	EXPECT_NE(e.getPhysicalState()->getPosition().z, originBefore.getPosition().z);

	controller->resetBatch();
	ASSERT_EQ(e.getPastTrajectory().size(), 3); //past trajectory isnt cleared when the entity is moved back
	EXPECT_EQ(e.getPhysicalState()->getPosition().x, originBefore.getPosition().x);
	EXPECT_EQ(e.getPhysicalState()->getPosition().y, originBefore.getPosition().y);
	EXPECT_EQ(e.getPhysicalState()->getPosition().z, originBefore.getPosition().z);
}

TEST_F(SimControllerTest, UpdateEntityData)
{
	m_mangager->clearEntities(); //Clear before adding

	controller->createEntity("TestEntity", 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);
	auto entities = m_mangager->getAllEntities();
	ASSERT_FALSE(entities->empty());
	Entity& e = entities->back();
	PhysicalState origin = e.getOrigin();
	EXPECT_EQ(e.getEntityName(), "TestEntity");
	EXPECT_FLOAT_EQ(origin.getPosition().x, 1.0f);
	EXPECT_FLOAT_EQ(origin.getPosition().y, 2.0f);
	EXPECT_FLOAT_EQ(origin.getPosition().z, 3.0f);
	EXPECT_FLOAT_EQ(origin.getVelocity().x, 4.0f);
	EXPECT_FLOAT_EQ(origin.getVelocity().y, 5.0f);
	EXPECT_FLOAT_EQ(origin.getVelocity().z, 6.0f);
	EXPECT_FLOAT_EQ(origin.getMass(), 7.0f);
	EXPECT_FLOAT_EQ(origin.getRadius(), 1.0f);

	controller->createEntity("TestEntity", 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 0.0f, 2.0f);
	ASSERT_FALSE(entities->empty());
	Entity& f = entities->back();
	PhysicalState origin2 = f.getOrigin();
	EXPECT_EQ(f.getEntityName(), "TestEntity");
	EXPECT_FLOAT_EQ(origin2.getPosition().x, 8.0f);
	EXPECT_FLOAT_EQ(origin2.getPosition().y, 9.0f);
	EXPECT_FLOAT_EQ(origin2.getPosition().z, 10.0f);
	EXPECT_FLOAT_EQ(origin2.getVelocity().x, 11.0f);
	EXPECT_FLOAT_EQ(origin2.getVelocity().y, 12.0f);
	EXPECT_FLOAT_EQ(origin2.getVelocity().z, 13.0f);
	EXPECT_FLOAT_EQ(origin2.getMass(), 7.0f); //Position info should have updated, but mass shouldnt have
	EXPECT_FLOAT_EQ(origin2.getRadius(), 2.0f); //Radius now 2

}