#include "pointlightwidget.h"
#include "ui_pointlightwidget.h"
#include "mainwindow.h"

PointLightWidget::PointLightWidget(MainWindow* window, QWidget *parent) :
    QWidget(parent),
    _window(window),
    ui(new Ui::PointLightWidget)
{
    ui->setupUi(this);

    connect(ui->colorWidget, SIGNAL(valueChanged(const QColor&)), this, SLOT(colorChanged(const QColor&)));
    connect(ui->constantDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(constantAttenuationChanged(double)));
    connect(ui->linearDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(linearAttenuationChanged(double)));
    connect(ui->quadraticDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(quadraticAttenuationChanged(double)));
}

PointLightWidget::~PointLightWidget()
{
    delete ui;
}

void PointLightWidget::setPointLight(Sahara::PointLight* pointLight)
{
    _pointLight = pointLight;

    populateFieldsFromPointLight();
}

void PointLightWidget::updateFields()
{
    populateFieldsFromPointLight();
}

void PointLightWidget::populateFieldsFromPointLight()
{
    double constant = static_cast<double>(_pointLight->constantAttenuation());
    double linear = static_cast<double>(_pointLight->linearAttenuation());
    double quadratic = static_cast<double>(_pointLight->quadraticAttenuation());

    ui->colorWidget->blockSignals(true);
    ui->constantDoubleSpinBox->blockSignals(true);
    ui->linearDoubleSpinBox->blockSignals(true);
    ui->quadraticDoubleSpinBox->blockSignals(true);

    ui->colorWidget->setValue(_pointLight->color());
    ui->constantDoubleSpinBox->setValue(constant);
    ui->linearDoubleSpinBox->setValue(linear);
    ui->quadraticDoubleSpinBox->setValue(quadratic);

    ui->colorWidget->blockSignals(false);
    ui->constantDoubleSpinBox->blockSignals(false);
    ui->linearDoubleSpinBox->blockSignals(false);
    ui->quadraticDoubleSpinBox->blockSignals(false);
}

void PointLightWidget::colorChanged(const QColor& color)
{
    _window->app()->undoStack().push(new UpdatePointLightCommand(_window, _pointLight, color));
}

void PointLightWidget::constantAttenuationChanged(double value)
{
    _window->app()->undoStack().push(new UpdatePointLightCommand(_window, _pointLight, UpdatePointLightCommand::ConstantAttenuation, value));
}

void PointLightWidget::linearAttenuationChanged(double value)
{
    _window->app()->undoStack().push(new UpdatePointLightCommand(_window, _pointLight, UpdatePointLightCommand::LinearAttenuation, value));
}

void PointLightWidget::quadraticAttenuationChanged(double value)
{
    _window->app()->undoStack().push(new UpdatePointLightCommand(_window, _pointLight, UpdatePointLightCommand::QuadraticAttenuation, value));
}
