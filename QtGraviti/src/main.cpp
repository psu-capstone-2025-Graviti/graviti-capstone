#include <QApplication>
#include "App/mainwindow.h"

#include "GravitiLib/BatchSimEnvironment.h"

int main(int argc, char* argv[])
{
    BatchSimEnvironment batch;
    batch.initialize();
    batch.run();

    QApplication app(argc, argv);
    MainWindow window;
    window.show();


    return app.exec();
}