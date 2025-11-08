#include <QApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include "App/mainwindow.h"

#include "GravitiLib/BatchSimEnvironment.h"
#include "App/TrajectoryRenderer.h"
#include "GravitiLib/EntityManager.h"

#include "App/controllers/SimulationControl.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/NBodyEngine.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // TODO - we will probably have multiple qml types we need to register
    //Move this to its own file / class
    qmlRegisterType<TrajectoryRenderer>("GravitiLib", 1, 0, "TrajectoryRenderer");
    qmlRegisterType<TrajectorySphere>("GravitiLib", 1, 0, "TrajectorySphere");
    qmlRegisterType<EntitySphere>("GravitiLib", 1, 0, "EntitySphere");

    // Run batch simulation
    SimulationController controller;
    //controller.initializeThreeBody();
    controller.initialize_json_body("EntityJsons/test1.json");
    //controller.startSimulation(10000, 0.01);
    // csvs written to C:\Users\shado\source\repos\PSU-Graviti\QtGraviti\out\build\debug

    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();

    Entity test(physicsEngine);
    test.setEntityName("ToOptimize");
    auto entityState = test.getPhysicalState();
    entityState->setPosition(X, 1000);
    entityState->setPosition(Y, 0);
    entityState->setPosition(Z, 1000);
    entityState->setVelocity(X, 1);
    entityState->setVelocity(Y, 10);
    entityState->setVelocity(Z, 1);
    entityState->setMass(1);
    entityState->setRadius(0.2f);


    Vec3 targetPosition = { 100.0f, 100.0f, 100.0f };
    int timeSteps = 5000;
    float timeStepSize = 0.1f;
    controller.optimizeTrajectory(test, targetPosition, timeSteps, timeStepSize);


    // Create trajectory renderer and convert trajectories - Renderer is View
    //TrajectoryRenderer trajectoryRenderer;
    ////Entities are already in the env, so render them
    //trajectoryRenderer.addEntityOrigins(0.2f);

    //MainWindow window(&trajectoryRenderer, &controller);
    //window.show();
    //return app.exec();
}