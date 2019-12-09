#ifndef SCENEGRAPHTREEVIEW_H
#define SCENEGRAPHTREEVIEW_H

#include <QObject>
#include <QTreeView>
#include <QKeyEvent>

class SceneGraphTreeView : public QTreeView
{
    Q_OBJECT

    public:
        explicit SceneGraphTreeView(QWidget* parent = nullptr);


    signals:
        void deleteKeyPressed(const QModelIndex& index);

    protected:
        void keyPressEvent(QKeyEvent* event) override;

    private:


};

#endif // SCENEGRAPHTREEVIEW_H
