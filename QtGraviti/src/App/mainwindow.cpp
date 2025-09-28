#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TrajectoryRenderer.h"
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

MainWindow::MainWindow(TrajectoryRenderer* trajectoryRenderer, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (trajectoryRenderer) {
        ui->quickWidget->rootContext()->setContextProperty("trajectoryRenderer", trajectoryRenderer);
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
