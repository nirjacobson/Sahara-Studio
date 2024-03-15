#include "rotatenodecommand.h"
#include "mainwindow.h"

RotateNodeCommand::RotateNodeCommand(MainWindow* window, Sahara::Node* node, Axis axis, float value)
    : _window(window)
    , _node(node)
    , _axis(axis)
    , _oldRotation(_node->rotation())
{
    setText("rotate node");

    _newRotation = _oldRotation;

    switch (_axis) {
    case X:
        _newRotation.setX(value);
        break;
    case Y:
        _newRotation.setY(value);
        break;
    case Z:
        _newRotation.setZ(value);
        break;
    }
}

void RotateNodeCommand::undo()
{
    _node->setRotation(_oldRotation);

    _window->updateNodeFields();
}

void RotateNodeCommand::redo()
{
    _node->setRotation(_newRotation);

    _window->updateNodeFields();
}

int RotateNodeCommand::id() const
{
    return 'R';
}

bool RotateNodeCommand::mergeWith(const QUndoCommand* other)
{
    const RotateNodeCommand* rnc;
    if ((rnc = dynamic_cast<const RotateNodeCommand*>(other))) {
        if (rnc->_node == _node && rnc->_axis == _axis) {
            _newRotation = rnc->_newRotation;

            return true;
        }
    }

    return false;
}
