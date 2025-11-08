#include "SimulationControl.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"
#include "GravitiLib/OptimizationEntityManager.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "OptimizationController.h"
#include <cmath>

const double PI = 3.141592653589793;

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


std::vector<Vec3> OptimizationController::GenerateDefaultAxes(Entity DefaultEntity) 
{
	Vec3 defaultVelVector=DefaultEntity.getPhysicalState()->getVelocity();
	std::vector<Vec3> AxesVectors;
	AxesVectors.push_back(defaultVelVector);
	AxesVectors.push_back({ -1*defaultVelVector.x,-1 * defaultVelVector.y,-1 * defaultVelVector.z });

	Vec3 rotateYVector = { 
		defaultVelVector.x* cos(PI/2)+ defaultVelVector.z*sin(PI/2),
		defaultVelVector.y ,
		-defaultVelVector.x*sin(PI/2)+defaultVelVector.z*cos(PI/2)};
	AxesVectors.push_back(rotateYVector);
	AxesVectors.push_back(
		{
		-1 * rotateYVector.x,
		-1 * rotateYVector.y,
		-1 * rotateYVector.z 
		});

	Vec3 rotateZVector = 
	{ 
		defaultVelVector.x, 
		defaultVelVector.y*cos(PI/2)- defaultVelVector.z*sin(PI/2),
		defaultVelVector.y*sin(PI/2)+ defaultVelVector.z*cos(PI/2)
	};
	AxesVectors.push_back(rotateZVector);
	AxesVectors.push_back(
		{	
		-1 * rotateZVector.x,
		-1 * rotateZVector.y,
		-1 * rotateZVector.z 
		});

	return AxesVectors;

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
			entityToOptimize.getPhysicalState()->getVelocity().x + i*5,
			entityToOptimize.getPhysicalState()->getVelocity().y + i*5,
			entityToOptimize.getPhysicalState()->getVelocity().z + i*5
			});
		EntityManagers[i].loadTargetPoint(targetPoint);
		EntityManagers[i].addTargetEntity(entityToOptimize);
		EntityManagers[i].run(numberOfSteps, timestepSize);
		bestPosition = EntityManagers[i].DetermineMinimumDistancePoint();
		std::cout << " listing best position " << bestPosition.x << bestPosition.y << bestPosition.z << std::endl;

		
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

void OptimizationController::optimize(int numberOfSteps, float timestepSize)
{
	auto x = initialEntity.getPhysicalState()->getVelocity();
}


float OptimizationController::cleanFloat(std::string value)
{
	if (value.back() == 'f') {
		value.pop_back();
	}
	return std::strtof(value.c_str(), nullptr);
}