#ifndef ADDSCENEGRAPHITEMCOMMAND_H
#define ADDSCENEGRAPHITEMCOMMAND_H

#include <QUndoCommand>

#include "scenegraphwidget/scenegraphitemmodel.h"

class MainWindow;

class AddSceneGraphItemCommand : public QUndoCommand
{
public:
    AddSceneGraphItemCommand(MainWindow* window, SceneGraphItemModel* itemModel, const QModelIndex& index, const QString& name, Sahara::NodeItem* item);
    ~AddSceneGraphItemCommand();

private:
    MainWindow* _window;
    SceneGraphItemModel* _itemModel;
    QModelIndex _index;
    QString _name;
    Sahara::NodeItem* _item;
    Sahara::Node* _node;
    bool _undone;

    // QUndoCommand interface
public:
    void undo();
    void redo();
};

#endif // ADDSCENEGRAPHITEMCOMMAND_H
