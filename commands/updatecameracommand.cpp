#include "updatecameracommand.h"
#include "mainwindow.h"
UpdateCameraCommand::UpdateCameraCommand(MainWindow* window, Sahara::Camera* camera, Sahara::Camera::Mode cameraMode)
    : _window(window)
    , _camera(camera)
    , _parameter(Parameter::Mode)
    , _modeBefore(camera->mode())
    , _modeAfter(cameraMode)
{
    setText("update camera");
}

UpdateCameraCommand::UpdateCameraCommand(MainWindow* window, Sahara::Camera* camera, Parameter param, float value)
    : _window(window)
    , _camera(camera)
    , _parameter(param)
    , _valueAfter(value)
{
    setText("update camera");

    switch (_parameter) {
    case Mode:
        break;
    case AspectRatio:
        _valueBefore = _camera->aspect();
        break;
    case Xfov:
        _valueBefore = _camera->xfov();
        break;
    case Xmag:
        _valueBefore = _camera->xmag();
        break;
    case Znear:
        _valueBefore = _camera->znear();
        break;
    case Zfar:
        _valueBefore = _camera->zfar();
        break;
    }
}

void UpdateCameraCommand::undo()
{
    switch (_parameter) {
    case Mode:
        _camera->setMode(_modeBefore);
        break;
    case AspectRatio:
        _camera->setAspect(_valueBefore);
        break;
    case Xfov:
        _camera->setXfov(_valueBefore);
        break;
    case Xmag:
        _camera->setXmag(_valueBefore);
        break;
    case Znear:
        _camera->setZnear(_valueBefore);
        break;
    case Zfar:
        _camera->setZfar(_valueBefore);
        break;
    }

    _window->updateCameraFields();
}

void UpdateCameraCommand::redo()
{
    switch (_parameter) {
    case Mode:
        _camera->setMode(_modeAfter);
        break;
    case AspectRatio:
        _camera->setAspect(_valueAfter);
        break;
    case Xfov:
        _camera->setXfov(_valueAfter);
        break;
    case Xmag:
        _camera->setXmag(_valueAfter);
        break;
    case Znear:
        _camera->setZnear(_valueAfter);
        break;
    case Zfar:
        _camera->setZfar(_valueAfter);
        break;
    }

    _window->updateCameraFields();
}

int UpdateCameraCommand::id() const
{
    return 'C';
}

bool UpdateCameraCommand::mergeWith(const QUndoCommand* other)
{
    if (_parameter == Parameter::Mode) {
        return false;
    }

    const UpdateCameraCommand* ucc;
    if ((ucc = dynamic_cast<const UpdateCameraCommand*>(other))) {
        if (ucc->_camera == _camera && ucc->_parameter == _parameter) {
            _valueAfter = ucc->_valueAfter;
            return true;
        }
    }

    return false;
}

