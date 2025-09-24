#include <QApplication>
#include "src/App/mainwindow.h"
#include "src/Simulation/BatchSimEnvironment.h"
#include <thread>

//#include "src/Simulation/ISimEnvironment.h"

int main(int argc, char* argv[])
{
    //std::thread t;

    BatchSimEnvironment batch;
    batch.initialize();
    batch.run();

    //QApplication app(argc, argv);
    //MainWindow window;
    //window.show();
    return 1;
}