#include "camerawidget.h"
#include "ui_camerawidget.h"
#include "mainwindow.h"

CameraWidget::CameraWidget(MainWindow* window, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWidget),
    _window(window)
{
    ui->setupUi(this);

    connect(ui->perspectiveRadioButton, &QRadioButton::clicked, this, &CameraWidget::perspectiveRadioButtonClicked);
    connect(ui->orthographicRadioButton, &QRadioButton::clicked, this, &CameraWidget::orthographicRadioButtonClicked);

    connect(ui->aspectRatioDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(aspectChanged(double)));
    connect(ui->xfovDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(xfovChanged(double)));
    connect(ui->xmagDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(xmagChanged(double)));
    connect(ui->znearDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(znearChanged(double)));
    connect(ui->zfarDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(zfarChanged(double)));
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
    _window->app()->undoStack().push(new UpdateCameraCommand(_window, _camera, Sahara::Camera::Mode::Perspective));
}

void CameraWidget::orthographicRadioButtonClicked()
{
    _window->app()->undoStack().push(new UpdateCameraCommand(_window, _camera, Sahara::Camera::Mode::Orthographic));
}

void CameraWidget::aspectChanged(double value)
{
    _window->app()->undoStack().push(new UpdateCameraCommand(_window, _camera, UpdateCameraCommand::AspectRatio, value));
}

void CameraWidget::xfovChanged(double value)
{
    _window->app()->undoStack().push(new UpdateCameraCommand(_window, _camera, UpdateCameraCommand::Xfov, value));
}

void CameraWidget::xmagChanged(double value)
{
    _window->app()->undoStack().push(new UpdateCameraCommand(_window, _camera, UpdateCameraCommand::Xmag, value));
}

void CameraWidget::znearChanged(double value)
{
    _window->app()->undoStack().push(new UpdateCameraCommand(_window, _camera, UpdateCameraCommand::Znear, value));
}

void CameraWidget::zfarChanged(double value)
{
    _window->app()->undoStack().push(new UpdateCameraCommand(_window, _camera, UpdateCameraCommand::Zfar, value));
}
