#ifndef SCALENODECOMMAND_H
#define SCALENODECOMMAND_H

#include <QUndoCommand>

#include "scene/node/node.h"

class MainWindow;

class ScaleNodeCommand : public QUndoCommand
{
public:
    enum Axis {
        X, Y, Z
    };

    ScaleNodeCommand(MainWindow* window, Sahara::Node* node, Axis axis, bool lockAxes, float value);

private:
    MainWindow* _window;
    Sahara::Node* _node;
    Axis _axis;
    bool _lockAxes;
    QVector3D _oldScale;
    QVector3D _newScale;

    // QUndoCommand interface
public:
    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand* other);
};

#endif // SCALENODECOMMAND_H
