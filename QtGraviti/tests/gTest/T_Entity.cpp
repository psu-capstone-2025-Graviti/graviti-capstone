#include "pch.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"
#include "GravitiLib/EntityManager.h"
#include <memory>

// Mock physics engine for testing
class MockPhysicsEngine : public IPhysicsEngine
{
public:
    bool simulateCalled = false;
    float lastDuration = 0.0f;
    float accumulatedTime = 0;
    long int lastCallingID = 0;

    void calculateForces(float duration, Entity& callingEntity) override
    {
        // Mock implementation - do nothing
    }

    void updatePosition(float duration, Entity& callingEntity) override
    {
        simulateCalled = true;
        lastDuration = duration;
        accumulatedTime += duration;
        // Add some mock states to the future trajectory

        //PhysicalState mockState;
        //auto curr_state = mockState.getPosition();
        //mockState.setPosition({ 1.0f + 1, 2.0f + i, 3.0f + i });
        //callingEntity.getFutureTrajectory().push_back(mockState);
        //}
    }
};

TEST(EntityTests, Constructor)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e(physicsEngine);
    
    // Test default values
    EXPECT_EQ(e.getEntityName(), "TEST");
    EXPECT_EQ(e.getEntityID(), 0);
    EXPECT_NE(e.getPhysicalState(), nullptr);
}

TEST(EntityTests, EntityNameGettersAndSetters)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e(physicsEngine);
    
    // Test default name
    EXPECT_EQ(e.getEntityName(), "TEST");
    
    // Test setting new name
    e.setEntityName("TestEntity");
    EXPECT_EQ(e.getEntityName(), "TestEntity");
    
    // Test setting empty name
    e.setEntityName("");
    EXPECT_EQ(e.getEntityName(), "");
    
    // Test setting name with special characters
    e.setEntityName("Entity-123_Test");
    EXPECT_EQ(e.getEntityName(), "Entity-123_Test");
}

TEST(EntityTests, EntityIDGettersAndSetters)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e(physicsEngine);
    
    // Test default ID
    EXPECT_EQ(e.getEntityID(), 0);
    
    // Test setting new ID
    e.setID(123);
    EXPECT_EQ(e.getEntityID(), 123);
    
    // Test setting negative ID
    e.setID(-1);
    EXPECT_EQ(e.getEntityID(), -1);
    
    // Test setting large ID
    e.setID(999999);
    EXPECT_EQ(e.getEntityID(), 999999);
}


TEST(EntityTests, TimestepSetter)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e(physicsEngine);
    
    // Test setting timestep
    e.setTimestep(0.1f);
    EXPECT_FLOAT_EQ(e.getTimestep(), 0.1f);
    
    // Test setting zero timestep
    e.setTimestep(0.0f);
    EXPECT_FLOAT_EQ(e.getTimestep(), 0.0f);
    
    // Test setting negative timestep
    e.setTimestep(-0.1f);
    EXPECT_FLOAT_EQ(e.getTimestep(), -0.1f);
    
    // Test setting large timestep
    e.setTimestep(1000.0f);
    EXPECT_FLOAT_EQ(e.getTimestep(), 1000.0f);
}


TEST(EntityTests, SetOrigin)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e(physicsEngine);
    
    // Create a test origin state
    PhysicalState origin;
    origin.setPosition({10.0f, 20.0f, 30.0f});
    origin.setVelocity({1.0f, 2.0f, 3.0f});
    origin.setAcceleration({0.1f, 0.2f, 0.3f});
    origin.setTimestamp(100.0f);
    origin.setMass(5.0f);
    
    // Set the origin
    e.setOrigin(origin);
    
    // Verify the current state was set
    PhysicalState* currentState = e.getPhysicalState();
    EXPECT_NE(currentState, nullptr);
    EXPECT_FLOAT_EQ(currentState->getPosition().x, 10.0f);
    EXPECT_FLOAT_EQ(currentState->getPosition().y, 20.0f);
    EXPECT_FLOAT_EQ(currentState->getPosition().z, 30.0f);
    EXPECT_FLOAT_EQ(currentState->getVelocity().x, 1.0f);
    EXPECT_FLOAT_EQ(currentState->getVelocity().y, 2.0f);
    EXPECT_FLOAT_EQ(currentState->getVelocity().z, 3.0f);
    EXPECT_FLOAT_EQ(currentState->getAcceleration().x, 0.1f);
    EXPECT_FLOAT_EQ(currentState->getAcceleration().y, 0.2f);
    EXPECT_FLOAT_EQ(currentState->getAcceleration().z, 0.3f);
    EXPECT_FLOAT_EQ(currentState->getTimestamp(), 100.0f);
    EXPECT_FLOAT_EQ(currentState->getMass(), 5.0f);
}


TEST(EntityTests, GetPhysicalState)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e(physicsEngine);
    
    // Test that getPhysicalState returns a valid pointer
    PhysicalState* state = e.getPhysicalState();
    EXPECT_NE(state, nullptr);
    
    // Test that we can modify the state through the returned pointer
    state->setPosition({1.0f, 2.0f, 3.0f});
    state->setVelocity({4.0f, 5.0f, 6.0f});
    
    // Verify the changes were applied
    PhysicalState* state2 = e.getPhysicalState();
    EXPECT_FLOAT_EQ(state2->getPosition().x, 1.0f);
    EXPECT_FLOAT_EQ(state2->getPosition().y, 2.0f);
    EXPECT_FLOAT_EQ(state2->getPosition().z, 3.0f);
    EXPECT_FLOAT_EQ(state2->getVelocity().x, 4.0f);
    EXPECT_FLOAT_EQ(state2->getVelocity().y, 5.0f);
    EXPECT_FLOAT_EQ(state2->getVelocity().z, 6.0f);
}


TEST(EntityTests, SimulateWithValidEngine)
{
    auto mockEngine = std::make_unique<MockPhysicsEngine>();
    MockPhysicsEngine* mockPtr = mockEngine.get();
    
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::move(mockEngine);
    Entity e(physicsEngine);
    
    // Set up initial state
    PhysicalState initialState;
    initialState.setPosition({0.0f, 0.0f, 0.0f});
    initialState.setTimestamp(0.0f);
    e.setOrigin(initialState);
    e.setTimestep(0.1f);
    float timestep = 0.1f;
    // Run simulation
    bool result = false;
    for (int i = 0; i < 50; i++)
    {
        result = e.Simulate(timestep);
        e.TickForward();
    }
    
    // Verify simulation was successful
    EXPECT_TRUE(result);
    EXPECT_TRUE(mockPtr->simulateCalled);
    EXPECT_FLOAT_EQ(mockPtr->lastDuration, 0.1f);
    EXPECT_EQ(mockPtr->lastCallingID, e.getEntityID());
}


TEST(EntityTests, SimulateWithNullEngine)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = nullptr;
    Entity e(physicsEngine);
    
    // Run simulation
    bool result = e.Simulate(5);
    
    // Verify simulation failed
    EXPECT_FALSE(result);
}


TEST(EntityTests, EntityWithNBodyPhysics)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<NBodyPhysics>();
    Entity e(physicsEngine);
    
    // Test basic functionality
    EXPECT_EQ(e.getEntityName(), "TEST");
    EXPECT_EQ(e.getEntityID(), 0);
    EXPECT_NE(e.getPhysicalState(), nullptr); //Entity should always initialize with a valid physical state
    
    // Test setting properties
    e.setEntityName("NBodyEntity");
    e.setTimestep(0.01f);
    
    EXPECT_EQ(e.getEntityName(), "NBodyEntity");
    
    // Test setting origin
    PhysicalState origin;
    origin.setPosition({1.0f, 2.0f, 3.0f});
    origin.setVelocity({0.1f, 0.2f, 0.3f});
    origin.setMass(1.0f);
    e.setOrigin(origin);
    
    // Verify origin was set
    PhysicalState* currentState = e.getPhysicalState();
    EXPECT_FLOAT_EQ(currentState->getPosition().x, 1.0f);
    EXPECT_FLOAT_EQ(currentState->getPosition().y, 2.0f);
    EXPECT_FLOAT_EQ(currentState->getPosition().z, 3.0f);
    EXPECT_FLOAT_EQ(currentState->getVelocity().x, 0.1f);
    EXPECT_FLOAT_EQ(currentState->getVelocity().y, 0.2f);
    EXPECT_FLOAT_EQ(currentState->getVelocity().z, 0.3f);
    EXPECT_FLOAT_EQ(currentState->getMass(), 1.0f);
}


TEST(EntityTests, EntityIDAssignment)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e1(physicsEngine);
    Entity e2(physicsEngine);
    
    // Test that entities start with different IDs (if auto-assigned)
    // or same IDs (if manually assigned)
    long int id1 = e1.getEntityID();
    long int id2 = e2.getEntityID();
    
    // Set different IDs
    e1.setID(100);
    e2.setID(200);
    
    EXPECT_EQ(e1.getEntityID(), 100);
    EXPECT_EQ(e2.getEntityID(), 200);
    EXPECT_NE(e1.getEntityID(), e2.getEntityID());
}


TEST(EntityTests, MultipleSimulationCalls)
{
    auto mockEngine = std::make_unique<MockPhysicsEngine>();
    MockPhysicsEngine* mockPtr = mockEngine.get();
    
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::move(mockEngine);
    Entity e(physicsEngine);
    
    e.setTimestep(0.1f);
    
    // First simulation
    bool result1 = e.Simulate(3);
    e.TickForward();
    EXPECT_TRUE(result1);
    EXPECT_TRUE(mockPtr->simulateCalled);
    
    // Reset the mock
    mockPtr->simulateCalled = false;
    
    // Second simulation
    bool result2 = e.Simulate(7);
    e.TickForward();
    EXPECT_TRUE(result2);
    EXPECT_TRUE(mockPtr->simulateCalled);
}