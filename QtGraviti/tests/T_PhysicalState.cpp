#include "pch.h"
#include "GravitiLib/PhysicalState.h"
#include <cmath>

// Test default constructor
TEST(PhysicalStateTests, DefaultConstructor)
{
	PhysicalState p;
	
	// Test that all values are initialized to zero
	EXPECT_FLOAT_EQ(p.getPosition().x, 0.0f);
	EXPECT_FLOAT_EQ(p.getPosition().y, 0.0f);
	EXPECT_FLOAT_EQ(p.getPosition().z, 0.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().x, 0.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().y, 0.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().z, 0.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().x, 0.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().y, 0.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().z, 0.0f);
	EXPECT_FLOAT_EQ(p.getTimestamp(), 0.0f);
}

// Test parameterized constructor
TEST(PhysicalStateTests, ParameterizedConstructor)
{
	Vec3 pos = {1.0f, 2.0f, 3.0f};
	Vec3 vel = {4.0f, 5.0f, 6.0f};
	Vec3 acc = {7.0f, 8.0f, 9.0f};
	float mass = 10.0f;
	float radius = 1.5f;
	float timestamp = 1.1f;
	
	PhysicalState p(pos, vel, acc, mass, radius, timestamp);
	
	EXPECT_FLOAT_EQ(p.getPosition().x, 1.0f);
	EXPECT_FLOAT_EQ(p.getPosition().y, 2.0f);
	EXPECT_FLOAT_EQ(p.getPosition().z, 3.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().x, 4.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().y, 5.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().z, 6.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().x, 7.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().y, 8.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().z, 9.0f);
	EXPECT_FLOAT_EQ(p.getMass(), 10.0f);
	EXPECT_FLOAT_EQ(p.getTimestamp(), 1.1f);
}

// Test position getters and setters
TEST(PhysicalStateTests, PositionGettersAndSetters)
{
	PhysicalState p;
	
	// Test Vec3 setter and getter
	Vec3 pos = {1.5f, 2.5f, 3.5f};
	p.setPosition(pos);
	Vec3 result = p.getPosition();
	EXPECT_FLOAT_EQ(result.x, 1.5f);
	EXPECT_FLOAT_EQ(result.y, 2.5f);
	EXPECT_FLOAT_EQ(result.z, 3.5f);
	
	// Test individual component setters and getters
	p.setPosition(X, 10.0f);
	p.setPosition(Y, 20.0f);
	p.setPosition(Z, 30.0f);
	
	EXPECT_FLOAT_EQ(p.getPosition(X), 10.0f);
	EXPECT_FLOAT_EQ(p.getPosition(Y), 20.0f);
	EXPECT_FLOAT_EQ(p.getPosition(Z), 30.0f);
	
	// Test invalid index
	EXPECT_FLOAT_EQ(p.getPosition(99), 0.0f);
}

// Test velocity getters and setters
TEST(PhysicalStateTests, VelocityGettersAndSetters)
{
	PhysicalState p;
	
	// Test Vec3 setter and getter
	Vec3 vel = {4.5f, 5.5f, 6.5f};
	p.setVelocity(vel);
	Vec3 result = p.getVelocity();
	EXPECT_FLOAT_EQ(result.x, 4.5f);
	EXPECT_FLOAT_EQ(result.y, 5.5f);
	EXPECT_FLOAT_EQ(result.z, 6.5f);
	
	// Test individual component setters and getters
	p.setVelocity(X, 11.0f);
	p.setVelocity(Y, 22.0f);
	p.setVelocity(Z, 33.0f);
	
	EXPECT_FLOAT_EQ(p.getVelocity(X), 11.0f);
	EXPECT_FLOAT_EQ(p.getVelocity(Y), 22.0f);
	EXPECT_FLOAT_EQ(p.getVelocity(Z), 33.0f);
	
	// Test invalid index
	EXPECT_FLOAT_EQ(p.getVelocity(99), 0.0f);
}

// Test acceleration getters and setters
TEST(PhysicalStateTests, AccelerationGettersAndSetters)
{
	PhysicalState p;
	
	// Test Vec3 setter and getter
	Vec3 acc = {7.5f, 8.5f, 9.5f};
	p.setAcceleration(acc);
	Vec3 result = p.getAcceleration();
	EXPECT_FLOAT_EQ(result.x, 7.5f);
	EXPECT_FLOAT_EQ(result.y, 8.5f);
	EXPECT_FLOAT_EQ(result.z, 9.5f);
	
	// Test individual component setters and getters
	p.setAcceleration(X, 12.0f);
	p.setAcceleration(Y, 24.0f);
	p.setAcceleration(Z, 36.0f);
	
	EXPECT_FLOAT_EQ(p.getAcceleration(X), 12.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration(Y), 24.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration(Z), 36.0f);
	
	// Test invalid index
	EXPECT_FLOAT_EQ(p.getAcceleration(99), 0.0f);
}

// Test addAcceleration function
TEST(PhysicalStateTests, AddAcceleration)
{
	PhysicalState p;
	p.setAcceleration({1.0f, 2.0f, 3.0f});
	
	Vec3 additional = {0.5f, 1.0f, 1.5f};
	p.addAcceleration(additional);
	
	Vec3 result = p.getAcceleration();
	EXPECT_FLOAT_EQ(result.x, 1.5f);
	EXPECT_FLOAT_EQ(result.y, 3.0f);
	EXPECT_FLOAT_EQ(result.z, 4.5f);
}

// Test mass getter and setter
TEST(PhysicalStateTests, MassGettersAndSetters)
{
	PhysicalState p;
	
	// Test setter and getter
	p.setMass(15.5f);
	EXPECT_FLOAT_EQ(p.getMass(), 15.5f);
	
	// Test with zero mass
	p.setMass(0.0f);
	EXPECT_FLOAT_EQ(p.getMass(), 0.0f);
	
	// Test with negative mass
	p.setMass(-5.0f);
	EXPECT_FLOAT_EQ(p.getMass(), -5.0f);
}

// Test timestamp setter and getter
TEST(PhysicalStateTests, TimestampGettersAndSetters)
{
	PhysicalState p;
	
	// Test setter and getter
	p.setTimestamp(123.45f);
	EXPECT_FLOAT_EQ(p.getTimestamp(), 123.45f);
	
	// Test with zero timestamp
	p.setTimestamp(0.0f);
	EXPECT_FLOAT_EQ(p.getTimestamp(), 0.0f);
	
	// Test with negative timestamp
	p.setTimestamp(-10.0f);
	EXPECT_FLOAT_EQ(p.getTimestamp(), -10.0f);
}

// Test integration function
TEST(PhysicalStateTests, Integration)
{
	PhysicalState p;
	
	// Set initial state
	p.setPosition({0.0f, 0.0f, 0.0f});
	p.setVelocity({1.0f, 2.0f, 3.0f});
	p.setAcceleration({0.5f, 1.0f, 1.5f});
	
	float timestep = 2.0f;
	p.integrate(timestep);
	
	// Check position: pos = pos + vel * timestep
	Vec3 pos = p.getPosition();
	EXPECT_FLOAT_EQ(pos.x, 2.0f);  // 0 + 1 * 2
	EXPECT_FLOAT_EQ(pos.y, 4.0f);  // 0 + 2 * 2
	EXPECT_FLOAT_EQ(pos.z, 6.0f);  // 0 + 3 * 2
	
	// Check velocity: vel = vel + acc * timestep
	Vec3 vel = p.getVelocity();
	EXPECT_FLOAT_EQ(vel.x, 2.0f);  // 1 + 0.5 * 2
	EXPECT_FLOAT_EQ(vel.y, 4.0f);  // 2 + 1 * 2
	EXPECT_FLOAT_EQ(vel.z, 6.0f);  // 3 + 1.5 * 2
}

// Test reset function
TEST(PhysicalStateTests, Reset)
{
	PhysicalState p;
	
	// Set some non-zero values
	p.setPosition({1.0f, 2.0f, 3.0f});
	p.setVelocity({4.0f, 5.0f, 6.0f});
	p.setAcceleration({7.0f, 8.0f, 9.0f});
	p.setTimestamp(10.0f);
	p.setMass(5.0f);
	
	// Reset and verify all values are zero
	p.reset();
	
	EXPECT_FLOAT_EQ(p.getPosition().x, 0.0f);
	EXPECT_FLOAT_EQ(p.getPosition().y, 0.0f);
	EXPECT_FLOAT_EQ(p.getPosition().z, 0.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().x, 0.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().y, 0.0f);
	EXPECT_FLOAT_EQ(p.getVelocity().z, 0.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().x, 0.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().y, 0.0f);
	EXPECT_FLOAT_EQ(p.getAcceleration().z, 0.0f);
	EXPECT_FLOAT_EQ(p.getTimestamp(), 0.0f);
	// Note: mass is not reset by the reset() function
}

// Test copy constructor
TEST(PhysicalStateTests, CopyConstructor)
{
	PhysicalState original;
	original.setPosition({1.0f, 2.0f, 3.0f});
	original.setVelocity({4.0f, 5.0f, 6.0f});
	original.setAcceleration({7.0f, 8.0f, 9.0f});
	original.setTimestamp(10.0f);
	original.setMass(5.0f);
	
	PhysicalState copy(original);
	
	// Verify copy has same values
	EXPECT_FLOAT_EQ(copy.getPosition().x, 1.0f);
	EXPECT_FLOAT_EQ(copy.getPosition().y, 2.0f);
	EXPECT_FLOAT_EQ(copy.getPosition().z, 3.0f);
	EXPECT_FLOAT_EQ(copy.getVelocity().x, 4.0f);
	EXPECT_FLOAT_EQ(copy.getVelocity().y, 5.0f);
	EXPECT_FLOAT_EQ(copy.getVelocity().z, 6.0f);
	EXPECT_FLOAT_EQ(copy.getAcceleration().x, 7.0f);
	EXPECT_FLOAT_EQ(copy.getAcceleration().y, 8.0f);
	EXPECT_FLOAT_EQ(copy.getAcceleration().z, 9.0f);
	EXPECT_FLOAT_EQ(copy.getTimestamp(), 10.0f);
	EXPECT_FLOAT_EQ(copy.getMass(), 5.0f);
}

// Test assignment operator
TEST(PhysicalStateTests, AssignmentOperator)
{
	PhysicalState original;
	original.setPosition({1.0f, 2.0f, 3.0f});
	original.setVelocity({4.0f, 5.0f, 6.0f});
	original.setAcceleration({7.0f, 8.0f, 9.0f});
	original.setTimestamp(10.0f);
	original.setMass(5.0f);

	PhysicalState assigned;
	assigned = original;
	
	// Verify assigned has same values
	EXPECT_FLOAT_EQ(assigned.getPosition().x, 1.0f);
	EXPECT_FLOAT_EQ(assigned.getPosition().y, 2.0f);
	EXPECT_FLOAT_EQ(assigned.getPosition().z, 3.0f);
	EXPECT_FLOAT_EQ(assigned.getVelocity().x, 4.0f);
	EXPECT_FLOAT_EQ(assigned.getVelocity().y, 5.0f);
	EXPECT_FLOAT_EQ(assigned.getVelocity().z, 6.0f);
	EXPECT_FLOAT_EQ(assigned.getAcceleration().x, 7.0f);
	EXPECT_FLOAT_EQ(assigned.getAcceleration().y, 8.0f);
	EXPECT_FLOAT_EQ(assigned.getAcceleration().z, 9.0f);
	EXPECT_FLOAT_EQ(assigned.getTimestamp(), 10.0f);
	EXPECT_FLOAT_EQ(assigned.getMass(), 5.0f);
}

// Test fileExists function
TEST(PhysicalStateTests, FileExists)
{
	PhysicalState p;
	
	// Test with non-existent file
	EXPECT_FALSE(p.fileExists("noFile.txt"));
	
	// Test with empty string
	EXPECT_FALSE(p.fileExists(""));
}