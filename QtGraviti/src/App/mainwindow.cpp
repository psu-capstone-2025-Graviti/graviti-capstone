#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TrajectoryRenderer.h"
#include "controllers/SimulationControl.h"
#include "controllers/EntityListController.h"
#include <QQuickWidget>
#include <QUrl>
#include <QDebug>
#include <QSizePolicy>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>

MainWindow::MainWindow(TrajectoryRenderer* trajectoryRenderer, SimulationController* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(controller)
    , m_renderer(trajectoryRenderer)
    , m_entityModel(nullptr)
{
    ui->setupUi(this);

    if (m_renderer) {
        ui->quickWidget->rootContext()->setContextProperty("trajectoryRenderer", m_renderer);
    }

    if (controller) {
        ui->quickWidget->rootContext()->setContextProperty("simController", controller);
    }

    //Set 3D view to the main qml file
    ui->quickWidget->setSource(QUrl("qrc:/sources/src/App/qml/main.qml"));

    //Connect buttons to actions
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onStartSimulationClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onResetSimulationClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onClearEntitiesClicked);
    connect(ui->AddEntity, &QPushButton::clicked, this, &MainWindow::onAddEntityClicked);
    connect(ui->actionSave_Entities, &QAction::triggered, this, &MainWindow::onSaveEntitiesClicked);
    connect(ui->actionLoad_Entities, &QAction::triggered, this, &MainWindow::onLoadEntitiesClicked);


    //Setup entity list model
    m_entityModel = new QStandardItemModel(this);
    ui->treeView->setModel(m_entityModel);
    ui->treeView->setHeaderHidden(false);

    //Call first update
    EntityListController::setupEntityList(m_entityModel);

    updateEntityList();
    updateRender();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QQmlContext* MainWindow::rootContext()
{
    return ui->quickWidget->rootContext();
}

void MainWindow::updateRender(int sphereCount)
{
    m_renderer->convertTrajectoriesToSpheres(sphereCount, 0.01f);
    m_renderer->addEntityOrigins(0.2f);
}

void MainWindow::updateEntityList()
{
    if (!m_controller || !m_entityModel) {
        return;
    }
    EntityListController::updateEntityList(m_entityModel);
}

void MainWindow::refreshEntityList()
{
    updateEntityList();
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
    updateEntityList();
}

void MainWindow::onClearEntitiesClicked()
{
    if (!m_controller) {
        return;
    }
    m_controller->clearEntities();
    updateRender();
    updateEntityList();
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
    updateEntityList();
}

void MainWindow::onSaveEntitiesClicked()
{
    if (!m_controller) {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save Entities", "", "JSON Files (*.json);;All Files (*)");

    if (!fileName.isEmpty()) {
        try {
            m_controller->saveEntitiesAsJson(fileName.toStdString());
            QMessageBox::information(this, "Success", "Entities saved successfully to " + fileName);
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", "Failed to save entities: " + QString::fromStdString(e.what()));
        }
    }
}

void MainWindow::onLoadEntitiesClicked()
{
    if (!m_controller) {
        QMessageBox::warning(this, "Error", "Simulation controller not available.");
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Load Entities", "", "JSON Files (*.json);;All Files (*)");

    if (!fileName.isEmpty()) {
        try {
            m_controller->clearEntities();
            m_controller->initialize_json_body(fileName.toStdString());
            updateEntityList();
            updateRender();
            QMessageBox::information(this, "Success", "Entities loaded successfully from " + fileName);
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", "Failed to load entities: " + QString::fromStdString(e.what()));
        }
    }
}