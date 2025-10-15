#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TrajectoryRenderer.h"
#include "controllers/SimulationControl.h"
#include <QQuickWidget>
#include <QUrl>
#include <QDebug>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    ui->quickWidget->setSource(QUrl("qrc:/sources/src/App/qml/main.qml"));
}

MainWindow::MainWindow(TrajectoryRenderer* trajectoryRenderer, SimulationController* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (trajectoryRenderer) {
        ui->quickWidget->rootContext()->setContextProperty("trajectoryRenderer", trajectoryRenderer);
    }

    if (controller) {
        ui->quickWidget->rootContext()->setContextProperty("simController", controller);
    }

    ui->quickWidget->setSource(QUrl("qrc:/sources/src/App/qml/main.qml"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QQmlContext* MainWindow::rootContext()
{
    return ui->quickWidget->rootContext();
}
