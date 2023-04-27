#include "scenewidget.h"
#include "ui_scenewidget.h"

SceneWidget::SceneWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SceneWidget)
{
    ui->setupUi(this);

    connect(ui->colorWidget, SIGNAL(valueChanged(const QColor&)), this, SLOT(fieldValueChanged()));
    connect(ui->strengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(fieldValueChanged()));
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

void SceneWidget::populateFieldsFromScene()
{
    double strength = static_cast<double>(_scene->ambientLight().strength());

    ui->colorWidget->setValue(_scene->ambientLight().color());
    ui->strengthSpinBox->setValue(strength);
}

void SceneWidget::fieldValueChanged()
{
    float strength = static_cast<float>(ui->strengthSpinBox->value());

    _scene->ambientLight().setColor(ui->colorWidget->value());
    _scene->ambientLight().setStrength(strength);
}
