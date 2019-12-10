#include "pointlightwidget.h"
#include "ui_pointlightwidget.h"

PointLightWidget::PointLightWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointLightWidget)
{
    ui->setupUi(this);

    connect(ui->colorWidget, SIGNAL(valueChanged(const QColor&)), this, SLOT(fieldValueChanged()));
    connect(ui->constantDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(fieldValueChanged()));
    connect(ui->linearDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(fieldValueChanged()));
    connect(ui->quadraticDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(fieldValueChanged()));
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

    ui->colorWidget->setValue(_pointLight->color());
    ui->constantDoubleSpinBox->setValue(constant);
    ui->linearDoubleSpinBox->setValue(linear);
    ui->quadraticDoubleSpinBox->setValue(quadratic);
}

void PointLightWidget::fieldValueChanged()
{
    float constant = static_cast<float>(ui->constantDoubleSpinBox->value());
    float linear = static_cast<float>(ui->linearDoubleSpinBox->value());
    float quadratic = static_cast<float>(ui->quadraticDoubleSpinBox->value());

    _pointLight->setColor(ui->colorWidget->value());
    _pointLight->setConstantAttenuation(constant);
    _pointLight->setLinearAttenuation(linear);
    _pointLight->setQuadraticAttenuation(quadratic);
}
