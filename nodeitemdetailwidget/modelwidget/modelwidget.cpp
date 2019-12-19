#include "modelwidget.h"
#include "ui_modelwidget.h"

ModelWidget::ModelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModelWidget),
    _geometryModel(nullptr),
    _geometryItemModel(nullptr)
{
    ui->setupUi(this);
    ui->materialWidget->setVisible(false);
    ui->tabWidget->setCurrentIndex( ui->tabWidget->count() - 1 );
    _animationsTabWidget = ui->tabWidget->currentWidget();
    ui->tabWidget->setCurrentIndex(0);

    connect(ui->materialsComboBox, &QComboBox::currentTextChanged, this, &ModelWidget::materialsComboBoxCurrentTextChanged);
    connect(ui->animationsComboBox, &QComboBox::currentTextChanged, this, &ModelWidget::animationsComboBoxCurrentTextChanged);
}

ModelWidget::~ModelWidget()
{
    delete ui;
}

void ModelWidget::setModel(Sahara::Model* model)
{
    _model = model;

    if (_geometryItemModel)
        delete _geometryItemModel;

    if (_geometryModel)
        delete _geometryModel;

    _geometryModel = new GeometryModel(*model);
    _geometryItemModel = new GeometryItemModel(*_geometryModel);
    ui->geometryTreeView->setModel(_geometryItemModel);
    ui->geometryTreeView->expandAll();
    ui->geometryTreeView->resizeColumnToContents(0);

    ui->materialsComboBox->clear();
    for (Sahara::MaterialDict::const_iterator i = _model->materials().begin(); i != _model->materials().end(); i++) {
        ui->materialsComboBox->addItem(i.key());
    }

    ui->animationsComboBox->blockSignals(true);
    ui->animationsComboBox->clear();
    for (const QString& animationClip : _model->animationClipNames()) {
        ui->animationsComboBox->addItem(animationClip);
    }
    ui->animationsComboBox->setCurrentText(_model->animationClip());
    ui->animationsComboBox->blockSignals(false);

    if (ui->tabWidget->count() == 3) {
        if (_model->animationClips().isEmpty()) {
            ui->tabWidget->removeTab(2);
        }
    } else {
        if (!_model->animationClips().isEmpty()) {
            ui->tabWidget->insertTab(2, _animationsTabWidget, "Animations");
        }
    }
}

void ModelWidget::materialsComboBoxCurrentTextChanged(const QString& text)
{
    if (text.isEmpty() || text.isNull()) {
        ui->materialWidget->setVisible(false);
        return;
    }

    ui->materialWidget->setMaterial(_model->materials()[text]);
    ui->materialWidget->setVisible(true);
}

void ModelWidget::animationsComboBoxCurrentTextChanged(const QString& text)
{
    _model->setAnimationClip(text);
}