#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQmlContext>

namespace Ui {
class MainWindow;
}

class sphereList;

class TrajectoryRenderer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(TrajectoryRenderer* trajectoryRenderer, QWidget *parent = nullptr);
    ~MainWindow();

    QQmlContext* rootContext();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
