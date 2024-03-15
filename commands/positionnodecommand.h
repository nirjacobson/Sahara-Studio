#ifndef POSITIONNODECOMMAND_H
#define POSITIONNODECOMMAND_H

#include <QUndoCommand>

#include "scene/node/node.h"

class MainWindow;

class PositionNodeCommand : public QUndoCommand
{
public:
    enum Axis {
        X, Y, Z
    };

    PositionNodeCommand(MainWindow* window, Sahara::Node* node, Axis axis, float value);

private:
    MainWindow* _window;
    Sahara::Node* _node;
    Axis _axis;
    QVector3D _oldLocation;
    QVector3D _newLocation;

    // QUndoCommand interface
public:
    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand* other);
};

#endif // POSITIONNODECOMMAND_H
