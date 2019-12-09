#include "nodedetailwidget.h"
#include "ui_nodedetailwidget.h"

NodeDetailWidget::NodeDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodeDetailWidget),
    _node(nullptr),
    _lockScale(true)
{
    ui->setupUi(this);

    ui->lockScaleButton->setIcon(QIcon(":/icons/lock.png"));

    connect(ui->lockScaleButton, &QPushButton::clicked, this, &NodeDetailWidget::lockScaleButtonClicked);

    connect(ui->locationXDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(const double)));
    connect(ui->locationYDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(const double)));
    connect(ui->locationZDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(const double)));
    connect(ui->rotationXDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(const double)));
    connect(ui->rotationYDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(const double)));
    connect(ui->rotationZDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(const double)));
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

    setTransformFieldsFromNode();
}

void NodeDetailWidget::setTransformFieldsFromNode()
{
    double m20 = static_cast<double>(_node->transform().row(2)[0]);
    double m10 = static_cast<double>(_node->transform().row(1)[0]);
    double m00 = static_cast<double>(_node->transform().row(0)[0]);
    double m21 = static_cast<double>(_node->transform().row(2)[1]);
    double m22 = static_cast<double>(_node->transform().row(2)[2]);
    double m12 = static_cast<double>(_node->transform().row(1)[2]);
    double m11 = static_cast<double>(_node->transform().row(1)[1]);

    double lx = static_cast<double>(_node->globalPosition().x());
    double ly = static_cast<double>(_node->globalPosition().x());
    double lz = static_cast<double>(_node->globalPosition().x());

    double rx, ry, rz;
    if (m20 < 1) {
        if (m20 > -1) {
            rx = qAtan2(m21, m22);
            ry = qAsin(-m20);
            rz = qAtan2(m10, m00);
        } else {
            rx = 0;
            ry = M_PI / 2;
            rz = -qAtan2(-m12, m11);
        }
    } else {
        rx = 0;
        ry = -M_PI / 2;
        rz = qAtan2(-m12, m11);
    }

    rx = qRadiansToDegrees(rx);
    ry = qRadiansToDegrees(ry);
    rz = qRadiansToDegrees(rz);

    double sx = static_cast<double>(_node->transform().column(0).length());
    double sy = static_cast<double>(_node->transform().column(1).length());
    double sz = static_cast<double>(_node->transform().column(2).length());

    ui->locationXDoubleSpinBox->blockSignals(true);
    ui->locationYDoubleSpinBox->blockSignals(true);
    ui->locationZDoubleSpinBox->blockSignals(true);
    ui->locationXDoubleSpinBox->setValue(lx);
    ui->locationYDoubleSpinBox->setValue(ly);
    ui->locationZDoubleSpinBox->setValue(lz);
    ui->locationXDoubleSpinBox->blockSignals(false);
    ui->locationYDoubleSpinBox->blockSignals(false);
    ui->locationZDoubleSpinBox->blockSignals(false);

    ui->rotationXDoubleSpinBox->blockSignals(true);
    ui->rotationYDoubleSpinBox->blockSignals(true);
    ui->rotationZDoubleSpinBox->blockSignals(true);
    ui->rotationXDoubleSpinBox->setValue(rx);
    ui->rotationYDoubleSpinBox->setValue(ry);
    ui->rotationZDoubleSpinBox->setValue(rz);
    ui->rotationXDoubleSpinBox->blockSignals(false);
    ui->rotationYDoubleSpinBox->blockSignals(false);
    ui->rotationZDoubleSpinBox->blockSignals(false);

    ui->scaleXDoubleSpinBox->blockSignals(true);
    ui->scaleYDoubleSpinBox->blockSignals(true);
    ui->scaleZDoubleSpinBox->blockSignals(true);
    ui->scaleXDoubleSpinBox->setValue(sx);
    ui->scaleYDoubleSpinBox->setValue(sy);
    ui->scaleZDoubleSpinBox->setValue(sz);
    ui->scaleXDoubleSpinBox->blockSignals(false);
    ui->scaleYDoubleSpinBox->blockSignals(false);
    ui->scaleZDoubleSpinBox->blockSignals(false);
}

void NodeDetailWidget::nodeUpdated()
{
    setTransformFieldsFromNode();
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

    emit updated();
}

void NodeDetailWidget::scaleXDoubleSpinBoxValueChanged(const double value)
{
    if (_lockScale) {
        ui->scaleYDoubleSpinBox->blockSignals(true);
        ui->scaleZDoubleSpinBox->blockSignals(true);
        ui->scaleYDoubleSpinBox->setValue(value);
        ui->scaleZDoubleSpinBox->setValue(value);
        ui->scaleYDoubleSpinBox->blockSignals(false);
        ui->scaleZDoubleSpinBox->blockSignals(false);
    }

    doubleSpinBoxValueChanged(value);
}

void NodeDetailWidget::scaleYDoubleSpinBoxValueChanged(const double value)
{
    if (_lockScale) {
        ui->scaleXDoubleSpinBox->blockSignals(true);
        ui->scaleZDoubleSpinBox->blockSignals(true);
        ui->scaleXDoubleSpinBox->setValue(value);
        ui->scaleZDoubleSpinBox->setValue(value);
        ui->scaleXDoubleSpinBox->blockSignals(false);
        ui->scaleZDoubleSpinBox->blockSignals(false);
    }

    doubleSpinBoxValueChanged(value);
}

void NodeDetailWidget::scaleZDoubleSpinBoxValueChanged(const double value)
{
    if (_lockScale) {
        ui->scaleXDoubleSpinBox->blockSignals(true);
        ui->scaleYDoubleSpinBox->blockSignals(true);
        ui->scaleXDoubleSpinBox->setValue(value);
        ui->scaleYDoubleSpinBox->setValue(value);
        ui->scaleXDoubleSpinBox->blockSignals(false);
        ui->scaleYDoubleSpinBox->blockSignals(false);
    }

    doubleSpinBoxValueChanged(value);
}

void NodeDetailWidget::lockScaleButtonClicked()
{
    _lockScale = !_lockScale;

    ui->lockScaleButton->setIcon(QIcon(_lockScale ? ":/icons/lock.png" : ":/icons/unlock.png"));
}
