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
    int lastTimeSteps = 0;
    long int lastCallingID = 0;

    void simulate(float duration, int timeSteps, PhysicalState& currentState, long int callingID, std::shared_ptr<std::vector<PhysicalState>> futureTrajectory) override
    {
        simulateCalled = true;
        lastDuration = duration;
        lastTimeSteps = timeSteps;
        lastCallingID = callingID;

        // Add some mock states to the future trajectory
        for (int i = 0; i < timeSteps; ++i)
        {
            PhysicalState mockState;
            mockState.setPosition({ 1.0f + i, 2.0f + i, 3.0f + i });
            mockState.setTimestamp(currentState.getTimestamp() + duration * (i + 1));
            futureTrajectory->push_back(mockState);
        }
    }
};

class MockEntityManager {
public:
    MockEntityManager(const MockEntityManager&) = delete;
    MockEntityManager& operator=(const MockEntityManager&) = delete;

    static MockEntityManager* getInstance()
    {
        static MockEntityManager* entity_manager = nullptr;
        if (entity_manager == nullptr) {
            entity_manager = new MockEntityManager();
        }
        return entity_manager;
    }

    void addEntity(Entity& entity)
    {
        entity.setID(m_nextID++);
        entities->push_back(entity);
    }

    std::shared_ptr<std::vector<Entity>> getAllEntities();

    long int m_nextID;

    std::shared_ptr<std::vector<Entity>> entities;

    MockEntityManager::MockEntityManager()
        : entities(std::make_shared<std::vector<Entity>>()),
        m_nextID(0)
    {
    }

    MockEntityManager::~MockEntityManager()
    {
        entities->clear();
    }
};

TEST(EntityManagerTests, VerifyEntities)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e(physicsEngine);
    MockEntityManager* Man = MockEntityManager::getInstance();
    Entity earth = Entity(physicsEngine);
    earth.setEntityName("Earth");
    earth.getPhysicalState()->setMass(100.0e10f);
    Man->addEntity(earth);


    auto x = Man->entities;
	x->at(0).getEntityName();
	x->at(0).getPhysicalState()->getMass();
	EXPECT_EQ(x->at(0).getEntityName(), "Earth");
	EXPECT_FLOAT_EQ(x->at(0).getPhysicalState()->getMass(), 100.0e10f);

    
}


TEST(EntityManagerTests, VerifyEntitiesDeleted)
{
    std::unique_ptr<IPhysicsEngine> physicsEngine = std::make_unique<MockPhysicsEngine>();
    Entity e(physicsEngine);
    MockEntityManager* Man = MockEntityManager::getInstance();
    Entity earth = Entity(physicsEngine);
    earth.setEntityName("Earth");
    earth.getPhysicalState()->setMass(100.0e10f);
    Man->addEntity(earth);


    auto x = Man->entities;
    x->at(0).getEntityName();
    x->at(0).getPhysicalState()->getMass();
    EXPECT_EQ(x->at(0).getEntityName(), "Earth");
    EXPECT_FLOAT_EQ(x->at(0).getPhysicalState()->getMass(), 100.0e10f);

    Man ->~MockEntityManager();
    auto y = Man->entities;
    
    EXPECT_THROW(y->at(0).getEntityName(), std::out_of_range);
    EXPECT_THROW(y->at(0).getPhysicalState(), std::out_of_range);
    
}

