#include <QApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include "App/mainwindow.h"

#include "GravitiLib/BatchSimEnvironment.h"
#include "App/TrajectoryRenderer.h"
#include "GravitiLib/EntityManager.h"

#include "App/controllers/SimulationControl.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // TODO - we will probably have multiple qml types we need to register
    //Move this to its own file / class
    qmlRegisterType<TrajectoryRenderer>("GravitiLib", 1, 0, "TrajectoryRenderer");
    qmlRegisterType<TrajectorySphere>("GravitiLib", 1, 0, "TrajectorySphere");
    qmlRegisterType<EntitySphere>("GravitiLib", 1, 0, "EntitySphere");
    //qmlRegisterType<TextureSphere>("GravitiLib", 1, 0, "TextureSphere");

    // Run batch simulation
    BatchSimEnvironment batch;
    SimulationController controller;
    //controller.initializeThreeBody();
    controller.loadJson("EntityJsons/test1.json");
    batch.run();

    // Create trajectory renderer and convert trajectories - Renderer is View
    TrajectoryRenderer trajectoryRenderer;

    //TODO - move these calls
    trajectoryRenderer.convertTrajectoriesToSpheres(50, 0.01f);
    trajectoryRenderer.addEntityOrigins(0.2f);

    MainWindow window(&trajectoryRenderer, &controller);
    window.show();
    return app.exec();
}