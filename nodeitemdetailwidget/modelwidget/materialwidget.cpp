#include "materialwidget.h"
#include "ui_materialwidget.h"
#include "mainwindow.h"

MaterialWidget::MaterialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaterialWidget)
{
    ui->setupUi(this);

    ui->imageFileLink->setVisible(false);

    connect(ui->emissionColorWidget, &ColorWidget::valueChanged, this, &MaterialWidget::emissionChanged);
    connect(ui->ambientColorWidget, &ColorWidget::valueChanged, this, &MaterialWidget::ambientChanged);
    connect(ui->diffuseColorWidget, &ColorWidget::valueChanged, this, &MaterialWidget::diffuseChanged);
    connect(ui->specularColorWidget, &ColorWidget::valueChanged, this, &MaterialWidget::specularChanged);
    connect(ui->shininessDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(shininessChanged(double)));
    connect(ui->imageFileLink, &LinkWidget::clicked, this, &MaterialWidget::imageFileLinkClicked);
}

MaterialWidget::~MaterialWidget()
{
    delete ui;
}

void MaterialWidget::setWindow(MainWindow* window)
{
    _window = window;
}

void MaterialWidget::setMaterial(Sahara::Material* material)
{
    _material = material;

    populateFieldsFromMaterial();
}

void MaterialWidget::updateFields()
{
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

    Sahara::OpenGLMaterial* glMaterial;
    Sahara::VulkanMaterial* vkMaterial;
    if ((glMaterial = dynamic_cast<Sahara::OpenGLMaterial*>(_material))) {
        if (glMaterial->image().has_value()) {
            ui->diffuseLabel->setText("Image:");
            QString url = QUrl::fromPercentEncoding((*glMaterial->image())->uri().toLatin1());
            ui->imageFileLink->setText(QFileInfo(url).fileName());
            ui->imageFileLink->setVisible(true);
            ui->diffuseColorWidget->setVisible(false);
        } else {
            ui->diffuseLabel->setText("Diffuse:");
            ui->diffuseColorWidget->setValue(_material->diffuse());
            ui->imageFileLink->setVisible(false);
            ui->diffuseColorWidget->setVisible(true);
        }
    } else {
        vkMaterial = dynamic_cast<Sahara::VulkanMaterial*>(_material);
        if (vkMaterial->image().has_value()) {
            ui->diffuseLabel->setText("Image:");
            QString url = QUrl::fromPercentEncoding((*vkMaterial->image())->uri().toLatin1());
            ui->imageFileLink->setText(QFileInfo(url).fileName());
            ui->imageFileLink->setVisible(true);
            ui->diffuseColorWidget->setVisible(false);
        } else {
            ui->diffuseLabel->setText("Diffuse:");
            ui->diffuseColorWidget->setValue(_material->diffuse());
            ui->imageFileLink->setVisible(false);
            ui->diffuseColorWidget->setVisible(true);
        }
    }

    ui->emissionColorWidget->blockSignals(false);
    ui->ambientColorWidget->blockSignals(false);
    ui->diffuseColorWidget->blockSignals(false);
    ui->specularColorWidget->blockSignals(false);
    ui->shininessDoubleSpinBox->blockSignals(false);
}

void MaterialWidget::emissionChanged(const QColor& color)
{
    _window->app()->undoStack().push(new UpdateModelMaterialCommand(_window, _material, UpdateModelMaterialCommand::EmissionColor, color));
}

void MaterialWidget::ambientChanged(const QColor& color)
{
    _window->app()->undoStack().push(new UpdateModelMaterialCommand(_window, _material, UpdateModelMaterialCommand::AmbientColor, color));
}

void MaterialWidget::diffuseChanged(const QColor& color)
{
    _window->app()->undoStack().push(new UpdateModelMaterialCommand(_window, _material, UpdateModelMaterialCommand::DiffuseColor, color));
}

void MaterialWidget::specularChanged(const QColor& color)
{
    _window->app()->undoStack().push(new UpdateModelMaterialCommand(_window, _material, UpdateModelMaterialCommand::SpecularColor, color));
}

void MaterialWidget::shininessChanged(double value)
{
    _window->app()->undoStack().push(new UpdateModelMaterialCommand(_window, _material, value));
}

void MaterialWidget::imageFileLinkClicked()
{
    QString url;
    Sahara::OpenGLMaterial* glMaterial;
    Sahara::VulkanMaterial* vkMaterial;
    if ((glMaterial = dynamic_cast<Sahara::OpenGLMaterial*>(_material))) {
        url = QUrl::fromPercentEncoding((*glMaterial->image())->uri().toLatin1());
    } else {
        vkMaterial = dynamic_cast<Sahara::VulkanMaterial*>(_material);
        url = QUrl::fromPercentEncoding((*vkMaterial->image())->uri().toLatin1());
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}
