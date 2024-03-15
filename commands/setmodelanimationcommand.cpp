#include "setmodelanimationcommand.h"
#include "mainwindow.h"

SetModelAnimationCommand::SetModelAnimationCommand(MainWindow* window, Sahara::Model* model, const QString& clipName)
    : _window(window)
    , _model(model)
    , _clipNameBefore(model->animationClip())
    , _clipNameAfter(clipName)
{
    setText("set animation");
}

void SetModelAnimationCommand::undo()
{
    _model->setAnimationClip(_clipNameBefore);
    _window->updateModelFields();
}

void SetModelAnimationCommand::redo()
{
    _model->setAnimationClip(_clipNameAfter);
    _window->updateModelFields();
}
