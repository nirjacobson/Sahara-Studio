#ifndef ROTATENODECOMMAND_H
#define ROTATENODECOMMAND_H

#include <QUndoCommand>

#include <Sahara/scene/node/node.h>

class MainWindow;

class RotateNodeCommand : public QUndoCommand
{
public:
    enum Axis {
        X, Y, Z
    };

    RotateNodeCommand(MainWindow* window, Sahara::Node* node, Axis axis, float value);

private:
    MainWindow* _window;
    Sahara::Node* _node;
    Axis _axis;
    QVector3D _oldRotation;
    QVector3D _newRotation;

    // QUndoCommand interface
public:
    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand* other);
};

#endif // ROTATENODECOMMAND_H
