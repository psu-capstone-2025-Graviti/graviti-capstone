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

void OptimizationController::LoadEntities(const std::vector<Entity>& entities)
{
	
		for (const auto& entity : entities) {
			auto entityCopy = entity; // Make a copy to avoid modifying the original
			SurroundingBodies.push_back(entityCopy);
		}

    
}


void OptimizationController::LoadTarget(Vec3 targetPosition)
{
	targetPoint = targetPosition;
}


std::vector<Vec3> OptimizationController::GenerateDefaultAxes(Entity DefaultEntity)
{
	Vec3 defaultVelVector = DefaultEntity.getPhysicalState()->getVelocity();
	std::vector<Vec3> AxesVectors;
	AxesVectors.push_back(defaultVelVector);
	AxesVectors.push_back({ -1 * defaultVelVector.x,-1 * defaultVelVector.y,-1 * defaultVelVector.z });

	Vec3 rotateYVector = {
		defaultVelVector.x * cos(PI / 2) + defaultVelVector.z * sin(PI / 2),
		defaultVelVector.y ,
		-defaultVelVector.x * sin(PI / 2) + defaultVelVector.z * cos(PI / 2) };
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
		defaultVelVector.y * cos(PI / 2) - defaultVelVector.z * sin(PI / 2),
		defaultVelVector.y * sin(PI / 2) + defaultVelVector.z * cos(PI / 2)
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

void OptimizationController::optimize(int numberOfSteps, float timestepSize, int numberOfIterations)
{
	// get initial velocity vector
	Vec3 x = initialEntity.getPhysicalState()->getVelocity();

	// generate default axes from initial velocity vector
	std::vector<Vec3> axes = GenerateDefaultAxes(initialEntity);

	// create new entity managers based on axes, and populate with existing entities too
	for (const auto& axis : axes)
	{
		auto entityManager = OptimizationEntityManager();
		auto entityCopy = initialEntity; // Make a copy to avoid modifying the original
		entityCopy.getPhysicalState()->setVelocity(axis);
		entityManager.loadTargetPoint(targetPoint);

		entityManager.addTargetEntity(entityCopy);
		for (Entity body : SurroundingBodies)
		{
			entityManager.addEntity(body);
		}
		EntityManagers.push_back(entityManager);
	}

	// run EntityManagers and evaluate best 3 results
	double closestMagnitude = 999999999999999999;
	Vec3 closestVelocity= { 0.0f, 0.0f, 0.0f };
	double secondClosestMagnitude = 999999999999999999;
	Vec3 secondclosestVelocity = { 0.0f, 0.0f, 0.0f };
	double thirdClosestMagnitude = 999999999999999999;
	Vec3 thirdclosestVelocity = { 0.0f, 0.0f, 0.0f };

	for (int i = 0; i<EntityManagers.size();i++)
	{
		EntityManagers.at(i).run(numberOfSteps, timestepSize);

		auto bestPosition=EntityManagers.at(i).DetermineMinimumDistancePoint();
		if (optimizationLogging == true)
		{
			std::cout << " listing best position " <<
				bestPosition.x << ", " <<
				bestPosition.y << ", " <<
				bestPosition.z <<
				std::endl;
		}
		double currentMagnitude = EntityManagers.at(i).getShortestMagnitude();
		std::cout << " magnitude is " <<currentMagnitude << std::endl;

		Vec3 currentVelocity = EntityManagers.at(i).targetEntity.getPhysicalState()->getVelocity();
		if (currentMagnitude < closestMagnitude)
		{
			thirdClosestMagnitude = secondClosestMagnitude;
			thirdclosestVelocity = secondclosestVelocity;
			secondClosestMagnitude = closestMagnitude;
			secondclosestVelocity = closestVelocity;
			closestMagnitude = currentMagnitude;
			closestVelocity = currentVelocity;

		}
		else if (currentMagnitude < secondClosestMagnitude)
		{
			thirdClosestMagnitude = secondClosestMagnitude;
			thirdclosestVelocity = secondclosestVelocity;
			secondClosestMagnitude = currentMagnitude;
			secondclosestVelocity = currentVelocity;

		}
		else if (currentMagnitude < thirdClosestMagnitude)
		{
			thirdClosestMagnitude = currentMagnitude;
			thirdclosestVelocity = currentVelocity;

		}


	}

	if (optimizationLogging == true)
	{
		std::cout << " listing best 3 magnitudes" <<
			closestMagnitude << ", " <<
			secondClosestMagnitude << ", " <<
			thirdClosestMagnitude <<
			std::endl;
		std::cout << " listing best velocity " <<
			closestVelocity.x << ", " <<
			closestVelocity.y << ", " <<
			closestVelocity.z <<
			std::endl;
		std::cout << " listing secondbest velocity " <<
			secondclosestVelocity.x << ", " <<
			secondclosestVelocity.y << ", " <<
			secondclosestVelocity.z <<
			std::endl;
		std::cout << " listing thirdbest velocity " <<
			thirdclosestVelocity.x << ", " <<
			thirdclosestVelocity.y << ", " <<
			thirdclosestVelocity.z <<
			std::endl;
	}
	initialEntity.getPhysicalState()->setVelocity(closestVelocity);
	bestEntity = initialEntity;
	if (totaliterations == 1)
	{
		return;
	}
	else
	{
		std::cout << "more iterations!" << std::endl;
	}

	return;
}


float OptimizationController::cleanFloat(std::string value)
{
	if (value.back() == 'f') {
		value.pop_back();
	}
	return std::strtof(value.c_str(), nullptr);
}