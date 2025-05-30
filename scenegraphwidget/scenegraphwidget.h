#ifndef SCENEGRAPHWIDGET_H
#define SCENEGRAPHWIDGET_H

#include <QWidget>
#include <QItemSelection>
#include <QFileDialog>
#include <QMenu>

#include <Sahara/widgets/scenewidget/scenewidget.h>
#include <Sahara/scene/node/node.h>
#include "scenegraphitemmodel.h"
#include "commands/addscenegraphitemcommand.h"
#include "commands/removescenegraphitemcommand.h"

namespace Ui {
class SceneGraphWidget;
}

class SceneGraphWidget : public QWidget
{
        Q_OBJECT

    private:

    public:
        explicit SceneGraphWidget(MainWindow* window, Sahara::SceneWidget& sceneWidget, QWidget *parent = nullptr);
        ~SceneGraphWidget();

        void reset();

    public slots:
        void sceneUpdated();

    signals:
        void selectionChanged(Sahara::Node* node);

    private:
        MainWindow* _window;
        Ui::SceneGraphWidget *ui;
        Sahara::SceneWidget& _sceneWidget;

        SceneGraphItemModel* _treeModel;
        Sahara::Node* _selectedNode;

        QMenu _addMenu;

        void initAddMenu();

    private slots:
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
