#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQmlContext>
#include <QPushButton>
#include <QLineEdit>
#include <QStandardItemModel>
#include <atomic>

namespace Ui {
class MainWindow;
}

class TrajectoryRenderer;
class SimulationController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(TrajectoryRenderer* trajectoryRenderer, SimulationController* controller, QWidget *parent = nullptr);
    ~MainWindow();

    QQmlContext* rootContext();

private slots:
    void onStartSimulationClicked();
    void onResetSimulationClicked();
    void onClearEntitiesClicked();
    void onAddEntityClicked();
    void onSaveEntitiesClicked();
    void onLoadEntitiesClicked();

public:
    // Public method to update entity list from external calls
    void refreshEntityList();

private:
    void updateRender(int sphereCount = 1);
    void updateEntityList();
    void updateFollowCombo();
    Ui::MainWindow *ui;
    SimulationController* m_controller;
    TrajectoryRenderer* m_renderer;
    QStandardItemModel* m_entityModel;
    std::atomic<bool> m_updateScheduled;
    bool m_isRunning = false;
};

#endif // MAINWINDOW_H
