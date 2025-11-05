#include "SimulationControl.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"
#include "GravitiLib/OptimizationEntityManager.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "OptimizationController.h"


OptimizationController::OptimizationController(QObject* parent)
	: QObject(parent),
	m_env()
{
	//m_entityList = new EntityListGUI(); //new? shared ptr?
}

OptimizationController::~OptimizationController()
{
	EntityManagers.clear();
}

void OptimizationController::startSimulation(int numSteps, float tickDuration)
{
	m_env.run(numSteps, tickDuration);
}

void OptimizationController::resetSimulation()
{
	m_env.resetSimulation();
}
void OptimizationController::clearEntities(OptimizationEntityManager ToClear)
{
	//todo for a specific optimizationentitymanager, clear its entities

	ToClear.clearEntities();
}


void OptimizationController::LoadProjectile(Entity projectile)
{
	
	//the projectile entity should be taken, and stored as a parent for all new sims
	initialEntity = projectile;
}

void OptimizationController::LoadEntities(const std::vector<Entity>& entities, int iterations)
{
	for (int i = 0; i < iterations; i = i + 1)

	{
		auto entityManager = OptimizationEntityManager();
		for (const auto& entity : entities) {
			auto entityCopy = entity; // Make a copy to avoid modifying the original
			entityManager.addEntity(entityCopy);
		}
		EntityManagers.push_back(entityManager);
	}

    
}


void OptimizationController::LoadTarget(Vec3 targetPosition)
{
	targetPoint = targetPosition;
}

void OptimizationController::optimize(int numberOfSteps, float timestepSize)
{
	auto entityToOptimize = initialEntity;

	// create 3 entity managers, each with a different initial velocity for the projectile and all other entities the same
	// 
	// 
	// simulate each for a set number of steps
	// 
	// evaluate which got closest to the target point
	// 
	// repeat 
	Vec3 bestPosition = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < EntityManagers.size(); i = i+1)
	{
		entityToOptimize.getPhysicalState()->setVelocity({
			entityToOptimize.getPhysicalState()->getVelocity().x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f)) - 1.0f,
			entityToOptimize.getPhysicalState()->getVelocity().y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f)) - 1.0f,
			entityToOptimize.getPhysicalState()->getVelocity().z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f)) - 1.0f
			});
		EntityManagers[i].loadTargetPoint(targetPoint);
		EntityManagers[i].addTargetEntity(entityToOptimize);
		EntityManagers[i].run(numberOfSteps, timestepSize);
		bestPosition = EntityManagers[i].DetermineMinimumDistancePoint();
		
	}
	int bestIndex = 0;
	float closestMagnitude = std::numeric_limits<float>::max();
	std::cout << " ==== listing out magnitudes of optimized values====" << std::endl;
	for (int i = 0; i < EntityManagers.size(); i = i + 1)
	{
		std::cout << "magnitude " << EntityManagers[i].getShortestMagnitude() << std::endl;
		if (EntityManagers[i].getShortestMagnitude() > closestMagnitude)
		{
			closestMagnitude = EntityManagers[i].getShortestMagnitude();
			bestIndex = i;
		}
	}
	std::cout << "best option was " << EntityManagers[bestIndex].getShortestMagnitude() << std::endl;

	//bestEntity = EntityManagers[bestIndex].targetEntity;

    return ;
}




float OptimizationController::cleanFloat(std::string value)
{
	if (value.back() == 'f') {
		value.pop_back();
	}
	return std::strtof(value.c_str(), nullptr);
}