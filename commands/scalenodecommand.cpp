#include "scalenodecommand.h"
#include "mainwindow.h"

ScaleNodeCommand::ScaleNodeCommand(MainWindow* window, Sahara::Node* node, Axis axis, bool lockAxes, float value)
    : _window(window)
    , _node(node)
    , _axis(axis)
    , _lockAxes(lockAxes)
    , _oldScale(_node->scale())
{
    setText("scale node");

    _newScale = _oldScale;

    if (_lockAxes) {
        _newScale.setX(value);
        _newScale.setY(value);
        _newScale.setZ(value);
    } else {
        switch (axis) {
            case X:
                _newScale.setX(value);
                break;
            case Y:
                _newScale.setY(value);
                break;
            case Z:
                _newScale.setZ(value);
                break;
        }
    }
}

void ScaleNodeCommand::undo()
{
    _node->setScale(_oldScale);

    _window->updateNodeFields();
}

void ScaleNodeCommand::redo()
{
    _node->setScale(_newScale);

    _window->updateNodeFields();
}

int ScaleNodeCommand::id() const
{
    return 'S';
}

bool ScaleNodeCommand::mergeWith(const QUndoCommand* other)
{
    const ScaleNodeCommand* snc;
    if ((snc = dynamic_cast<const ScaleNodeCommand*>(other))) {
        if (snc->_node == _node && snc->_axis == _axis && snc->_lockAxes == _lockAxes) {
            _newScale = snc->_newScale;

            return true;
        }
    }

    return false;
}
