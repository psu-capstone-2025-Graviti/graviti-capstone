#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQmlContext>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
class MainWindow;
}

class TrajectoryRenderer;
class SimulationController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(TrajectoryRenderer* trajectoryRenderer, SimulationController* controller, QWidget *parent = nullptr);
    ~MainWindow();

    QQmlContext* rootContext();

private slots:
    void onStartSimulationClicked();
    void onResetSimulationClicked();
    void onClearEntitiesClicked();
    void onAddEntityClicked();

private:
    void updateRender(int sphereCount);
    Ui::MainWindow *ui;
    SimulationController* m_controller;
    TrajectoryRenderer* m_renderer;
};

#endif // MAINWINDOW_H
