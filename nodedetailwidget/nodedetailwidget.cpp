#include "nodedetailwidget.h"
#include "ui_nodedetailwidget.h"
#include "mainwindow.h"

NodeDetailWidget::NodeDetailWidget(MainWindow* window, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodeDetailWidget),
    _window(window),
    _node(nullptr),
    _lockScale(true)
{
    ui->setupUi(this);

    ui->lockScaleButton->setIcon(QIcon(":/icons/lock.png"));

    connect(ui->lockScaleButton, &QPushButton::clicked, this, &NodeDetailWidget::lockScaleButtonClicked);

    connect(ui->locationXDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(loctnXDoubleSpinBoxValueChanged(const double)));
    connect(ui->locationYDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(loctnYDoubleSpinBoxValueChanged(const double)));
    connect(ui->locationZDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(loctnZDoubleSpinBoxValueChanged(const double)));
    connect(ui->rotationXDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(rottnXDoubleSpinBoxValueChanged(const double)));
    connect(ui->rotationYDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(rottnYDoubleSpinBoxValueChanged(const double)));
    connect(ui->rotationZDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(rottnZDoubleSpinBoxValueChanged(const double)));
    connect(ui->scaleXDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(scaleXDoubleSpinBoxValueChanged(const double)));
    connect(ui->scaleYDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(scaleYDoubleSpinBoxValueChanged(const double)));
    connect(ui->scaleZDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(scaleZDoubleSpinBoxValueChanged(const double)));
}

NodeDetailWidget::~NodeDetailWidget()
{
    delete ui;
}

void NodeDetailWidget::setNode(Sahara::Node* node)
{
    _node = node;

    populateFieldsFromNode();
}

void NodeDetailWidget::updateFields()
{
    populateFieldsFromNode();
}

void NodeDetailWidget::populateFieldsFromNode()
{
    ui->locationXDoubleSpinBox->blockSignals(true);
    ui->locationYDoubleSpinBox->blockSignals(true);
    ui->locationZDoubleSpinBox->blockSignals(true);
    ui->locationXDoubleSpinBox->setValue(_node->location().x());
    ui->locationYDoubleSpinBox->setValue(_node->location().y());
    ui->locationZDoubleSpinBox->setValue(_node->location().z());
    ui->locationXDoubleSpinBox->blockSignals(false);
    ui->locationYDoubleSpinBox->blockSignals(false);
    ui->locationZDoubleSpinBox->blockSignals(false);

    ui->rotationXDoubleSpinBox->blockSignals(true);
    ui->rotationYDoubleSpinBox->blockSignals(true);
    ui->rotationZDoubleSpinBox->blockSignals(true);
    ui->rotationXDoubleSpinBox->setValue(_node->rotation().x());
    ui->rotationYDoubleSpinBox->setValue(_node->rotation().y());
    ui->rotationZDoubleSpinBox->setValue(_node->rotation().z());
    ui->rotationXDoubleSpinBox->blockSignals(false);
    ui->rotationYDoubleSpinBox->blockSignals(false);
    ui->rotationZDoubleSpinBox->blockSignals(false);

    ui->scaleXDoubleSpinBox->blockSignals(true);
    ui->scaleYDoubleSpinBox->blockSignals(true);
    ui->scaleZDoubleSpinBox->blockSignals(true);
    ui->scaleXDoubleSpinBox->setValue(_node->scale().x());
    ui->scaleYDoubleSpinBox->setValue(_node->scale().y());
    ui->scaleZDoubleSpinBox->setValue(_node->scale().z());
    ui->scaleXDoubleSpinBox->blockSignals(false);
    ui->scaleYDoubleSpinBox->blockSignals(false);
    ui->scaleZDoubleSpinBox->blockSignals(false);
}

void NodeDetailWidget::nodeUpdated()
{
    populateFieldsFromNode();
}

void NodeDetailWidget::doubleSpinBoxValueChanged(const double)
{
    QMatrix4x4 transform;
    transform.rotate(static_cast<float>(ui->rotationZDoubleSpinBox->value()), {0, 0, 1});
    transform.rotate(static_cast<float>(ui->rotationYDoubleSpinBox->value()), {0, 1, 0});
    transform.rotate(static_cast<float>(ui->rotationXDoubleSpinBox->value()), {1, 0, 0});

    QVector3D globalPosition = {
        static_cast<float>(ui->locationXDoubleSpinBox->value()),
        static_cast<float>(ui->locationYDoubleSpinBox->value()),
        static_cast<float>(ui->locationZDoubleSpinBox->value())
    };

    if (!_node->isRoot()) {
        QVector3D localPosition = _node->parent().globalTransform().inverted().map(globalPosition);
        transform.setColumn(3, {localPosition, 1});
    } else {
        transform.setColumn(3, {globalPosition, 1});
    }

    float sx = static_cast<float>(ui->scaleXDoubleSpinBox->value());
    float sy = static_cast<float>(ui->scaleYDoubleSpinBox->value());
    float sz = static_cast<float>(ui->scaleZDoubleSpinBox->value());

    QMatrix4x4 scale;
    scale.setRow(0, {sx, 0, 0, 0});
    scale.setRow(1, {0, sy, 0, 0});
    scale.setRow(2, {0, 0, sz, 0});

    _node->setTransform(transform * scale);
}

void NodeDetailWidget::loctnXDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new PositionNodeCommand(_window, _node, PositionNodeCommand::X, value));
}

void NodeDetailWidget::loctnYDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new PositionNodeCommand(_window, _node, PositionNodeCommand::Y, value));
}

void NodeDetailWidget::loctnZDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new PositionNodeCommand(_window, _node, PositionNodeCommand::Z, value));
}

void NodeDetailWidget::rottnXDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new RotateNodeCommand(_window, _node, RotateNodeCommand::X, value));
}

void NodeDetailWidget::rottnYDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new RotateNodeCommand(_window, _node, RotateNodeCommand::Y, value));
}

void NodeDetailWidget::rottnZDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new RotateNodeCommand(_window, _node, RotateNodeCommand::Z, value));
}

void NodeDetailWidget::scaleXDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new ScaleNodeCommand(_window, _node, ScaleNodeCommand::X, _lockScale, value));
}

void NodeDetailWidget::scaleYDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new ScaleNodeCommand(_window, _node, ScaleNodeCommand::Y, _lockScale, value));
}

void NodeDetailWidget::scaleZDoubleSpinBoxValueChanged(const double value)
{
    _window->app()->undoStack().push(new ScaleNodeCommand(_window, _node, ScaleNodeCommand::Z, _lockScale, value));
}

void NodeDetailWidget::lockScaleButtonClicked()
{
    _lockScale = !_lockScale;

    ui->lockScaleButton->setIcon(QIcon(_lockScale ? ":/icons/lock.png" : ":/icons/unlock.png"));
}
