#ifndef REMOVESCENEGRAPHITEMCOMMAND_H
#define REMOVESCENEGRAPHITEMCOMMAND_H

#include <QUndoCommand>

#include "scenegraphwidget/scenegraphitemmodel.h"

class MainWindow;

class RemoveSceneGraphItemCommand : public QUndoCommand
{
public:
    RemoveSceneGraphItemCommand(MainWindow* window, SceneGraphItemModel* itemModel, const QModelIndex& index);
    ~RemoveSceneGraphItemCommand();

private:
    MainWindow* _window;
    SceneGraphItemModel* _itemModel;
    QModelIndex _index;
    Sahara::Node* _node;

    // QUndoCommand interface
public:
    void undo();
    void redo();
};

#endif // REMOVESCENEGRAPHITEMCOMMAND_H
