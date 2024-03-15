#include "scenewidget.h"
#include "ui_scenewidget.h"
#include "mainwindow.h"

SceneWidget::SceneWidget(MainWindow* window, QWidget *parent) :
    QWidget(parent),
    _window(window),
    ui(new Ui::SceneWidget)
{
    ui->setupUi(this);

    connect(ui->colorWidget, SIGNAL(valueChanged(const QColor&)), this, SLOT(ambientColorChanged(const QColor&)));
    connect(ui->strengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(ambientStrengthChanged(double)));
}

SceneWidget::~SceneWidget()
{
    delete ui;
}

void SceneWidget::setScene(Sahara::Scene *scene)
{
    _scene = scene;

    populateFieldsFromScene();
}

void SceneWidget::updateFields()
{
    populateFieldsFromScene();
}

void SceneWidget::populateFieldsFromScene()
{
    double strength = static_cast<double>(_scene->ambientLight().strength());

    ui->colorWidget->blockSignals(true);
    ui->strengthSpinBox->blockSignals(true);

    ui->colorWidget->setValue(_scene->ambientLight().color());
    ui->strengthSpinBox->setValue(strength);

    ui->colorWidget->blockSignals(false);
    ui->strengthSpinBox->blockSignals(false);
}

void SceneWidget::ambientColorChanged(const QColor& color)
{
    _window->app()->undoStack().push(new UpdateSceneAmbientLightCommand(_window, _scene, color));
}

void SceneWidget::ambientStrengthChanged(double value)
{
    _window->app()->undoStack().push(new UpdateSceneAmbientLightCommand(_window, _scene, value));
}
