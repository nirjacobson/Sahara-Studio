#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "widgets/scenewidget/scenewidget.h"
#include "scenegraphwidget/scenegraphwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        SceneGraphWidget* _sceneGraphWidget;

        Sahara::Node* _selectedNode;

    private slots:
        void sceneWidgetInitialized();
        void sceneGraphSelectionChanged(Sahara::Node* node);
};

#endif // MAINWINDOW_H
