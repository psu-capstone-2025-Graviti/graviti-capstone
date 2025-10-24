#include "pch.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"
#include "GravitiLib/EntityManager.h"
#include "GravitiLib/BatchSimEnvironment.h"
#include <memory>

// Mock physics engine for testing

class BatchSimeEnvTesting : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    BatchSimEnvironment* m_env;
    EntityManager* manager;
};

void BatchSimeEnvTesting::SetUp()
{
    m_env = new BatchSimEnvironment();
    manager = EntityManager::getInstance();
}

void BatchSimeEnvTesting::TearDown()
{
    delete m_env;
	manager->clearEntities();
}

TEST_F(BatchSimeEnvTesting, Constructor)
{
    EXPECT_FLOAT_EQ(0.0f, m_env->getOriginTime());
}

TEST_F(BatchSimeEnvTesting, RunningSimpleSim)
{
	std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();

	Entity newEntity(physicsEngine);
	newEntity.setEntityName("Name");
	PhysicalState entityState;
	entityState.setPosition(X, 0);
	entityState.setPosition(Y, 0);
	entityState.setPosition(Z, 0);
	entityState.setVelocity(X, 0);
	entityState.setVelocity(Y, 0);
	entityState.setVelocity(Z, 1);
	entityState.setMass(1);
	entityState.setRadius(0.2f);

	newEntity.setOrigin(entityState);
	manager->addEntity(newEntity);

	m_env->run(1, 1);

	auto entities = manager->getAllEntities();

	auto testEntity = entities->at(0);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().x, 0);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().y, 0);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().z, 1);
}

TEST_F(BatchSimeEnvTesting, RunningComplexSim)
{
	std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();

	Entity newEntity(physicsEngine);
	newEntity.setEntityName("One");
	PhysicalState entityState;
	entityState.setPosition(X, 0);
	entityState.setPosition(Y, 0);
	entityState.setPosition(Z, 0);
	entityState.setVelocity(X, 0);
	entityState.setVelocity(Y, 0);
	entityState.setVelocity(Z, 1);
	entityState.setMass(100000000000);
	entityState.setRadius(0.2f);
	newEntity.setOrigin(entityState);
	manager->addEntity(newEntity);

	Entity twoEntity(physicsEngine);
	twoEntity.setEntityName("One");
	PhysicalState twoState;
	twoState.setPosition(X, 0);
	twoState.setPosition(Y, 0);
	twoState.setPosition(Z, 25);
	twoState.setVelocity(X, 0.5);
	twoState.setVelocity(Y, 0);
	twoState.setVelocity(Z, 0);
	twoState.setMass(1);
	twoState.setRadius(0.2f);
	twoEntity.setOrigin(twoState);
	manager->addEntity(twoEntity);

	m_env->run(200, 0.01);

	auto entities = manager->getAllEntities();

	auto testEntity = entities->at(1);

	//std::cout << testEntity.getPhysicalState()->getPosition().x << std::endl;
	//std::cout << testEntity.getPhysicalState()->getPosition().z << std::endl;

	//These tests are not meant to evaluate the accuracy of the simulation.
	//Those tests are in T_NBodyEngine. These are just meant to proove that
	//Physics calculations are occuring and that changing code in the future doesn't
	//change these results

	EXPECT_FLOAT_EQ(testEntity.getPhysicalState()->getPosition().x, 0.99968);
	EXPECT_FLOAT_EQ(testEntity.getPhysicalState()->getPosition().y, 0);
	EXPECT_FLOAT_EQ(testEntity.getPhysicalState()->getPosition().z, 24.97744);
}

TEST_F(BatchSimeEnvTesting, ClearingSim)
{
	std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();

	Entity newEntity(physicsEngine);
	newEntity.setEntityName("Name");
	PhysicalState entityState;
	entityState.setPosition(X, 0);
	entityState.setPosition(Y, 0);
	entityState.setPosition(Z, 0);
	entityState.setVelocity(X, 1);
	entityState.setVelocity(Y, 2);
	entityState.setVelocity(Z, 3);
	entityState.setMass(1);
	entityState.setRadius(0.2f);

	newEntity.setOrigin(entityState);
	manager->addEntity(newEntity);

	m_env->run(1, 1);

	auto entities = manager->getAllEntities();

	auto testEntity = entities->at(0);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().x, 1);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().y, 2);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().z, 3);
	
	//After calling resetSimulation the entity should be back at its original position
	m_env->resetSimulation();

	auto resetentities = manager->getAllEntities();

	testEntity = resetentities->at(0);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().x, 0);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().y, 0);
	EXPECT_EQ(testEntity.getPhysicalState()->getPosition().z, 0);
}