#include "camerawidget.h"
#include "ui_camerawidget.h"

CameraWidget::CameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWidget)
{
    ui->setupUi(this);

    connect(ui->perspectiveRadioButton, &QRadioButton::clicked, this, &CameraWidget::perspectiveRadioButtonClicked);
    connect(ui->orthographicRadioButton, &QRadioButton::clicked, this, &CameraWidget::orthographicRadioButtonClicked);

    connect(ui->aspectRatioDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(void)));
    connect(ui->xfovDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(void)));
    connect(ui->xmagDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(void)));
    connect(ui->znearDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(void)));
    connect(ui->zfarDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxValueChanged(void)));
}

CameraWidget::~CameraWidget()
{
    delete ui;
}

void CameraWidget::setCamera(Sahara::Camera* camera)
{
    _camera = camera;

    populateFieldsFromCamera();
}

void CameraWidget::updateFields()
{
    populateFieldsFromCamera();
}

void CameraWidget::populateFieldsFromCamera()
{
    ui->perspectiveRadioButton->blockSignals(true);
    ui->orthographicRadioButton->blockSignals(true);
    ui->aspectRatioDoubleSpinBox->blockSignals(true);
    ui->xfovDoubleSpinBox->blockSignals(true);
    ui->xmagDoubleSpinBox->blockSignals(true);
    ui->znearDoubleSpinBox->blockSignals(true);
    ui->zfarDoubleSpinBox->blockSignals(true);

    double aspectRatio = static_cast<double>(_camera->aspect());
    double xfov = static_cast<double>(_camera->xfov());
    double xmag = static_cast<double>(_camera->xmag());
    double znear = static_cast<double>(_camera->znear());
    double zfar = static_cast<double>(_camera->zfar());

    if (_camera->mode() == Sahara::Camera::Mode::Perspective) {
        ui->perspectiveRadioButton->click();
        ui->xmagLabel->setVisible(false);
        ui->xmagDoubleSpinBox->setVisible(false);
        ui->xfovLabel->setVisible(true);
        ui->xfovDoubleSpinBox->setVisible(true);
    }
    if (_camera->mode() == Sahara::Camera::Mode::Orthographic) {
        ui->orthographicRadioButton->click();
        ui->xfovLabel->setVisible(false);
        ui->xfovDoubleSpinBox->setVisible(false);
        ui->xmagLabel->setVisible(true);
        ui->xmagDoubleSpinBox->setVisible(true);
    }
    ui->aspectRatioDoubleSpinBox->setValue(aspectRatio);
    ui->xfovDoubleSpinBox->setValue(xfov);
    ui->xmagDoubleSpinBox->setValue(xmag);
    ui->znearDoubleSpinBox->setValue(znear);
    ui->zfarDoubleSpinBox->setValue(zfar);

    ui->perspectiveRadioButton->blockSignals(false);
    ui->orthographicRadioButton->blockSignals(false);
    ui->aspectRatioDoubleSpinBox->blockSignals(false);
    ui->xfovDoubleSpinBox->blockSignals(false);
    ui->xmagDoubleSpinBox->blockSignals(false);
    ui->znearDoubleSpinBox->blockSignals(false);
    ui->zfarDoubleSpinBox->blockSignals(false);
}

void CameraWidget::perspectiveRadioButtonClicked()
{
    _camera->setMode(Sahara::Camera::Mode::Perspective);
    ui->xmagLabel->setVisible(false);
    ui->xmagDoubleSpinBox->setVisible(false);
    ui->xfovLabel->setVisible(true);
    ui->xfovDoubleSpinBox->setVisible(true);
}

void CameraWidget::orthographicRadioButtonClicked()
{
    _camera->setMode(Sahara::Camera::Mode::Orthographic);
    ui->xfovLabel->setVisible(false);
    ui->xfovDoubleSpinBox->setVisible(false);
    ui->xmagLabel->setVisible(true);
    ui->xmagDoubleSpinBox->setVisible(true);
}

void CameraWidget::doubleSpinBoxValueChanged()
{

    float aspectRatio = static_cast<float>(ui->aspectRatioDoubleSpinBox->value());
    float xfov = static_cast<float>(ui->xfovDoubleSpinBox->value());
    float xmag = static_cast<float>(ui->xmagDoubleSpinBox->value());
    float znear = static_cast<float>(ui->znearDoubleSpinBox->value());
    float zfar = static_cast<float>(ui->zfarDoubleSpinBox->value());

    _camera->setAspect(aspectRatio);
    _camera->setXfov(xfov);
    _camera->setXmag(xmag);
    _camera->setZnear(znear);
    _camera->setZfar(zfar);
}
