#include "scenegraphwidget.h"
#include "ui_scenegraphwidget.h"

SceneGraphWidget::SceneGraphWidget(Sahara::SceneWidget& sceneWidget, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::SceneGraphWidget),
    _sceneWidget(sceneWidget),
    _selectedNode(nullptr)
{
    ui->setupUi(this);

    ui->eyeToolButton->setIcon(QIcon(":/icons/eye.png"));
    ui->flyThroughToolButton->setIcon(QIcon(":/icons/plane.png"));
    ui->eyeToolButton->setVisible(false);
    ui->flyThroughToolButton->setVisible(false);

    _treeModel = new SceneGraphItemModel(sceneWidget.scene());
    ui->sceneGraphTreeView->setModel(_treeModel);
    ui->sceneGraphTreeView->expandAll();

    connect(ui->sceneGraphTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SceneGraphWidget::treeViewSelectionChanged);
    connect(ui->addPushButton, &QPushButton::clicked, this, &SceneGraphWidget::addPushButtonClicked);
    connect(ui->sceneGraphTreeView, &SceneGraphTreeView::deleteKeyPressed, this, &SceneGraphWidget::deleteItemRequested);

    connect(ui->eyeToolButton, &QToolButton::clicked, this, &SceneGraphWidget::eyeButtonClicked);
    connect(ui->flyThroughToolButton, &QToolButton::clicked, this, &SceneGraphWidget::flyThroughButtonClicked);

    initAddMenu();
}

SceneGraphWidget::~SceneGraphWidget()
{
    delete ui;
}

void SceneGraphWidget::reload()
{
    SceneGraphItemModel* model = _treeModel;

    _treeModel = new SceneGraphItemModel(_sceneWidget.scene());
    ui->sceneGraphTreeView->setModel(_treeModel);
    ui->sceneGraphTreeView->expandAll();
    connect(ui->sceneGraphTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SceneGraphWidget::treeViewSelectionChanged);

    delete model;
}

void SceneGraphWidget::initAddMenu()
{
    QAction* cameraAction = _addMenu.addAction("Camera");

    QMenu* lightMenu = _addMenu.addMenu("Light");
    QAction* lightMenuPointAction = lightMenu->addAction("Point");

    QAction* modelAction = _addMenu.addAction("Model");

    connect(cameraAction, &QAction::triggered, this, &SceneGraphWidget::addCameraRequested);
    connect(lightMenuPointAction, &QAction::triggered, this, &SceneGraphWidget::addPointLightRequested);
    connect(modelAction, &QAction::triggered, this, &SceneGraphWidget::addModelRequested);
}

void SceneGraphWidget::sceneUpdated()
{
    QItemSelectionModel* selectionModel = ui->sceneGraphTreeView->selectionModel();

    if (!selectionModel->selectedIndexes().isEmpty()) {
        selectionModel->select(selectionModel->selectedIndexes().first(), QItemSelectionModel::Deselect);
    }

    selectionModel->select(_treeModel->focusIndex(), QItemSelectionModel::Select);
}

void SceneGraphWidget::treeViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    if (selected.indexes().isEmpty()) {
        _selectedNode = nullptr;

        if (!deselected.indexes().isEmpty()) {
            QModelIndex index = deselected.indexes().first();
            Sahara::Node* _deselectedNode = static_cast<Sahara::Node*>(index.internalPointer());
            _deselectedNode->setFocus(false);
        }

        emit selectionChanged(nullptr);
        return;
    }

    QModelIndex index = selected.indexes().first();
    _selectedNode = static_cast<Sahara::Node*>(index.internalPointer());
    _selectedNode->setFocus(true);

    ui->eyeToolButton->setVisible(dynamic_cast<Sahara::Camera*>(&_selectedNode->item()));
    ui->flyThroughToolButton->setVisible(dynamic_cast<Sahara::Camera*>(&_selectedNode->item()));

    emit selectionChanged(_selectedNode);
}

void SceneGraphWidget::addPushButtonClicked()
{
    _addMenu.exec(mapToGlobal(ui->addPushButton->pos()));
}

void SceneGraphWidget::addCameraRequested()
{
    QModelIndex index;
    if (ui->sceneGraphTreeView->selectionModel()->selectedIndexes().isEmpty()) {
        index = _treeModel->index(0, 0, QModelIndex());
    } else {
        index = ui->sceneGraphTreeView->selectionModel()->selectedIndexes().first();
    }

    Sahara::Camera* camera = new Sahara::Camera("Camera", Sahara::Camera::Mode::Perspective, 49, 7.31429f, 1.33f, 1, 100);
    _treeModel->addItem(index, "Camera", camera);

}

void SceneGraphWidget::addPointLightRequested()
{
    QModelIndex index;
    if (ui->sceneGraphTreeView->selectionModel()->selectedIndexes().isEmpty()) {
        index = _treeModel->index(0, 0, QModelIndex());
    } else {
        index = ui->sceneGraphTreeView->selectionModel()->selectedIndexes().first();
    }

    Sahara::PointLight* pointLight = new Sahara::PointLight("Light", QColor(255, 255, 255), 1, 0, 0.0016f);
    _treeModel->addItem(index, "Light", pointLight);
}

void SceneGraphWidget::addModelRequested()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Collada File"), "",
        tr("Collada document (*.dae);;All files (*)"));

    if (fileName.isEmpty())
        return;

    QModelIndex index;
    if (ui->sceneGraphTreeView->selectionModel()->selectedIndexes().isEmpty()) {
        index = _treeModel->index(0, 0, QModelIndex());
    } else {
        index = ui->sceneGraphTreeView->selectionModel()->selectedIndexes().first();
    }

    Sahara::Model* model = Sahara::Model::fromCollada(fileName);
    QString name = QFileInfo(fileName).baseName();
    name[0] = name.at(0).toUpper();
    _treeModel->addItem(index, name, model);
}

void SceneGraphWidget::deleteItemRequested(const QModelIndex& index)
{
    _treeModel->removeItem(index);
}

void SceneGraphWidget::eyeButtonClicked()
{
    _sceneWidget.scene().setCameraNode(_selectedNode);
}

void SceneGraphWidget::flyThroughButtonClicked()
{
    _sceneWidget.scene().setCameraNode(_selectedNode);
    _sceneWidget.scene().camera().setMode(Sahara::Camera::Mode::Perspective);

    _sceneWidget.setFocus();
    _sceneWidget.flyThrough(true);
}
