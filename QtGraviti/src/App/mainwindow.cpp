#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQuickWidget>
#include <QUrl>
#include <QDebug>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Configure the QQuickWidget for proper resizing
    ui->quickWidget->setResizeMode(QQuickWidget::SizeViewToRootObject);
    
    // Set the source to the QML file
    ui->quickWidget->setSource(QUrl("qrc:/sources/src/App/qml/main.qml"));
    
    // Ensure the QQuickWidget fills the available space
    ui->quickWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MainWindow::~MainWindow()
{
    delete ui;
}
