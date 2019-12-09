#include "scenegraphtreeview.h"


SceneGraphTreeView::SceneGraphTreeView(QWidget* parent)
    : QTreeView(parent)
{

}

void SceneGraphTreeView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete) {
        if (!selectionModel()->selectedIndexes().isEmpty()) {
            emit deleteKeyPressed( selectionModel()->selectedIndexes().first() );
        }
    }
}
