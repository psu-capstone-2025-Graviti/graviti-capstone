//#include "pch.h"
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
//    void simulate(float duration, int timeSteps, PhysicalState& currentState, long int callingID, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory) override
//    {
//        simulateCalled = true;
//        lastDuration = duration;
//        lastTimeSteps = timeSteps;
//        lastCallingID = callingID;
//
//        // Add some mock states to the future trajectory
//        for (int i = 0; i < timeSteps; ++i)
//        {
//            PhysicalState mockState;
//            mockState.setPosition({ 1.0f + i, 2.0f + i, 3.0f + i });
//            mockState.setTimestamp(currentState.getTimestamp() + duration * (i + 1));
//            futureTrajectory->push_back(mockState);
//        }
//    }
//};
//
//
//TEST(EntityManagerTests, VerifyEntities)
//{
//    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
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
//    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
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
//
//TEST(EntityManagerTests, VerifyEntitiesDeletedJson)
//{
//    
//    EntityManager* Man = EntityManager::getInstance();
//	Man->addEntityFromJson("EntityJsons/test1.json");
//    auto x = Man->getAllEntities();
//    x->at(0).getEntityName();
//    x->at(0).getPhysicalState()->getMass();
//    EXPECT_EQ(x->at(0).getEntityName(), "earth");
//    EXPECT_FLOAT_EQ(x->at(0).getPhysicalState()->getMass(), 100.0e10f);
//    EXPECT_EQ(x->at(1).getEntityName(), "moon");
//    EXPECT_FLOAT_EQ(x->at(1).getPhysicalState()->getMass(), 1.0e10f);
//    EXPECT_EQ(x->at(2).getEntityName(), "moon2");
//    EXPECT_FLOAT_EQ(x->at(2).getPhysicalState()->getMass(), 1.0e10f);
//
//    EXPECT_THROW(x->at(3).getEntityName(), std::out_of_range);
//    EXPECT_THROW(x->at(3).getPhysicalState(), std::out_of_range);
//
//    // Clear the entities vector to simulate deletion
//    Man->getAllEntities()->clear();
//    auto y = Man->getAllEntities();
//
//    EXPECT_THROW(y->at(0).getEntityName(), std::out_of_range);
//    EXPECT_THROW(y->at(0).getPhysicalState(), std::out_of_range);
//}
//
//
//TEST(EntityManagerTests, VerifyNonExistJson)
//{
//
//    EntityManager* Man = EntityManager::getInstance();
//    Man->addEntityFromJson("EntityJsons/NonExist.json");
//    auto y = Man->getAllEntities();
//    EXPECT_THROW(y->at(0).getEntityName(), std::out_of_range);
//    EXPECT_THROW(y->at(0).getPhysicalState(), std::out_of_range);
//}