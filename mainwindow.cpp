#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _toolsWidget(nullptr),
    _sceneGraphWidget(nullptr),
    _nodeDetailWidget(nullptr),
    _cameraWidget(nullptr),
    _pointLightWidget(nullptr),
    _modelWidget(nullptr)
{
    ui->setupUi(this);
    ui->nodeDockWidget->setWidget(nullptr);

    _nodeDetailWidget = new NodeDetailWidget;
    _cameraWidget = new CameraWidget;
    _pointLightWidget = new PointLightWidget;
    _modelWidget = new ModelWidget;

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

    connect(ui->menuView, &QMenu::aboutToShow, this, &MainWindow::viewMenuAboutToShow);
    connect(ui->actionFPS, &QAction::triggered, this, &MainWindow::fpsActionTriggered);
    connect(ui->actionGrid, &QAction::triggered, this, &MainWindow::gridActionTriggered);
    connect(ui->actionAxes, &QAction::triggered, this, &MainWindow::axesActionTriggered);
    connect(ui->actionLights, &QAction::triggered, this, &MainWindow::lightsActionTriggered);
    connect(ui->actionCameras, &QAction::triggered, this, &MainWindow::camerasActionTriggered);
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

    if (node) {
        _nodeDetailWidget->setNode(node);
        ui->nodeDockWidget->setWidget(&_nodeDetailWidgetScrollArea);

        Sahara::Camera* camera;
        Sahara::PointLight* pointLight;
        Sahara::Model* model;
        if ((camera = dynamic_cast<Sahara::Camera*>(&node->item()))) {
            _cameraWidget->setCamera(camera);
            ui->nodeItemDockWidget->setWidget(&_cameraWidgetScrollArea);
        } else if ((pointLight = dynamic_cast<Sahara::PointLight*>(&node->item()))) {
            _pointLightWidget->setPointLight(pointLight);
            ui->nodeItemDockWidget->setWidget(&_pointLightWidgetScrollArea);
        } else if ((model = dynamic_cast<Sahara::Model*>(&node->item()))) {
            _modelWidget->setModel(model);
            ui->nodeItemDockWidget->setWidget(_modelWidget);
        } else {
            ui->nodeItemDockWidget->setWidget(nullptr);
        }
    } else {
        ui->nodeDockWidget->setWidget(nullptr);
        ui->nodeItemDockWidget->setWidget(nullptr);
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
    _sceneGraphWidget->reset();
    ui->nodeDockWidget->setWidget(nullptr);
    ui->nodeItemDockWidget->setWidget(nullptr);
}

void MainWindow::viewMenuAboutToShow()
{
    ui->actionFPS->setChecked(ui->sceneWidget->showFPS());
    ui->actionGrid->setChecked(ui->sceneWidget->showGrid());
    ui->actionAxes->setChecked(ui->sceneWidget->showGrid() && ui->sceneWidget->showAxes());
    ui->actionAxes->setEnabled(ui->sceneWidget->showGrid());
    ui->actionLights->setChecked(ui->sceneWidget->showLights());
    ui->actionCameras->setChecked(ui->sceneWidget->showCameras());
}

void MainWindow::fpsActionTriggered()
{
    ui->sceneWidget->showFPS(!ui->sceneWidget->showFPS());
}

void MainWindow::gridActionTriggered()
{
    ui->sceneWidget->showGrid(!ui->sceneWidget->showGrid());
}

void MainWindow::axesActionTriggered()
{
    ui->sceneWidget->showAxes(!ui->sceneWidget->showAxes());
}

void MainWindow::lightsActionTriggered()
{
    ui->sceneWidget->showLights(!ui->sceneWidget->showLights());
}

void MainWindow::camerasActionTriggered()
{
    ui->sceneWidget->showCameras(!ui->sceneWidget->showCameras());
}

