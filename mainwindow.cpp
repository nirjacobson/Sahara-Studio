#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _toolsWidget(nullptr),
    _sceneGraphWidget(nullptr),
    _nodeDetailWidget(nullptr),
    _cameraWidget(nullptr),
    _pointLightWidget(nullptr)
{
    ui->setupUi(this);
    ui->nodeDockWidget->setWidget(nullptr);

    _nodeDetailWidget = new NodeDetailWidget;
    _cameraWidget = new CameraWidget;
    _pointLightWidget = new PointLightWidget;

    _nodeDetailWidgetScrollArea.setWidget(_nodeDetailWidget);
    _cameraWidgetScrollArea.setWidget(_cameraWidget);
    _pointLightWidgetScrollArea.setWidget(_pointLightWidget);

    connect(ui->sceneWidget, &Sahara::SceneWidget::initialized, this, &MainWindow::sceneWidgetInitialized);
    connect(ui->sceneWidget, &Sahara::SceneWidget::sizeChanged, this, &MainWindow::sceneWidgetSizeChanged);
    connect(ui->sceneWidget, &Sahara::SceneWidget::cameraMotion, this, &MainWindow::sceneWidgetCameraMotion);
    connect(ui->sceneWidget, &Sahara::SceneWidget::sceneLoaded, this, &MainWindow::sceneLoaded);

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newActionTriggered);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openActionTriggered);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAsActionTriggered);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _sceneGraphWidget;
}

void MainWindow::sceneWidgetInitialized()
{
    ui->sceneWidget->showGrid(true);

    _sceneGraphWidget = new SceneGraphWidget(*ui->sceneWidget);
    connect(_sceneGraphWidget, &SceneGraphWidget::selectionChanged, this, &MainWindow::sceneGraphWidgetSelectionChanged);
    ui->sceneGraphDockWidget->setWidget(_sceneGraphWidget);

    _toolsWidget = new ToolsWidget(ui->sceneWidget->scene());
    connect(_toolsWidget, &ToolsWidget::updatedNode, _nodeDetailWidget, &NodeDetailWidget::updateFields);
    connect(_toolsWidget, &ToolsWidget::updatedScene, _sceneGraphWidget, &SceneGraphWidget::sceneUpdated);
    connect(ui->sceneWidget, &Sahara::SceneWidget::mouseMoved, _toolsWidget, &ToolsWidget::mouseMoved);
    connect(ui->sceneWidget, &Sahara::SceneWidget::mousePressed, _toolsWidget, &ToolsWidget::mousePressed);
    ui->toolsDockWidget->setWidget(_toolsWidget);
}

void MainWindow::sceneWidgetSizeChanged(QSize size)
{
    ui->sceneWidget->scene().camera().setAspect(static_cast<float>(size.width()) / size.height());
    if (_selectedNode == &ui->sceneWidget->scene().cameraNode()) {
        _cameraWidget->updateFields();
    }
}

void MainWindow::sceneWidgetCameraMotion()
{
    if (_selectedNode == &ui->sceneWidget->scene().cameraNode()) {
        _nodeDetailWidget->updateFields();
    }
}

void MainWindow::sceneGraphWidgetSelectionChanged(Sahara::Node* node)
{
    _selectedNode = node;

   ui->nodeTabWidget->clear();

    if (node) {
        _nodeDetailWidget->setNode(node);
        ui->nodeTabWidget->insertTab(0, &_nodeDetailWidgetScrollArea, "Node");

        Sahara::Camera* camera;
        Sahara::PointLight* pointLight;
        if ((camera = dynamic_cast<Sahara::Camera*>(&node->item()))) {
            _cameraWidget->setCamera(camera);
            ui->nodeTabWidget->insertTab(1, &_cameraWidgetScrollArea, "Camera");
        } else if ((pointLight = dynamic_cast<Sahara::PointLight*>(&node->item()))) {
            _pointLightWidget->setPointLight(pointLight);
            ui->nodeTabWidget->insertTab(1, &_pointLightWidgetScrollArea, "Point Light");
        }
        ui->nodeDockWidget->setWidget(ui->nodeTabWidget);
    } else {
        ui->nodeDockWidget->setWidget(nullptr);
    }
}

void MainWindow::newActionTriggered()
{
    ui->sceneWidget->newScene();
}

void MainWindow::openActionTriggered()
{
    QString path = QFileDialog::getOpenFileName(this,
        tr("Save Scene"), "",
        tr("JSON documents (*.json);;All Files (*)"));

    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox;
        msgBox.setText("Could not open "+QFileInfo(file).fileName()+" for reading.");
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
        return;
    }

    Sahara::Scene* scene = Sahara::JSON::toScene(QJsonDocument::fromJson(file.readAll()).object());
    ui->sceneWidget->setScene(scene);
}

void MainWindow::saveAsActionTriggered()
{
    QString path = QFileDialog::getSaveFileName(this,
        tr("Save Scene"), "",
        tr("JSON documents (*.json);;All Files (*)"));

    if (path.isEmpty())
        return;


    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox msgBox;
        msgBox.setText("Could not open "+QFileInfo(file).fileName()+" for writing.");
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
        return;
    }

    QJsonDocument saveDocument = QJsonDocument(Sahara::JSON::fromScene(&ui->sceneWidget->scene()));
    file.write(saveDocument.toJson(QJsonDocument::JsonFormat::Indented));
}

void MainWindow::sceneLoaded()
{
    _toolsWidget->setScene(ui->sceneWidget->scene());
    _sceneGraphWidget->reload();
    ui->nodeDockWidget->setWidget(nullptr);
}

