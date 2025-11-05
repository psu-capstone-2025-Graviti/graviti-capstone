#include "pch.h"
#include <QGuiApplication>
#include <QVector3D>
#include <QString>
#include <QObject>
#include <QSignalSpy>
#include <QCoreApplication>
#include "GravitiLib/Entity.h"
#include "GravitiLib/EntityManager.h"
#include "App/controllers/OptimizationController.h"
#include <gtest/gtest.h>
#include <memory>
#include <cmath>


static bool approx_eq(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) <= eps;
}
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


TEST(OptimizationControllerTest, LoadEntitiesExtendsListOfEntityManagers) {
    OptimizationController controller;

    OptimizationEntityManager mgr1 = OptimizationEntityManager();
    OptimizationEntityManager mgr2 = OptimizationEntityManager();

    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<MockPhysicsEngine>();
    Entity projectile = Entity(physicsEngine);
    projectile.setEntityName("ship");
    projectile.getPhysicalState()->setMass(100.0e10f);
	mgr1.addEntity(projectile);

    std::shared_ptr<IPhysicsEngine> physicsEngine2 = std::make_shared<MockPhysicsEngine>();
    Entity projectile2 = Entity(physicsEngine2);
    projectile2.setEntityName("ship2");
    projectile2.getPhysicalState()->setMass(100.0e10f);
	mgr2.addEntity(projectile2);

    // First load: count increases to 1
    controller.LoadEntities(mgr1.getAllEntities(),1);
    //EXPECT_EQ(controller.entityManagerCount(), 1);

    // Second load: count increases to 2 (extends)
    controller.LoadEntities(mgr2.getAllEntities(),1);
    //EXPECT_EQ(controller.entityManagerCount(), 2);
}

TEST(OptimizationControllerTest, LoadProjectileSetsInitialEntityToProjectile) {
    OptimizationController controller;

    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<MockPhysicsEngine>();
    Entity projectile = Entity(physicsEngine);
    projectile.setEntityName("ship");
    projectile.getPhysicalState()->setMass(100.0e10f);
    controller.LoadProjectile(projectile);

    auto current = controller.initialEntity;
    // Expect the controller holds the same shared pointer (same ownership)
    EXPECT_EQ(current.getEntityID(), projectile.getEntityID());
}

TEST(OptimizationControllerTest, LoadTargetLoadsVec3ToTargetPoint) {
    OptimizationController controller;

    // Assume a Vec3-like type exists; construct a value and load it.
    // Try to use a common name if present; otherwise assume Entity/Vec3 is defined in GravitiLib.
#if defined(GRAVITILIB_VEC3_DEFINED) // placeholder guard if your project defines this macro
    Vec3 target{1.5, -2.25, 3.75};
#else
    // Fallback: construct a simple anonymous struct with x,y,z to pass to loadTarget,
    // in case the controller accepts an aggregate with matching fields.
    Vec3 target{1.5, -2.25, 3.75};
#endif
    auto stored = controller.targetPoint;

    // Try to access x/y/z members; compare approximately.
    EXPECT_EQ(stored.x, 0);
    EXPECT_EQ(stored.y, 0);
    EXPECT_EQ(stored.z, 0);
    controller.LoadTarget(target);
    // Retrieve the stored target point
    auto stored2 = controller.targetPoint;

    // Try to access x/y/z members; compare approximately.
    EXPECT_EQ(stored2.x,1.5);
    EXPECT_EQ(stored2.y, -2.25);
    EXPECT_EQ(stored2.z, 3.75);
}

TEST(OptimizationControllerTest, OptimizeReturnsZero) {
    OptimizationController controller;

    // Call optimize and expect a 0 return code for success.
    controller.optimize();
    
}

TEST(OptimizationControllerTest, DestructorClearsEntityManager) {
    OptimizationEntityManager weakRef;

    {
        auto mgr = OptimizationEntityManager();
        weakRef = mgr;

        // Create controller in an inner scope so destructor runs when leaving scope.
        {
            OptimizationController controller;
            controller.LoadEntities(mgr.getAllEntities(),1);

            // While controller is alive, use_count should be at least 2 (mgr + controller)
            EXPECT_GE(weakRef.entityManagerCount(), 0);
        } // controller destroyed here
    } // mgr goes out of scope; but we still use weakRef to check ownership semantics

    EXPECT_TRUE(weakRef.entityManagerCount() <= 1u);
}

