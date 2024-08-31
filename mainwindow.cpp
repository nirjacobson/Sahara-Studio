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

#ifdef Q_OS_WIN
    QSettings settings(Application::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(Application::Organization, Application::ApplicationName);
#endif

    _vulkan = settings.value("API").toString() == "Vulkan" && QVulkanInstance().create();
    ui->actionOpenGL->setChecked(!_vulkan);
    ui->actionVulkan->setChecked(_vulkan);

    if (_vulkan) {
        Sahara::VulkanSceneWidget* vkSceneWidget = new Sahara::VulkanSceneWidget(this);
        _primarySceneWidget = vkSceneWidget;
        ui->centralWidget->layout()->replaceWidget(ui->sceneWidget, vkSceneWidget);

        connect(vkSceneWidget, &Sahara::VulkanSceneWidget::initialized, this, &MainWindow::sceneWidgetInitialized);
        connect(vkSceneWidget, &Sahara::VulkanSceneWidget::sizeChanged, this, &MainWindow::sceneWidgetSizeChanged);
        connect(vkSceneWidget, &Sahara::VulkanSceneWidget::cameraMotion, this, &MainWindow::sceneWidgetCameraMotion);
        connect(vkSceneWidget, &Sahara::VulkanSceneWidget::sceneLoaded, this, &MainWindow::sceneLoaded);
    } else {
        Sahara::OpenGLSceneWidget* glSceneWidget = new Sahara::OpenGLSceneWidget(this);
        _primarySceneWidget = glSceneWidget;
        ui->centralWidget->layout()->replaceWidget(ui->sceneWidget, glSceneWidget);

        connect(glSceneWidget, &Sahara::OpenGLSceneWidget::initialized, this, &MainWindow::sceneWidgetInitialized);
        connect(glSceneWidget, &Sahara::OpenGLSceneWidget::sizeChanged, this, &MainWindow::sceneWidgetSizeChanged);
        connect(glSceneWidget, &Sahara::OpenGLSceneWidget::cameraMotion, this, &MainWindow::sceneWidgetCameraMotion);
        connect(glSceneWidget, &Sahara::OpenGLSceneWidget::sceneLoaded, this, &MainWindow::sceneLoaded);
    }

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
    connect(ui->actionOpenGL, &QAction::triggered, this, &MainWindow::openGLTriggered);
    connect(ui->actionVulkan, &QAction::triggered, this, &MainWindow::vulkanTriggered);

    ui->menuBar->setNativeMenuBar(false);
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
            _primarySceneWidget->pause();
            break;
        case QEvent::WindowUnblocked:
            _primarySceneWidget->resume();
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
    _primarySceneWidget->showGrid(true);

    _sceneGraphWidget = new SceneGraphWidget(this, *_primarySceneWidget);
    connect(_sceneGraphWidget, &SceneGraphWidget::selectionChanged, this, &MainWindow::sceneGraphWidgetSelectionChanged);
    ui->sceneGraphDockWidget->setWidget(_sceneGraphWidget);

    _toolsWidget = new ToolsWidget(this, _primarySceneWidget->scene());
    connect(_toolsWidget, &ToolsWidget::updatedScene, _sceneGraphWidget, &SceneGraphWidget::sceneUpdated);
    connect(dynamic_cast<QObject*>(_primarySceneWidget), SIGNAL(mouseMoved(const QVector2D&)), _toolsWidget, SLOT(mouseMoved(const QVector2D&)));
    connect(dynamic_cast<QObject*>(_primarySceneWidget), SIGNAL(mousePressed(const QVector2D&)), _toolsWidget, SLOT(mousePressed(const QVector2D&)));
    ui->toolsDockWidget->setWidget(_toolsWidget);

    _renderer = _primarySceneWidget->renderer();
}

void MainWindow::sceneWidgetSizeChanged(QSize)
{
    if (_selectedNode == &_primarySceneWidget->scene().cameraNode()) {
        _cameraWidget->updateFields();
    }
}

void MainWindow::sceneWidgetCameraMotion()
{
    Sahara::Node& cameraNode = _primarySceneWidget->scene().cameraNode();

    _app->undoStack().push(new TransformNodeCommand(this, &cameraNode, _primarySceneWidget->cameraControl().update(cameraNode)));
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
        if (node == &_primarySceneWidget->scene().root()) {
            _sceneWidget->setScene(&_primarySceneWidget->scene());
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
    _primarySceneWidget->newScene();
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

    Sahara::Scene* scene = Sahara::JSON::toScene(_primarySceneWidget->renderer(), QJsonDocument::fromJson(file.readAll()).object());
    _primarySceneWidget->setScene(scene);
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

    QJsonDocument saveDocument = QJsonDocument(Sahara::JSON::fromScene(&_primarySceneWidget->scene()));
    file.write(saveDocument.toJson(QJsonDocument::JsonFormat::Indented));
}

void MainWindow::sceneLoaded()
{
    _toolsWidget->setScene(_primarySceneWidget->scene());
    _sceneGraphWidget->reset();
    ui->nodeDockWidget->setWidget(nullptr);
    ui->nodeItemDockWidget->setWidget(nullptr);
}

void MainWindow::viewMenuAboutToShow()
{
    ui->actionFPS->setChecked(_primarySceneWidget->showFPS());
    ui->actionGrid->setChecked(_primarySceneWidget->showGrid());
    ui->actionAxes->setChecked(_primarySceneWidget->showGrid() && _primarySceneWidget->showAxes());
    ui->actionAxes->setEnabled(_primarySceneWidget->showGrid());
    ui->actionLights->setChecked(_primarySceneWidget->showLights());
    ui->actionCameras->setChecked(_primarySceneWidget->showCameras());
}

void MainWindow::fpsActionTriggered()
{
    _primarySceneWidget->showFPS(!_primarySceneWidget->showFPS());
}

void MainWindow::gridActionTriggered()
{
    _primarySceneWidget->showGrid(!_primarySceneWidget->showGrid());
}

void MainWindow::axesActionTriggered()
{
    _primarySceneWidget->showAxes(!_primarySceneWidget->showAxes());
}

void MainWindow::lightsActionTriggered()
{
    _primarySceneWidget->showLights(!_primarySceneWidget->showLights());
}

void MainWindow::camerasActionTriggered()
{
    _primarySceneWidget->showCameras(!_primarySceneWidget->showCameras());
}

void MainWindow::openGLTriggered()
{
    ui->actionOpenGL->setChecked(!_vulkan);
    ui->actionVulkan->setChecked(_vulkan);

#ifdef Q_OS_WIN
    QSettings settings(Application::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(Application::Organization, Application::ApplicationName);
#endif

    settings.setValue("API", "OpenGL");

    QMessageBox::information(this, "Please restart the program", "Please restart the program for the changes to take effect.");
}

void MainWindow::vulkanTriggered()
{
    ui->actionOpenGL->setChecked(!_vulkan);
    ui->actionVulkan->setChecked(_vulkan);

#ifdef Q_OS_WIN
    QSettings settings(Application::SettingsFile, QSettings::IniFormat);
#else
    QSettings settings(Application::Organization, Application::ApplicationName);
#endif

    settings.setValue("API", "Vulkan");

    QMessageBox::information(this, "Please restart the program", "Please restart the program for the changes to take effect.");
}

