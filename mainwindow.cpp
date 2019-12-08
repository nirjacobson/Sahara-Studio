#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->sceneWidget, &Sahara::SceneWidget::initialized, this, &MainWindow::sceneWidgetInitialized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sceneWidgetInitialized()
{
    ui->sceneWidget->showGrid(true);
}
