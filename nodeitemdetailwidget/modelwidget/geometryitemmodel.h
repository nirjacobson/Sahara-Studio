#ifndef GEOMETRYITEMMODEL_H
#define GEOMETRYITEMMODEL_H

#include <QAbstractItemModel>
#include <QIcon>

#include "geometrymodel.h"

class GeometryItemModel : public QAbstractItemModel
{
        Q_OBJECT

    public:
        explicit GeometryItemModel(GeometryModel& model, QObject *parent = nullptr);

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    private:
        GeometryModel& _model;
};

#endif // GEOMETRYITEMMODEL_H
