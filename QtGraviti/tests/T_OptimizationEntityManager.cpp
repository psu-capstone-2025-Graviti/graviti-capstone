//#include "pch.h"
//#include <QGuiApplication>
//#include <QVector3D>
//#include <QString>
//#include <QObject>
//#include <QSignalSpy>
//#include <QCoreApplication>
//#include "GravitiLib/Entity.h"
//#include "GravitiLib/NBodyEngine.h"
//#include "GravitiLib/EntityManager.h"
//#include <memory>
//
//// Mock physics engine for testing
//class MockPhysicsEngine : public IPhysicsEngine
//{
//public:
//    bool simulateCalled = false;
//    float lastDuration = 0.0f;
//    int lastTimeSteps = 0;
//    long int lastCallingID = 0;
//
//    void calculateForces(float duration, Entity& callingEntity) override
//    {
//        // Mock implementation - do nothing
//    }
//
//    void updatePosition(float duration, Entity& callingEntity) override
//    {
//        simulateCalled = true;
//        lastDuration = duration;
//
//        // Add some mock states to the future trajectory
//        for (int i = 0; i < 100; ++i)
//        {
//            PhysicalState mockState;
//            mockState.setPosition({ 1.0f + i, 2.0f + i, 3.0f + i });
//            //mockState.setTimestamp(currentState.getTimestamp() + duration * (i + 1));
//            callingEntity.getFutureTrajectory().push_back(mockState);
//        }
//    }
//};
//
//
//TEST(EntityManagerTests, VerifyEntities)
//{
//    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<MockPhysicsEngine>();
//    Entity e(physicsEngine);
//    EntityManager* Man = EntityManager::getInstance();
//    Entity earth = Entity(physicsEngine);
//    earth.setEntityName("Earth");
//    earth.getPhysicalState()->setMass(100.0e10f);
//    Man->addEntity(earth);
//
//
//    auto x = Man->getAllEntities();
//	x->at(0).getEntityName();
//	x->at(0).getPhysicalState()->getMass();
//	EXPECT_EQ(x->at(0).getEntityName(), "Earth");
//	EXPECT_FLOAT_EQ(x->at(0).getPhysicalState()->getMass(), 100.0e10f);
//
//    
//}
//
//
//TEST(EntityManagerTests, VerifyEntitiesDeleted)
//{
//    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<MockPhysicsEngine>();
//    Entity e(physicsEngine);
//    EntityManager* Man = EntityManager::getInstance();
//    Entity earth = Entity(physicsEngine);
//    earth.setEntityName("Earth");
//    earth.getPhysicalState()->setMass(100.0e10f);
//    Man->addEntity(earth);
//
//    auto x = Man->getAllEntities();
//    x->at(0).getEntityName();
//    x->at(0).getPhysicalState()->getMass();
//    EXPECT_EQ(x->at(0).getEntityName(), "Earth");
//    EXPECT_FLOAT_EQ(x->at(0).getPhysicalState()->getMass(), 100.0e10f);
//
//    // Clear the entities vector to simulate deletion
//    Man->getAllEntities()->clear();
//    auto y = Man->getAllEntities();
//
//    EXPECT_THROW(y->at(0).getEntityName(), std::out_of_range);
//    EXPECT_THROW(y->at(0).getPhysicalState(), std::out_of_range);
//}
//
