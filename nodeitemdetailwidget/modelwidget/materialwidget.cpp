#include "materialwidget.h"
#include "ui_materialwidget.h"

MaterialWidget::MaterialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaterialWidget)
{
    ui->setupUi(this);

    ui->imageFileLink->setVisible(false);

    connect(ui->emissionColorWidget, &ColorWidget::valueChanged, this, &MaterialWidget::fieldValueChanged);
    connect(ui->ambientColorWidget, &ColorWidget::valueChanged, this, &MaterialWidget::fieldValueChanged);
    connect(ui->diffuseColorWidget, &ColorWidget::valueChanged, this, &MaterialWidget::fieldValueChanged);
    connect(ui->specularColorWidget, &ColorWidget::valueChanged, this, &MaterialWidget::fieldValueChanged);
    connect(ui->shininessDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(fieldValueChanged()));
    connect(ui->imageFileLink, &LinkWidget::clicked, this, &MaterialWidget::imageFileLinkClicked);
}

MaterialWidget::~MaterialWidget()
{
    delete ui;
}

void MaterialWidget::setMaterial(Sahara::Material* material)
{
    _material = material;

    populateFieldsFromMaterial();
}

void MaterialWidget::populateFieldsFromMaterial()
{
    ui->emissionColorWidget->blockSignals(true);
    ui->ambientColorWidget->blockSignals(true);
    ui->diffuseColorWidget->blockSignals(true);
    ui->specularColorWidget->blockSignals(true);
    ui->shininessDoubleSpinBox->blockSignals(true);

    ui->emissionColorWidget->setValue(_material->emission());
    ui->ambientColorWidget->setValue(_material->ambient());
    ui->specularColorWidget->setValue(_material->specular());
    ui->shininessDoubleSpinBox->setValue(static_cast<double>(_material->shininess()));

    if (_material->image().has_value()) {
        ui->diffuseLabel->setText("Image:");
        ui->imageFileLink->setText(QFileInfo((*_material->image())->uri()).fileName());
        ui->imageFileLink->setVisible(true);
        ui->diffuseColorWidget->setVisible(false);
    } else {
        ui->diffuseLabel->setText("Diffuse:");
        ui->diffuseColorWidget->setValue(_material->diffuse());
        ui->imageFileLink->setVisible(false);
        ui->diffuseColorWidget->setVisible(true);
    }

    ui->emissionColorWidget->blockSignals(false);
    ui->ambientColorWidget->blockSignals(false);
    ui->diffuseColorWidget->blockSignals(false);
    ui->specularColorWidget->blockSignals(false);
    ui->shininessDoubleSpinBox->blockSignals(false);
}

void MaterialWidget::fieldValueChanged()
{
    _material->setEmission(ui->emissionColorWidget->value());
    _material->setAmbient(ui->ambientColorWidget->value());
    _material->setDiffuse(ui->diffuseColorWidget->value());
    _material->setSpecular(ui->specularColorWidget->value());
    _material->setShininess(static_cast<float>(ui->shininessDoubleSpinBox->value()));
}

void MaterialWidget::imageFileLinkClicked()
{
    QDesktopServices::openUrl(QUrl((*_material->image())->uri()));
}
