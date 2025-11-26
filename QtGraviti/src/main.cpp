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
    qmlRegisterType<FlatEntity>("GravitiLib", 1, 0, "FlatEntity");

    // Run batch simulation
    SimulationController controller;
    //controller.initializeThreeBody();

    //controller.initialize_json_body("EntityJsons/LEO.json");

    //controller.startSimulation(10000, 0.01);
    // csvs written to C:\Users\shado\source\repos\PSU-Graviti\QtGraviti\out\build\debug

    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();

    /*Entity test(physicsEngine);
    test.setEntityName("ToOptimize");
    auto entityState = test.getPhysicalState();
    entityState->setPosition(X, 150);
    entityState->setPosition(Y, 150);
    entityState->setPosition(Z, 150);
    entityState->setVelocity(X, 1);
    entityState->setVelocity(Y, 10);
    entityState->setVelocity(Z, 1);
    entityState->setMass(1);
    entityState->setRadius(0.2f);


    Vec3 targetPosition = { 100.0f, 100.0f, 100.0f };
    int timeSteps = 5000;
    float timeStepSize = 0.1f;
    Entity OptEnt = controller.optimizeTrajectory(test, targetPosition, timeSteps, timeStepSize);
    std::cout << "Optimized Entity Position: (" 
              << OptEnt.getPhysicalState()->getPosition().x << ", "
              << OptEnt.getPhysicalState()->getPosition().y << ", "
              << OptEnt.getPhysicalState()->getPosition().z << ")\n";
    std::cout << "Optimized Entity velocity: ("
        << OptEnt.getPhysicalState()->getVelocity().x << ", "
        << OptEnt.getPhysicalState()->getVelocity().y << ", "
        << OptEnt.getPhysicalState()->getVelocity().z << ")\n";

    controller.createEntity("OptimizedEntity",
        OptEnt.getPhysicalState()->getPosition().x,
        OptEnt.getPhysicalState()->getPosition().y,
        OptEnt.getPhysicalState()->getPosition().z,
        OptEnt.getPhysicalState()->getVelocity().x,
        OptEnt.getPhysicalState()->getVelocity().y,
        OptEnt.getPhysicalState()->getVelocity().z,
        OptEnt.getPhysicalState()->getMass()
	);
    controller.createEntity("Target",
        100,
        100,
        100,
        0,
        0,
        0,
        0
    );*/

    // Create trajectory renderer and convert trajectories - Renderer is View
    TrajectoryRenderer trajectoryRenderer;
    //Entities are already in the env, so render them
    trajectoryRenderer.addEntityOrigins(0.2f);

    MainWindow window(&trajectoryRenderer, &controller);
    window.show();
    return app.exec();
}