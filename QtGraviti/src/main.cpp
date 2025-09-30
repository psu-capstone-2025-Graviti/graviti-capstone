#include <QApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include "App/mainwindow.h"

#include "GravitiLib/BatchSimEnvironment.h"
#include "App/TrajectoryRenderer.h"
#include "GravitiLib/EntityManager.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // TODO - we will probably have multiple qml types we need to register
    //Move this to its own file / class
    qmlRegisterType<TrajectoryRenderer>("GravitiLib", 1, 0, "TrajectoryRenderer");
    qmlRegisterType<TrajectorySphere>("GravitiLib", 1, 0, "TrajectorySphere");

    // Run batch simulation
    BatchSimEnvironment batch;
    batch.initialize_two_body();
    //batch.initialize_three_body();
    batch.run();

    // Create trajectory renderer and convert trajectories
    TrajectoryRenderer trajectoryRenderer;
    //change first number to see more spheres, change second number to change sphere scale
    trajectoryRenderer.convertTrajectoriesToSpheres(5, 0.1f);

    MainWindow window(&trajectoryRenderer);

    window.show();

    return app.exec();
}