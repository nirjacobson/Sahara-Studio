#include "positionnodecommand.h"
#include "mainwindow.h"

PositionNodeCommand::PositionNodeCommand(MainWindow* window, Sahara::Node* node, Axis axis, float value)
    : _window(window)
    , _node(node)
    , _axis(axis)
    , _oldLocation(_node->location())
{
    setText("position node");

    _newLocation = _oldLocation;

    switch (_axis) {
    case X:
        _newLocation.setX(value);
        break;
    case Y:
        _newLocation.setY(value);
        break;
    case Z:
        _newLocation.setZ(value);
        break;
    }
}

void PositionNodeCommand::undo()
{
    _node->setLocation(_oldLocation);

    _window->updateNodeFields();
}

void PositionNodeCommand::redo()
{
    _node->setLocation(_newLocation);

    _window->updateNodeFields();
}

int PositionNodeCommand::id() const
{
    return 'P';
}

bool PositionNodeCommand::mergeWith(const QUndoCommand* other)
{
    const PositionNodeCommand* pnc;
    if ((pnc = dynamic_cast<const PositionNodeCommand*>(other))) {
        if (pnc->_node == _node && pnc->_axis == _axis) {
            _newLocation = pnc->_newLocation;

            return true;
        }
    }

    return false;
}
