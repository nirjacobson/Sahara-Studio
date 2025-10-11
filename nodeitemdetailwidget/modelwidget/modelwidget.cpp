#include "modelwidget.h"
#include "ui_modelwidget.h"
#include "mainwindow.h"

ModelWidget::ModelWidget(MainWindow* window, QWidget *parent) :
    QWidget(parent),
    _window(window),
    ui(new Ui::ModelWidget),
    _geometryModel(nullptr),
    _geometryItemModel(nullptr)
{
    ui->setupUi(this);
    ui->materialWidget->setWindow(window);
    ui->materialWidget->setVisible(false);
    ui->tabWidget->setCurrentIndex( ui->tabWidget->count() - 1 );
    _animationsTabWidget = ui->tabWidget->currentWidget();
    ui->tabWidget->setCurrentIndex(0);

    ui->addMaterialPushButton->setIcon(QIcon(":/icons/add.png"));

    connect(ui->materialsComboBox, &QComboBox::currentTextChanged, this, &ModelWidget::materialsComboBoxCurrentTextChanged);
    connect(ui->animationsComboBox, &QComboBox::currentTextChanged, this, &ModelWidget::animationsComboBoxCurrentTextChanged);
    connect(ui->addMaterialPushButton, &QPushButton::clicked, this, &ModelWidget::addMaterial);
}

ModelWidget::~ModelWidget()
{
    delete ui;
}

void ModelWidget::setModel(Sahara::Model* model)
{
    ui->geometryTreeView->setModel(nullptr);

    _model = model;

    if (_geometryItemModel)
        delete _geometryItemModel;

    if (_geometryModel)
        delete _geometryModel;

    _materialIdsByName.clear();

    _geometryModel = new GeometryModel(*model);
    _geometryItemModel = new GeometryItemModel(*_geometryModel);
    ui->geometryTreeView->setModel(_geometryItemModel);
    ui->geometryTreeView->expandAll();
    ui->geometryTreeView->resizeColumnToContents(0);

    ui->materialsComboBox->clear();
    for (Sahara::MaterialDict::const_iterator i = _model->materials().begin(); i != _model->materials().end(); i++) {
        _materialIdsByName[i.value()->name()] = i.key();
        ui->materialsComboBox->addItem(i.value()->name());
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

void ModelWidget::updateFields()
{
    ui->materialWidget->updateFields();
    ui->animationsComboBox->blockSignals(true);
    ui->animationsComboBox->setCurrentText(_model->animationClip());
    ui->animationsComboBox->blockSignals(false);
}

void ModelWidget::materialsComboBoxCurrentTextChanged(const QString& text)
{
    if (text.isEmpty() || text.isNull()) {
        ui->materialWidget->setVisible(false);
        return;
    }

    ui->materialWidget->setMaterial(_model->materials()[_materialIdsByName[text]]);
    ui->materialWidget->setVisible(true);
}

void ModelWidget::animationsComboBoxCurrentTextChanged(const QString& text)
{
    _window->app()->undoStack().push(new SetModelAnimationCommand(_window, _model, text));
}

void ModelWidget::addMaterial()
{
    bool ok;
    QString name = QInputDialog::getText(nullptr, "Enter material name", "Material name:", QLineEdit::Normal, QString(), &ok);

    if (ok && !name.isEmpty()) {
        if (dynamic_cast<Sahara::OpenGLModel*>(_model)) {
            _model->addMaterial(name, new Sahara::OpenGLMaterial(name, name, QColor(), QColor(), QColor(), QColor(), 0));
        } else if (dynamic_cast<Sahara::VulkanModel*>(_model)) {
            _model->addMaterial(name, new Sahara::VulkanMaterial(dynamic_cast<Sahara::VulkanRenderer*>(_window->renderer()), name, name,  QColor(), QColor(), QColor(), QColor(), 0));
        }
        _materialIdsByName[name] = name;
        ui->materialsComboBox->addItem(name);
        ui->materialsComboBox->setCurrentText(name);
    }
}
