#include "../../pch.h"
#include "src/Simulation/NBodyEngine.h"
#include "src/Simulation/Entity.h"
#include "src/Simulation/EntityManager.h"
#include <memory>
#include <cmath>

// Test default constructor
TEST(NBodyEngineTests, Constructor)
{
    NBodyPhysics physicsEngine;
    SUCCEED();
}

// Test simulate function with single entity (no gravitational forces)
TEST(NBodyEngineTests, SimulateSingleEntity)
{
    NBodyPhysics physicsEngine;
    
    // Create a test entity
    std::unique_ptr<IPhysicsEngine> engine = std::make_unique<NBodyPhysics>();
    Entity testEntity(engine);
    testEntity.setID(1);
    
    // Set up initial state
    PhysicalState initialState;
    initialState.setPosition({0.0f, 0.0f, 0.0f});
    initialState.setVelocity({1.0f, 0.0f, 0.0f});
    initialState.setAcceleration({0.0f, 0.0f, 0.0f});
    initialState.setMass(1.0f);
    initialState.setTimestamp(0.0f);
    testEntity.setOrigin(initialState);
    
    // Create future trajectory vector
    auto futureTrajectory = std::make_shared<std::vector<PhysicalState>>();
    
    // Run simulation
    physicsEngine.simulate(1.0f, 1, initialState, 1, futureTrajectory);
    
    // Verify that acceleration was set (should be zero for single entity)
    EXPECT_FLOAT_EQ(initialState.getAcceleration().x, 0.0f);
    EXPECT_FLOAT_EQ(initialState.getAcceleration().y, 0.0f);
    EXPECT_FLOAT_EQ(initialState.getAcceleration().z, 0.0f);
}

// Test gravitational force calculation with known values
TEST(NBodyEngineTests, GravitationalForceCalculation)
{
    NBodyPhysics physicsEngine;
    
    // Create two entities with known masses and positions
    std::unique_ptr<IPhysicsEngine> engine1 = std::make_unique<NBodyPhysics>();
    std::unique_ptr<IPhysicsEngine> engine2 = std::make_unique<NBodyPhysics>();
    
    Entity entity1(engine1);
    Entity entity2(engine2);
    
    entity1.setID(1);
    entity2.setID(2);
    
    // Set up entity1 at origin
    PhysicalState state1;
    state1.setPosition({0.0f, 0.0f, 0.0f});
    state1.setVelocity({0.0f, 0.0f, 0.0f});
    state1.setAcceleration({0.0f, 0.0f, 0.0f});
    state1.setMass(1.0f); // 1 kg
    state1.setTimestamp(0.0f);
    entity1.setOrigin(state1);
    entity1.setEntityName("Entity1");
    
    // Set up entity2 at distance 1m along x-axis
    PhysicalState state2;
    state2.setPosition({1.0f, 0.0f, 0.0f});
    state2.setVelocity({0.0f, 0.0f, 0.0f});
    state2.setAcceleration({0.0f, 0.0f, 0.0f});
    state2.setMass(1.0f); // 1 kg
    state2.setTimestamp(0.0f);
    entity2.setOrigin(state2);
    entity2.setEntityName("Entity2");
    
    // Add entities to EntityManager
    auto entityManager = EntityManager::getInstance();
    entityManager->addEntity(entity1);
    entityManager->addEntity(entity2);
    
    // Create future trajectory vector
    auto futureTrajectory = std::make_shared<std::vector<PhysicalState>>();
    
    // Run simulation on entity1
    physicsEngine.simulate(1.0f, 1, state1, 0, futureTrajectory);
    
    // Calculate expected acceleration using F = G * m1 * m2 / r^2
    // and a = F / m1 = G * m2 / r^2
    float G = 6.67430e-11f; // Gravitational constant
    float r = 1.0f; // Distance between entities
    float expectedAcceleration = G / (r * r);
    
    // Verify acceleration is approximately correct (within floating point precision)
    EXPECT_NEAR(state1.getAcceleration().x, expectedAcceleration, 1e-15f);
    EXPECT_FLOAT_EQ(state1.getAcceleration().y, 0.0f);
    EXPECT_FLOAT_EQ(state1.getAcceleration().z, 0.0f);
}

// Test simulation with entities at same position (division by zero protection)
TEST(NBodyEngineTests, SimulateSamePosition)
{
    NBodyPhysics physicsEngine;
    
    // Create two entities at the same position
    std::unique_ptr<IPhysicsEngine> engine1 = std::make_unique<NBodyPhysics>();
    std::unique_ptr<IPhysicsEngine> engine2 = std::make_unique<NBodyPhysics>();
    
    Entity entity1(engine1);
    Entity entity2(engine2);
    
    entity1.setID(1);
    entity2.setID(2);
    
    // Set up entity1 at origin
    PhysicalState state1;
    state1.setPosition({0.0f, 0.0f, 0.0f});
    state1.setVelocity({0.0f, 0.0f, 0.0f});
    state1.setAcceleration({0.0f, 0.0f, 0.0f});
    state1.setMass(1.0f);
    state1.setTimestamp(0.0f);
    entity1.setOrigin(state1);
    
    // Set up entity2 at the same position
    PhysicalState state2;
    state2.setPosition({0.0f, 0.0f, 0.0f});
    state2.setVelocity({0.0f, 0.0f, 0.0f});
    state2.setAcceleration({0.0f, 0.0f, 0.0f});
    state2.setMass(1.0f);
    state2.setTimestamp(0.0f);
    entity2.setOrigin(state2);
    
    // Add entities to EntityManager
    auto entityManager = EntityManager::getInstance();
    entityManager->addEntity(entity1);
    entityManager->addEntity(entity2);
    
    // Create future trajectory vector
    auto futureTrajectory = std::make_shared<std::vector<PhysicalState>>();
    
    // Run simulation on entity1
    physicsEngine.simulate(1.0f, 1, state1, 1, futureTrajectory);
    
    // Verify that acceleration is zero (division by zero protection)
    EXPECT_FLOAT_EQ(state1.getAcceleration().x, 0.0f);
    EXPECT_FLOAT_EQ(state1.getAcceleration().y, 0.0f);
    EXPECT_FLOAT_EQ(state1.getAcceleration().z, 0.0f);
}

// Test simulation with different masses
TEST(NBodyEngineTests, SimulateDifferentMasses)
{
    NBodyPhysics physicsEngine;
    
    // Create two entities with different masses
    std::unique_ptr<IPhysicsEngine> engine1 = std::make_unique<NBodyPhysics>();
    std::unique_ptr<IPhysicsEngine> engine2 = std::make_unique<NBodyPhysics>();
    
    Entity entity1(engine1);
    Entity entity2(engine2);
    
    entity1.setID(1);
    entity2.setID(2);
    
    // Set up entity1 at origin
    PhysicalState state1;
    state1.setPosition({0.0f, 0.0f, 0.0f});
    state1.setVelocity({0.0f, 0.0f, 0.0f});
    state1.setAcceleration({0.0f, 0.0f, 0.0f});
    state1.setMass(1.0f);
    state1.setTimestamp(0.0f);
    entity1.setOrigin(state1);
    
    // Set up entity2 with larger mass
    PhysicalState state2;
    state2.setPosition({1.0f, 0.0f, 0.0f});
    state2.setVelocity({0.0f, 0.0f, 0.0f});
    state2.setAcceleration({0.0f, 0.0f, 0.0f});
    state2.setMass(10.0f); // 10 times larger mass
    state2.setTimestamp(0.0f);
    entity2.setOrigin(state2);
    
    // Add entities to EntityManager
    auto entityManager = EntityManager::getInstance();
    entityManager->addEntity(entity1);
    entityManager->addEntity(entity2);
    
    // Create future trajectory vector
    auto futureTrajectory = std::make_shared<std::vector<PhysicalState>>();
    
    // Run simulation on entity1
    physicsEngine.simulate(1.0f, 1, state1, 1, futureTrajectory);
    
    // Calculate expected acceleration with larger mass
    float G = 6.67430e-11f;
    float m2 = 10.0f; // Larger mass
    float r = 1.0f;
    float expectedAcceleration = G * m2 / (r * r);
    
    // Verify acceleration is approximately correct (should be 10 times larger)
    EXPECT_NEAR(state1.getAcceleration().x, expectedAcceleration, 1e-15f);
    EXPECT_FLOAT_EQ(state1.getAcceleration().y, 0.0f);
    EXPECT_FLOAT_EQ(state1.getAcceleration().z, 0.0f);
}