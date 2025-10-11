#include "surfacewidget.h"
#include "ui_surfacewidget.h"

SurfaceWidget::SurfaceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SurfaceWidget)
{
    ui->setupUi(this);

    connect(ui->materialComboBox, &QComboBox::currentTextChanged, this, &SurfaceWidget::materialChanged);
}

SurfaceWidget::~SurfaceWidget()
{
    delete ui;
}

void SurfaceWidget::setSurface(Sahara::Model* model, Sahara::Instance *inst, const int surfaceIdx)
{
    _model = model;
    _inst = inst;
    _surfaceIdx = surfaceIdx;

    ui->materialComboBox->blockSignals(true);

    ui->materialComboBox->clear();
    for (auto& mat : _model->materials()) {
        ui->materialComboBox->addItem(mat->name());
    }

    Sahara::InstanceMesh* im;
    Sahara::InstanceController* ic;
    if ((im = dynamic_cast<Sahara::InstanceMesh*>(_inst))) {
        ui->materialComboBox->setCurrentText(im->getMaterial(im->mesh().surface(_surfaceIdx).material()).name() );
    } else if ((ic = dynamic_cast<Sahara::InstanceController*>(_inst))) {
        ui->materialComboBox->setCurrentText(ic->getMaterial(ic->controller().mesh().surface(_surfaceIdx).material()).name());
    }

    ui->materialComboBox->blockSignals(false);
}

void SurfaceWidget::materialChanged(const QString &name)
{
    Sahara::InstanceMesh* im;
    Sahara::InstanceController* ic;
    if ((im = dynamic_cast<Sahara::InstanceMesh*>(_inst))) {
        im->addMaterial(name, *std::find_if(_model->materials().begin(), _model->materials().end(), [&](const Sahara::Material* mat){ return mat->name() == name; }));
        im->mesh().surface(_surfaceIdx).setMaterial(name);
    } else if ((ic = dynamic_cast<Sahara::InstanceController*>(_inst))) {
        ic->addMaterial(name, *std::find_if(_model->materials().begin(), _model->materials().end(), [&](const Sahara::Material* mat){ return mat->name() == name; }));
        ic->controller().mesh().surface(_surfaceIdx).setMaterial(name);
    }
}
