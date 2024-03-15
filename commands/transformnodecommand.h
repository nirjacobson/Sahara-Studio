#ifndef TRANSFORMNODECOMMAND_H
#define TRANSFORMNODECOMMAND_H

#include <QUndoCommand>

#include "scene/node/node.h"

class MainWindow;

class TransformNodeCommand : public QUndoCommand
{
public:
    TransformNodeCommand(MainWindow* window, Sahara::Node* node, const QMatrix4x4& transform);

private:
    MainWindow* _window;
    Sahara::Node* _node;
    QMatrix4x4 _transformBefore;
    QMatrix4x4 _transformAfter;


    // QUndoCommand interface
public:
    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand* other);
};

#endif // TRANSFORMNODECOMMAND_H
