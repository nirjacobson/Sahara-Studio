#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Application* application) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _toolsWidget(nullptr),
    _sceneGraphWidget(nullptr),
    _nodeDetailWidget(nullptr),
    _cameraWidget(nullptr),
    _pointLightWidget(nullptr),
    _modelWidget(nullptr),
    _app(application)
{
    ui->setupUi(this);

    QAction* undoAction = _app->undoStack().createUndoAction(this, "&Undo");
    undoAction->setShortcuts(QKeySequence::Undo);
    ui->menuEdit->addAction(undoAction);

    QAction* redoAction = _app->undoStack().createRedoAction(this, "&Redo");
    redoAction->setShortcuts(QKeySequence::Redo);
    ui->menuEdit->addAction(redoAction);

    ui->actionNew->setIcon(QIcon::fromTheme("document-new"));
    ui->actionOpen->setIcon(QIcon::fromTheme("document-open"));
    ui->actionSaveAs->setIcon(QIcon::fromTheme("document-save-as"));
    ui->actionExit->setIcon(QIcon::fromTheme("application-exit"));

    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSaveAs->setShortcut(QKeySequence::SaveAs);
    ui->actionExit->setShortcut(QKeySequence::Quit);

    ui->actionFPS->setIcon(QIcon(":/icons/counter.png"));
    ui->actionGrid->setIcon(QIcon(":/icons/grid.png"));
    ui->actionAxes->setIcon(QIcon(":/icons/axes.png"));
    ui->actionLights->setIcon(QIcon(":/icons/light.png"));
    ui->actionCameras->setIcon(QIcon(":/icons/camera.png"));

    ui->actionFPS->setShortcut(QKeySequence(Qt::ALT | Qt::Key_P));
    ui->actionGrid->setShortcut(QKeySequence(Qt::ALT | Qt::Key_G));
    ui->actionAxes->setShortcut(QKeySequence(Qt::ALT | Qt::Key_A));
    ui->actionLights->setShortcut(QKeySequence(Qt::ALT | Qt::Key_L));
    ui->actionCameras->setShortcut(QKeySequence(Qt::ALT | Qt::Key_C));

    ui->nodeDockWidget->setWidget(nullptr);

    _nodeDetailWidget = new NodeDetailWidget(this);
    _cameraWidget = new CameraWidget(this);
    _pointLightWidget = new PointLightWidget(this);
    _modelWidget = new ModelWidget(this);
    _sceneWidget = new SceneWidget(this);

    _nodeDetailWidgetScrollArea.setWidget(_nodeDetailWidget);
    _cameraWidgetScrollArea.setWidget(_cameraWidget);
    _pointLightWidgetScrollArea.setWidget(_pointLightWidget);
    _sceneWidgetScrollArea.setWidget(_sceneWidget);

    connect(ui->sceneWidget, &Sahara::SceneWidget::initialized, this, &MainWindow::sceneWidgetInitialized);
    connect(ui->sceneWidget, &Sahara::SceneWidget::sizeChanged, this, &MainWindow::sceneWidgetSizeChanged);
    connect(ui->sceneWidget, &Sahara::SceneWidget::cameraMotion, this, &MainWindow::sceneWidgetCameraMotion);
    connect(ui->sceneWidget, &Sahara::SceneWidget::sceneLoaded, this, &MainWindow::sceneLoaded);

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newActionTriggered);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openActionTriggered);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAsActionTriggered);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

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

bool MainWindow::event(QEvent* event)
{
    switch (event->type()) {
        case QEvent::WindowBlocked:
            ui->sceneWidget->pause();
            break;
        case QEvent::WindowUnblocked:
            ui->sceneWidget->resume();
            break;
        default:
            break;
    }

        return QMainWindow::event(event);
}

void MainWindow::updateNodeFields()
{   _nodeDetailWidget->updateFields();

}

void MainWindow::updateCameraFields()
{
        _cameraWidget->updateFields();
}

void MainWindow::updateModelFields()
{   _modelWidget->updateFields();

}

void MainWindow::updatePointLightFields()
{
    _pointLightWidget->updateFields();
}

void MainWindow::updateSceneFields()
{
        _sceneWidget->updateFields();
}

Application* MainWindow::app() {
        return _app;
}

Sahara::Renderer* MainWindow::renderer()
{
    return _renderer;
}

void MainWindow::sceneWidgetInitialized()
{
    ui->sceneWidget->showGrid(true);

    _sceneGraphWidget = new SceneGraphWidget(this, *ui->sceneWidget);
    connect(_sceneGraphWidget, &SceneGraphWidget::selectionChanged, this, &MainWindow::sceneGraphWidgetSelectionChanged);
    ui->sceneGraphDockWidget->setWidget(_sceneGraphWidget);

    _toolsWidget = new ToolsWidget(this, ui->sceneWidget->scene());
    connect(_toolsWidget, &ToolsWidget::updatedScene, _sceneGraphWidget, &SceneGraphWidget::sceneUpdated);
    connect(ui->sceneWidget, &Sahara::SceneWidget::mouseMoved, _toolsWidget, &ToolsWidget::mouseMoved);
    connect(ui->sceneWidget, &Sahara::SceneWidget::mousePressed, _toolsWidget, &ToolsWidget::mousePressed);
    ui->toolsDockWidget->setWidget(_toolsWidget);

    _renderer = ui->sceneWidget->renderer();
}

void MainWindow::sceneWidgetSizeChanged(QSize)
{
    if (_selectedNode == &ui->sceneWidget->scene().cameraNode()) {
        _cameraWidget->updateFields();
    }
}

void MainWindow::sceneWidgetCameraMotion()
{
    Sahara::Node& cameraNode = ui->sceneWidget->scene().cameraNode();

    _app->undoStack().push(new TransformNodeCommand(this, &cameraNode, ui->sceneWidget->cameraControl().update(cameraNode)));
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
        if (node == &ui->sceneWidget->scene().root()) {
            _sceneWidget->setScene(&ui->sceneWidget->scene());
            ui->nodeItemDockWidget->setWidget(&_sceneWidgetScrollArea);
        } else if ((camera = dynamic_cast<Sahara::Camera*>(&node->item()))) {
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

    Sahara::Scene* scene = Sahara::JSON::toScene(ui->sceneWidget->renderer(), QJsonDocument::fromJson(file.readAll()).object());
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

