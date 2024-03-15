#ifndef SETMODELANIMATIONCOMMAND_H
#define SETMODELANIMATIONCOMMAND_H

#include <QUndoCommand>
#include "scene/model.h"

class MainWindow;

class SetModelAnimationCommand : public QUndoCommand
{
public:
    SetModelAnimationCommand(MainWindow* window, Sahara::Model* model, const QString& clipName);

private:
    MainWindow* _window;
    Sahara::Model* _model;
    QString _clipNameBefore;
    QString _clipNameAfter;

    // QUndoCommand interface
public:
    void undo();
    void redo();
};

#endif // SETMODELANIMATIONCOMMAND_H
