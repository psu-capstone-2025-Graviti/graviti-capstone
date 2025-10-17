#include "pch.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"
#include "GravitiLib/EntityManager.h"
#include "GravitiLib/BatchSimEnvironment.h"
#include <memory>

// Mock physics engine for testing


TEST(BatchSimTests, twoBody)
{
    BatchSimEnvironment batch;
    batch.initialize_two_body();
    batch.run();
    auto man = EntityManager::getInstance();
    auto e = man->getAllEntities();
    EXPECT_EQ(e->at(0).getEntityName(), "Earth");
    EXPECT_FLOAT_EQ(e->at(0).getPhysicalState()->getMass(), 100.0e10f);
    // Test default values
    EXPECT_EQ(e->at(1).getEntityName(), "Moon");
    EXPECT_EQ(e->at(1).getEntityID(), 1);
    EXPECT_NE(e->at(1).getPhysicalState(), nullptr);

    EXPECT_THROW(e->at(2).getEntityName(), std::out_of_range);


}


TEST(BatchSimTests, threeBody)
{
    BatchSimEnvironment batch;
    batch.initialize_three_body();
    batch.run();
    auto man = EntityManager::getInstance();
    auto e = man->getAllEntities();
    
    // Test default values
    EXPECT_EQ(e->at(0).getEntityName(), "Earth");
    EXPECT_EQ(e->at(0).getEntityID(), 0);
    EXPECT_NE(e->at(0).getPhysicalState(), nullptr);

    EXPECT_EQ(e->at(1).getEntityName(), "Moon");
    EXPECT_EQ(e->at(1).getEntityID(), 1);
    EXPECT_NE(e->at(1).getPhysicalState(), nullptr);
    
    EXPECT_EQ(e->at(2).getEntityName(), "moon2");
    EXPECT_EQ(e->at(2).getEntityID(), 2);
    EXPECT_NE(e->at(2).getPhysicalState(), nullptr);

    EXPECT_THROW(e->at(3).getEntityName(), std::out_of_range);
}

TEST(BatchSimTests, json)
{
    BatchSimEnvironment batch;
    batch.initialize_json_body("EntityJsons/test1.json");
    batch.run();
    auto man = EntityManager::getInstance();
    auto& e = man->getAllEntities();
    
    // Test default values
    EXPECT_EQ(e->at(0).getEntityName(), "earth");
    EXPECT_EQ(e->at(0).getEntityID(), 0);
    EXPECT_NE(e->at(0).getPhysicalState(), nullptr);

    EXPECT_EQ(e->at(1).getEntityName(), "moon");
    EXPECT_EQ(e->at(1).getEntityID(), 1);
    EXPECT_NE(e->at(1).getPhysicalState(), nullptr);

    EXPECT_EQ(e->at(2).getEntityName(), "moon2");
    EXPECT_EQ(e->at(2).getEntityID(), 2);
    EXPECT_NE(e->at(2).getPhysicalState(), nullptr);

    EXPECT_THROW(e->at(3).getEntityName(), std::out_of_range);
}

TEST(BatchSimTests, saveJson)
{
    BatchSimEnvironment batch;
    batch.initialize_three_body();
    batch.run();
    auto man = EntityManager::getInstance();
    auto e = man->getAllEntities();


	batch.saveEntitiesAsJson("EntityJsons/testoutput.json");
    // Test default values
    EXPECT_EQ(e->at(0).getEntityName(), "Earth");
    EXPECT_EQ(e->at(0).getEntityID(), 0);
    EXPECT_NE(e->at(0).getPhysicalState(), nullptr);

    EXPECT_EQ(e->at(1).getEntityName(), "Moon");
    EXPECT_EQ(e->at(1).getEntityID(), 1);
    EXPECT_NE(e->at(1).getPhysicalState(), nullptr);

    EXPECT_EQ(e->at(2).getEntityName(), "moon2");
    EXPECT_EQ(e->at(2).getEntityID(), 2);
    EXPECT_NE(e->at(2).getPhysicalState(), nullptr);

    EXPECT_THROW(e->at(3).getEntityName(), std::out_of_range);
}
