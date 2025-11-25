#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TrajectoryRenderer.h"
#include "controllers/SimulationControl.h"
#include "controllers/EntityListController.h"
#include "GravitiLib/EntityManager.h"
#include <QQuickWidget>
#include <QUrl>
#include <QDebug>
#include <QSizePolicy>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QQuickItem>

MainWindow::MainWindow(TrajectoryRenderer* trajectoryRenderer, SimulationController* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(controller)
    , m_renderer(trajectoryRenderer)
    , m_entityModel(nullptr)
    , m_updateScheduled(false)
{
    ui->setupUi(this);

    if (m_renderer) {
        ui->quickWidget->rootContext()->setContextProperty("trajectoryRenderer", m_renderer);
    }

    if (controller) {
        ui->quickWidget->rootContext()->setContextProperty("simController", controller);
    }
    ui->quickWidget->rootContext()->setContextProperty("followEntityName", QString(""));

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

    // Populate follow combo and connect selection
    updateFollowCombo();
    connect(ui->followCombo, &QComboBox::currentTextChanged, this, [this](const QString& text) {
        const QString value = (text == "None") ? QString("") : text;
        m_renderer->lockCameraEntity(value.toStdString());
        m_renderer->updateEntitySpheres();
    });
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
    //m_renderer->convertTrajectoriesToSpheres(sphereCount, 0.01f);
    //m_renderer->addEntityOrigins(0.2f);
    m_renderer->updateEntitySpheres();
}

void MainWindow::updateEntityList()
{
    if (!m_controller || !m_entityModel) {
        return;
    }
    EntityListController::updateEntityList(m_entityModel);
    updateFollowCombo();
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

    if (!m_isRunning) {
        // Get values from the line edit fields
        int numSteps = ui->lineEdit_10->text().toInt();
        float tickDuration = ui->lineEdit_9->text().toFloat();
        int spheresPerEntity = ui->lineEdit_11->text().toInt();
        int simScalar = ui->lineEdit_12->text().toInt();

        // Ensure that dependency injection update function is stored
        m_controller->setUpdateFunction([this, spheresPerEntity] {
            if (m_updateScheduled.load())
            {
                return;
            }
            m_updateScheduled.store(true);
            QMetaObject::invokeMethod(this, [this, spheresPerEntity] {
                updateRender(spheresPerEntity);
                updateEntityList();
                m_updateScheduled.store(false);
            }, Qt::QueuedConnection);
        });

        // Start (or resume) simulation
        m_isRunning = true;
        m_controller->startSimulation(numSteps, tickDuration, simScalar);
        ui->pushButton->setText("Pause Simulation");
    } else {
        // Pause simulation
        m_controller->pauseSimulation();
        m_isRunning = false;
        ui->pushButton->setText("Resume Simulation");
    }

}

void MainWindow::onResetSimulationClicked()
{
    if (!m_controller) {
        return;
    }
    m_controller->resetSimulation();
    updateRender();
    updateEntityList();
    m_isRunning = false;
    ui->pushButton->setText("Start Simulation");
}

void MainWindow::onClearEntitiesClicked()
{
    m_controller->clearEntities();
    if (m_renderer) {
        m_renderer->clearEntitySpheres();
    }
    onResetSimulationClicked();
    // Reset follow selection
    updateFollowCombo();
    // Do not change running state; just refresh
}

void MainWindow::updateFollowCombo()
{
    QString previous = ui->followCombo->currentText();
    ui->followCombo->clear();
    ui->followCombo->addItem("None");

    auto manager = EntityManager::getInstance();
    auto entities = manager->getAllEntities();
    if (entities) {
        for (auto& e : *entities) {
            ui->followCombo->addItem(QString::fromStdString(e.getEntityName()));
        }
    }

    int index = ui->followCombo->findText(previous);
    if (index >= 0 && previous != "None") {
        ui->followCombo->setCurrentIndex(index);
    }
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

void MainWindow::onAddOptimizedEntityClicked()
{
    if (!m_controller) {
        return;
    }

    // Get values from the form fields
    QString name = ui->optimizedName->text();
    if (name.isEmpty()) {
        // Could add error handling here
        return;
    }

    // Get position values
    float posX = ui->optimizedPosX->text().toFloat();
    float posY = ui->optimizedPosY->text().toFloat();
    float posZ = ui->optimizedPosZ->text().toFloat();

    // Get velocity values
    float velX = ui->optimizedVelX->text().toFloat();
    float velY = ui->optimizedVelY->text().toFloat();
    float velZ = ui->optimizedVelZ->text().toFloat();

	// get target position values
    float targetX = ui->optimizedTargetX->text().toFloat();
    float targetY = ui->optimizedTargetY->text().toFloat();
    float targetZ = ui->optimizedTargetZ->text().toFloat();

    // Get mass value
    
    float radius = ui->OptimizedRadius->text().toFloat();

    float mass = ui->optimizedMass->text().toFloat();


    float tick = ui->OptimizeTick->text().toFloat();

    float steps = ui->OptimizedSteps->text().toFloat();

    float numIter = ui->NumIter->text().toFloat();

    std::shared_ptr<IPhysicsEngine> physicsEngine = std::make_shared<NBodyPhysics>();

	Vec3 initialPosition = { posX, posY, posZ };
    Entity projectile(physicsEngine);
    projectile.setEntityName("ToOptimize");
    auto entityState = projectile.getPhysicalState();
    entityState->setPosition(X, posX);
    entityState->setPosition(Y, posY);
    entityState->setPosition(Z, posZ);
    entityState->setVelocity(X, velX);
    entityState->setVelocity(Y, velY);
    entityState->setVelocity(Z, velZ);
    entityState->setMass(mass);
    entityState->setRadius(radius);


    Vec3 targetPosition = { targetX, targetY, targetZ };
    int timeSteps = 5000;
    float timeStepSize = 0.1f;

    auto newEntity=m_controller->optimizeTrajectory(projectile, initialPosition, steps, tick, numIter);
    // Add the entity through the controller
    m_controller->createEntity(newEntity.getEntityName(), 
        newEntity.getPhysicalState()->getPosition().x, 
        newEntity.getPhysicalState()->getPosition().y, 
        newEntity.getPhysicalState()->getPosition().z, 
        newEntity.getPhysicalState()->getVelocity().x, 
        newEntity.getPhysicalState()->getVelocity().y, 
        newEntity.getPhysicalState()->getVelocity().z, 
        newEntity.getPhysicalState()->getMass());
    // Clear the form fields after adding
    ui->optimizedName->clear();
    ui->optimizedPosX->clear();
    ui->optimizedPosY->clear();
    ui->optimizedPosZ->clear();
    ui->optimizedVelX->clear();
    ui->optimizedVelY->clear();
    ui->optimizedVelZ->clear();
    ui->optimizedTargetX->clear();
    ui->optimizedTargetY->clear();
    ui->optimizedTargetZ->clear();
    ui->OptimizedRadius->clear();
    ui->optimizedMass->clear();
    ui->OptimizeTick->clear();
    ui->OptimizedSteps->clear();
    ui->NumIter->clear();
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