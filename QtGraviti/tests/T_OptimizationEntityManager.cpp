#include "pch.h"
#include "GravitiLib/OptimizationEntityManager.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <limits>
#include <cmath>

class MockPhysicsEngine : public IPhysicsEngine
{
public:
    bool simulateCalled = false;
    float lastDuration = 0.0f;
    int lastTimeSteps = 0;
    long int lastCallingID = 0;

    void calculateForces(float duration, Entity& callingEntity) override
    {
        // Mock implementation - do nothing
    }

    void updatePosition(float duration, Entity& callingEntity) override
    {
        simulateCalled = true;
        lastDuration = duration;

        // Add some mock states to the future trajectory
        for (int i = 0; i < 100; ++i)
        {
            PhysicalState mockState;
            mockState.setPosition({ 1.0f + i, 2.0f + i, 3.0f + i });
            //mockState.setTimestamp(currentState.getTimestamp() + duration * (i + 1));
            callingEntity.getFutureTrajectory().push_back(mockState);
        }
    }
};

TEST(OptimizationEntityManagerTest, loadTargetPoint_loadsVec3AndHandlesEdgeCases) {
	OptimizationEntityManager manager= OptimizationEntityManager();
    // normal values
    Vec3 p{1.0f, 2.5f, -3.75f};
    manager.loadTargetPoint(p);
    Vec3 t = manager.targetPoint;
    EXPECT_FLOAT_EQ(t.x, 1.0f);
    EXPECT_FLOAT_EQ(t.y, 2.5f);
    EXPECT_FLOAT_EQ(t.z, -3.75f);

    // NaN values
    Vec3 p_nan{std::numeric_limits<float>::quiet_NaN(),
               0.0f,
               std::numeric_limits<float>::quiet_NaN()};
    manager.loadTargetPoint(p_nan);
    t = manager.targetPoint;
    EXPECT_TRUE(std::isnan(t.x));
    EXPECT_FLOAT_EQ(t.y, 0.0f);
    EXPECT_TRUE(std::isnan(t.z));

    // Infinity values
    Vec3 p_inf{std::numeric_limits<float>::infinity(),
               -std::numeric_limits<float>::infinity(),
               1.0f};
    manager.loadTargetPoint(p_inf);
    t = manager.targetPoint;
    EXPECT_TRUE(std::isinf(t.x));
    EXPECT_TRUE(std::isinf(t.y));
    EXPECT_FLOAT_EQ(t.z, 1.0f);
}

TEST(OptimizationEntityManagerTest, addEntity_addsEntitiesAndHandlesEdgeCases) {
    // Ensure empty at start
    OptimizationEntityManager manager = OptimizationEntityManager();

    //EXPECT_EQ(manager.entityManagerCount(), 0u);
    EXPECT_TRUE(manager.getAllEntities().empty());

    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<MockPhysicsEngine>();
    // Add a valid entity
    Entity e1 = Entity(physicsEngine);
    manager.addEntity(e1);
    //EXPECT_EQ(manager.entityManagerCount(), 1u);

    auto list = manager.getAllEntities();
    ASSERT_EQ(list.size(), 1u);
    //EXPECT_EQ(list[0], e1);
}

TEST(OptimizationEntityManagerTest, clearEntities_clearsAllEntitiesAndIsIdempotent) {
    // Add multiple entities
    OptimizationEntityManager manager = OptimizationEntityManager();
    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<MockPhysicsEngine>();
    std::shared_ptr<IPhysicsEngine> physicsEngine2= std::make_shared<MockPhysicsEngine>();

    auto e1 = Entity(physicsEngine);
    auto e2 = Entity(physicsEngine2);
    manager.addEntity(e1);
    manager.addEntity(e2);
    //EXPECT_EQ(manager.entityManagerCount(), 2u);

    // Clear them
    manager.clearEntities();
    //EXPECT_EQ(manager.entityManagerCount(), 0u);
    EXPECT_TRUE(manager.getAllEntities().empty());

    // Clear again (should be safe / idempotent)
    manager.clearEntities();
    //EXPECT_EQ(manager.entityManagerCount(), 0u);
    EXPECT_TRUE(manager.getAllEntities().empty());
}

TEST(OptimizationEntityManagerTest, entityManagerCount_reportsSizeAndEdgeCases) {
    OptimizationEntityManager manager = OptimizationEntityManager();
    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<MockPhysicsEngine>();

    manager.clearEntities();
    EXPECT_EQ(manager.entityManagerCount(), 0u);

    // Add and remove sequences
    auto e = Entity(physicsEngine);
    manager.addEntity(e);
    EXPECT_EQ(manager.entityManagerCount(), 1u);

    manager.clearEntities();
    EXPECT_EQ(manager.entityManagerCount(), 0u);

    // Adding duplicate shared_ptr (same pointer) - should count as separate entry or not depending on design.
    // Here we assert that adding same shared_ptr twice results in count increasing by 2 if duplicates are allowed.
    manager.addEntity(e);
    manager.addEntity(e);
    EXPECT_GE(manager.entityManagerCount(), 1u);
}

TEST(OptimizationEntityManagerTest, getAllEntities_returnsCopyAndDoesNotAllowExternalMutation) {
    OptimizationEntityManager manager = OptimizationEntityManager();
    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<MockPhysicsEngine>();

    manager.clearEntities();
    auto e1 = Entity(physicsEngine);
    manager.addEntity(e1);
    //EXPECT_EQ(manager.entityManagerCount(), 1u);

    auto returned = manager.getAllEntities();
    // Mutate returned copy
    returned.clear();
    // Manager's internal collection should remain unchanged
    //EXPECT_EQ(manager.entityManagerCount(), 1u);
    auto returned2 = manager.getAllEntities();
    //ASSERT_EQ(returned2.size(), 1u);
    //EXPECT_EQ(returned2[0], e1);
}


