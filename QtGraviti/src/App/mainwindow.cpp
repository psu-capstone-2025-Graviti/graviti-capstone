#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TrajectoryRenderer.h"
#include "controllers/SimulationControl.h"
#include <QQuickWidget>
#include <QUrl>
#include <QDebug>
#include <QSizePolicy>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(nullptr)
{
    ui->setupUi(this);
    
    ui->quickWidget->setSource(QUrl("qrc:/sources/src/App/qml/main.qml"));
}

MainWindow::MainWindow(TrajectoryRenderer* trajectoryRenderer, SimulationController* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(controller)
    , m_renderer(trajectoryRenderer)
{
    ui->setupUi(this);

    if (m_renderer) {
        ui->quickWidget->rootContext()->setContextProperty("trajectoryRenderer", m_renderer);
    }

    if (controller) {
        ui->quickWidget->rootContext()->setContextProperty("simController", controller);
    }

    // Connect the Start Simulation button
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onStartSimulationClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onResetSimulationClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onClearEntitiesClicked);
    connect(ui->AddEntity, &QPushButton::clicked, this, &MainWindow::onAddEntityClicked);

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

void MainWindow::updateRender(int sphereCount = 1)
{
    m_renderer->convertTrajectoriesToSpheres(sphereCount, 0.01f);
    m_renderer->addEntityOrigins(0.2f);
}

void MainWindow::onStartSimulationClicked()
{
    if (!m_controller) {
        return;
    }

    // Get values from the line edit fields
    int numSteps = ui->lineEdit_10->text().toInt();
    float tickDuration = ui->lineEdit_9->text().toFloat();
    int spheresPerEntity = ui->lineEdit_11->text().toInt();

    // Call the simulation with the parameters
    m_controller->startSimulation(numSteps, tickDuration);
    updateRender(spheresPerEntity);

}

void MainWindow::onResetSimulationClicked()
{
    if (!m_controller) {
        return;
    }
    m_controller->resetSimulation();
    updateRender();
}

void MainWindow::onClearEntitiesClicked()
{
    if (!m_controller) {
        return;
    }
    m_controller->clearEntities();
    updateRender();
}

void MainWindow::onAddEntityClicked()
{
    if (!m_controller) {
        return;
    }

    // Get values from the form fields
    QString name = ui->lineEdit->text();
    if (name.isEmpty()) {
        // Could add error handling here
        return;
    }

    // Get position values
    float posX = ui->lineEdit_5->text().toFloat();
    float posY = ui->lineEdit_4->text().toFloat();
    float posZ = ui->lineEdit_6->text().toFloat();

    // Get velocity values
    float velX = ui->lineEdit_7->text().toFloat();
    float velY = ui->lineEdit_3->text().toFloat();
    float velZ = ui->lineEdit_2->text().toFloat();

    // Get mass value
    float mass = ui->lineEdit_8->text().toFloat();

    // Add the entity through the controller
    m_controller->createEntity(name.toStdString(), posX, posY, posZ, velX, velY, velZ, mass);

    // Clear the form fields after adding
    ui->lineEdit->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_8->clear();
    updateRender();
}