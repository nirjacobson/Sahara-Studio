#include "removescenegraphitemcommand.h"

RemoveSceneGraphItemCommand::RemoveSceneGraphItemCommand(MainWindow* window, SceneGraphItemModel* itemModel, const QModelIndex& index)
    : _window(window)
    , _itemModel(itemModel)
    , _index(index)
    , _node(nullptr)
{
    setText("remove item");
}

RemoveSceneGraphItemCommand::~RemoveSceneGraphItemCommand()
{
    if (_node) delete _node;
}

void RemoveSceneGraphItemCommand::undo()
{
    _itemModel->addItem(_index.parent(), _node);
    _node = nullptr;
}

void RemoveSceneGraphItemCommand::redo()
{
    _node = _itemModel->removeItem(_index);
}
