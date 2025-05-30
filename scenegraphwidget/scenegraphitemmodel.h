#ifndef SCENEGRAPHITEMMODEL_H
#define SCENEGRAPHITEMMODEL_H

#include <QAbstractItemModel>
#include <QIcon>

#include <Sahara/scene/scene.h>
#include <Sahara/scene/node/node.h>
#include <Sahara/scene/node/nodeitem.h>

class SceneGraphItemModel : public QAbstractItemModel
{
    public:
        SceneGraphItemModel(Sahara::Scene& scene, QObject* parent = nullptr);

    public:
        QModelIndex index(int row, int column, const QModelIndex& parent) const override;
        QModelIndex parent(const QModelIndex& child) const override;
        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex&) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int) override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        Sahara::Node* addItem(const QModelIndex& index, const QString& name, Sahara::NodeItem* item);
        void addItem(const QModelIndex& index, Sahara::Node* node);
        Sahara::Node* removeItem(const QModelIndex& index);
        void removeItem(const QModelIndex& parent, Sahara::Node* childNode);

        QModelIndex focusIndex() const;

    private:
        Sahara::Scene& _scene;
        QModelIndex findFocusIndex(QModelIndex parent) const;
};

#endif // SCENEGRAPHITEMMODEL_H
