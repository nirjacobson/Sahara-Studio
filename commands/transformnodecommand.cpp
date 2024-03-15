#include "transformnodecommand.h"
#include "mainwindow.h"

TransformNodeCommand::TransformNodeCommand(MainWindow* window, Sahara::Node* node, const QMatrix4x4& transform)
    : _window(window)
    , _node(node)
    , _transformBefore(node->transform())
    , _transformAfter(transform)
{
    setText("orient node");
}

void TransformNodeCommand::undo()
{
    _node->setTransform(_transformBefore);
    _window->updateNodeFields();
}

void TransformNodeCommand::redo()
{
    _node->setTransform(_transformAfter);
    _window->updateNodeFields();
}

int TransformNodeCommand::id() const
{
    return 'T';
}

bool TransformNodeCommand::mergeWith(const QUndoCommand* other)
{
    const TransformNodeCommand* tnc;
    if ((tnc = dynamic_cast<const TransformNodeCommand*>(other))) {
        if (tnc->_node == _node) {
            _transformAfter = tnc->_transformAfter;
            return true;
        }
    }

    return false;
}
