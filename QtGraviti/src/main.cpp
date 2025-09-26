//#include <QApplication>
//#include "src/App/mainwindow.h"
#include "GravitiLib/BatchSimEnvironment.h"
#include <thread>

int main(int argc, char* argv[])
{
    //std::thread t
    BatchSimEnvironment batch;
    batch.initialize();
    batch.run();

    //QApplication app(argc, argv);
    //MainWindow window;
    //window.show();
    return 1;
}