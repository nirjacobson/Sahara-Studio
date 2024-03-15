#include "addscenegraphitemcommand.h"

AddSceneGraphItemCommand::AddSceneGraphItemCommand(MainWindow* window, SceneGraphItemModel* itemModel, const QModelIndex& index, const QString& name, Sahara::NodeItem* item)
    : _window(window)
    , _itemModel(itemModel)
    , _index(index)
    , _name(name)
    , _item(item)
    , _node(nullptr)
    , _undone(false)
{
    setText("add item");
}

AddSceneGraphItemCommand::~AddSceneGraphItemCommand()
{
    if (_undone) delete _node;
}

void AddSceneGraphItemCommand::undo()
{
    _itemModel->removeItem(_index, _node);
    _undone = true;
}

void AddSceneGraphItemCommand::redo()
{
    if (_node) {
        _itemModel->addItem(_index, _node);
    } else {
        _node = _itemModel->addItem(_index, _name, _item);
    }
    _undone = false;
}
