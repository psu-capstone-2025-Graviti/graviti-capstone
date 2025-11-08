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
int totaliterations = 3;

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

	auto entityManager = OptimizationEntityManager();
	for (const auto& entity : entities) {
		auto entityCopy = entity; // Make a copy to avoid modifying the original
		entityManager.addEntity(entityCopy);
	}
	EntityManagers.push_back(entityManager);

	totaliterations = iterations;


    
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

double OptimizationController::vectorMagnitude(Vec3 vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

std::vector<Vec3> OptimizationController::TriangulationVectors(Vec3 Best, Vec3 SecondBest, Vec3 ThirdBest)
{
	std::vector<Vec3> TriangulatedVectors;
	TriangulatedVectors.push_back(Best);
	
	Vec3 bsb = {
		(Best.x + SecondBest.x) ,
		(Best.y + SecondBest.y) ,
		(Best.z + SecondBest.z) 
	};
	
	Vec3 SBNormalized = {
		(bsb.x) * vectorMagnitude(Best) / (vectorMagnitude(bsb)),
		(bsb.y) * vectorMagnitude(Best) / (vectorMagnitude(bsb)),
		(bsb.z) * vectorMagnitude(Best) / (vectorMagnitude(bsb))
	};

	TriangulatedVectors.push_back(SBNormalized);

	Vec3 tsb = {
		(Best.x + ThirdBest.x) ,
		(Best.y + ThirdBest.y) ,
		(Best.z + ThirdBest.z)
	};


	Vec3 TBNormalized = {
		(tsb.x) * vectorMagnitude(Best) / (vectorMagnitude(tsb)),
		(tsb.y) * vectorMagnitude(Best) / (vectorMagnitude(tsb)),
		(tsb.z) * vectorMagnitude(Best) / (vectorMagnitude(tsb))
	};

	TriangulatedVectors.push_back(TBNormalized);

	return TriangulatedVectors;



}



void OptimizationController::exampleoptimize(int numberOfSteps, float timestepSize)
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
		EntityManagers.at(i).loadTargetPoint(targetPoint);
		EntityManagers.at(i).addTargetEntity(entityToOptimize);
		EntityManagers.at(i).run(numberOfSteps, timestepSize);
		bestPosition = EntityManagers.at(i).DetermineMinimumDistancePoint();
		std::cout << " listing best position " << bestPosition.x << bestPosition.y << bestPosition.z << std::endl;

		
	}
	int bestIndex = 0;
	float closestMagnitude = std::numeric_limits<float>::max();
	std::cout << " ==== listing out magnitudes of optimized values====" << std::endl;
	for (int i = 0; i < EntityManagers.size(); i = i + 1)
	{
		std::cout << "magnitude " << EntityManagers.at(i).getShortestMagnitude() << std::endl;
		if (EntityManagers.at(i).getShortestMagnitude() > closestMagnitude)
		{
			closestMagnitude = EntityManagers.at(i).getShortestMagnitude();
			bestIndex = i;
		}
	}
	std::cout << "best option was " << EntityManagers[bestIndex].getShortestMagnitude() << std::endl;

	//bestEntity = EntityManagers[bestIndex].targetEntity;

    return ;
}

void OptimizationController::optimize(int numberOfSteps, float timestepSize)
{
	auto entityToOptimize = initialEntity;
	std::vector<Vec3> defaultAxes = GenerateDefaultAxes(entityToOptimize);
	
	// initialize first 6 cases, 
	for (const auto& axis : defaultAxes)
	{
		auto entityCopy = entityToOptimize; // Make a copy to avoid modifying the original
		entityCopy.getPhysicalState()->setVelocity(axis);
		auto entityManager = OptimizationEntityManager();
		entityManager.loadTargetPoint(targetPoint);

		//eventually need to add load all entities as well
		entityManager.addTargetEntity(entityCopy);
		EntityManagers.push_back(entityManager);
	}
	// run all 6 cases
	for (int i = 0; i < EntityManagers.size(); i = i + 1)
	{
		EntityManagers.at(i).run(numberOfSteps, timestepSize);
		auto bestPosition = EntityManagers.at(i).DetermineMinimumDistancePoint();
		std::cout << " listing best position " << bestPosition.x << "," << bestPosition.y << "," << bestPosition.z << std::endl;
	}

	// for all managers, find best 3, delete all cases except best 3
	for (int j = 0; j < totaliterations; j = j + 1)
	{
		std::cout << " ==== Optimization Iteration " << j << "====" << std::endl;

		// generate default axes for entity manager and add each to a new optimization entity manager
		// solve all 6, find the best 3
		// triangulate new vectors from best 3
		
		//find best 3
		int bestIndex = -1;
		float closestMagnitude = std::numeric_limits<float>::max();

		// find best vector
		std::vector<Vec3> bestVectors;
		for (int i = 0; i < EntityManagers.size(); i = i + 1)
		{
			if ((EntityManagers.at(i).getShortestMagnitude() < closestMagnitude) && (EntityManagers.at(i).getShortestMagnitude() != 0))
			{
				closestMagnitude = EntityManagers.at(i).getShortestMagnitude();
				bestIndex = i;
			}
		}

		Vec3 bestVelocity = EntityManagers[bestIndex].targetEntity.getPhysicalState()->getVelocity();
		//EntityManagers.erase(EntityManagers.begin() + bestIndex);
		//delete best from list so we can find second best

		//store best vector in new list
		bestVectors.push_back(bestVelocity);
		closestMagnitude = std::numeric_limits<float>::max();
		for (int i = 0; i < EntityManagers.size(); i = i + 1)
		{
			if (i != bestIndex && (EntityManagers.at(i).getShortestMagnitude() < closestMagnitude) && (EntityManagers.at(i).getShortestMagnitude() != 0))
			{
				closestMagnitude = EntityManagers.at(i).getShortestMagnitude();
				bestIndex = i;
			}
		}
		Vec3 secondBest = EntityManagers[bestIndex].targetEntity.getPhysicalState()->getVelocity();
		//EntityManagers.erase(EntityManagers.begin() + bestIndex);
		//delete second best from list so we can find third best

		//store second best vector in new list
		bestVectors.push_back(secondBest);
		closestMagnitude = std::numeric_limits<float>::max();
		for (int i = 0; i < EntityManagers.size(); i = i + 1)
		{
			if ((EntityManagers.at(i).getShortestMagnitude() < closestMagnitude) && (EntityManagers.at(i).getShortestMagnitude() != 0))
			{
				closestMagnitude = EntityManagers.at(i).getShortestMagnitude();
				bestIndex = i;
			}
		}
		Vec3 thirdBest = EntityManagers[bestIndex].targetEntity.getPhysicalState()->getVelocity();
		//EntityManagers.erase(EntityManagers.begin() + bestIndex);
		//delete second best from list so we can find third best

		//store second best vector in new list
		bestVectors.push_back(thirdBest);

		std::cout << " best 3 indices "
			<< bestVectors.at(0).x << ", "
			<< bestVectors.at(0).y << ", "
			<< bestVectors.at(0).z << std::endl;

		std::cout << " best 3 indices "
			<< bestVectors.at(1).x << ", "
			<< bestVectors.at(1).y << ", "
			<< bestVectors.at(1).z << std::endl;

		std::cout << " best 3 indices "
			<< bestVectors.at(2).x << ", "
			<< bestVectors.at(2).y << ", "
			<< bestVectors.at(2).z << std::endl;
		
		//clear entity managers and reinitialize with triangulated vectors
		//EntityManagers.clear();

		//for (int i = 0; i < 3; i = i + 1)
		//{
		//	auto entityCopy = entityToOptimize; // Make a copy to avoid modifying the original
		//	entityCopy.getPhysicalState()->setVelocity(bestVectors.at(i));
		//	auto entityManager = OptimizationEntityManager();
		//	entityManager.loadTargetPoint(targetPoint);
		//	entityManager.addTargetEntity(entityCopy);
		//	EntityManagers.push_back(entityManager);
		//}
		
	}

	
}


float OptimizationController::cleanFloat(std::string value)
{
	if (value.back() == 'f') {
		value.pop_back();
	}
	return std::strtof(value.c_str(), nullptr);
}