#ifndef UPDATECAMERACOMMAND_H
#define UPDATECAMERACOMMAND_H

#include <QUndoCommand>

#include "scene/asset/camera.h"

class MainWindow;

class UpdateCameraCommand : public QUndoCommand
{
public:
    enum Parameter {
        Mode,
        AspectRatio,
        Xfov,
        Xmag,
        Znear,
        Zfar
    };
    UpdateCameraCommand(MainWindow* window, Sahara::Camera* camera, Sahara::Camera::Mode cameraMode);
    UpdateCameraCommand(MainWindow* window, Sahara::Camera* camera, Parameter param, float value);

private:
    MainWindow* _window;
    Sahara::Camera* _camera;
    Parameter _parameter;
    Sahara::Camera::Mode _modeBefore;
    float _valueBefore;
    Sahara::Camera::Mode _modeAfter;
    float _valueAfter;

    // QUndoCommand interface
public:
    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand* other);
};

#endif // UPDATECAMERACOMMAND_H
