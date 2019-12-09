#ifndef SCENEGRAPHWIDGET_H
#define SCENEGRAPHWIDGET_H

#include <QWidget>

#include "widgets/scenewidget/scenewidget.h"
#include "scene/node/node.h"
#include "scenegraphitemmodel.h"

namespace Ui {
class SceneGraphWidget;
}

class SceneGraphWidget : public QWidget
{
        Q_OBJECT

    private:

    public:
        explicit SceneGraphWidget(Sahara::SceneWidget& sceneWidget, QWidget *parent = nullptr);
        ~SceneGraphWidget();

    signals:
        void selectionChanged(Sahara::Node* node);

    private:
        Ui::SceneGraphWidget *ui;
        Sahara::SceneWidget& _sceneWidget;

        SceneGraphItemModel* _treeModel;
        Sahara::Node* _selectedNode;

        QMenu _addMenu;

        void initAddMenu();

    private slots:
        void sceneUpdated();
        void treeViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
        void addPushButtonClicked();
        void addCameraRequested();
        void addPointLightRequested();
        void addModelRequested();
        void deleteItemRequested(const QModelIndex& index);
        void eyeButtonClicked();
        void flyThroughButtonClicked();
};

#endif // SCENEGRAPHWIDGET_H
