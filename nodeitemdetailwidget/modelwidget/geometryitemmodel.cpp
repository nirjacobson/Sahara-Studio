#include "geometryitemmodel.h"

GeometryItemModel::GeometryItemModel(GeometryModel& model, QObject *parent)
    : QAbstractItemModel(parent)
    , _model(model)
{
}

QVariant GeometryItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (section == 0) {
                return "Geometry";
            }
            if (section == 1) {
                return "Triangles";
            }
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

QModelIndex GeometryItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, _model.root());
    }

    GeometryModel::Node* parentNode = static_cast<GeometryModel::Node*>(parent.internalPointer());

    if (row >= parentNode->count())
        return QModelIndex();

    return createIndex(row, column, parentNode->child(row));
}

QModelIndex GeometryItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    GeometryModel::Node* childNode = static_cast<GeometryModel::Node*>(index.internalPointer());
    GeometryModel::Node* parentNode = childNode->parent();

    if (childNode == _model.root())
        return QModelIndex();

    return createIndex(parentNode->index(), 0, parentNode);
}

int GeometryItemModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 1;

    GeometryModel::Node* parentNode = static_cast<GeometryModel::Node*>(parent.internalPointer());

    return parentNode->count();
}

int GeometryItemModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant GeometryItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.column() == 1 && role == Qt::TextAlignmentRole) {
        return Qt::AlignRight;
    }

    GeometryModel::Node* node = static_cast<GeometryModel::Node*>(index.internalPointer());

    GeometryModel::ModelNode* modelNode;
    GeometryModel::MeshNode* meshNode;
    GeometryModel::SurfaceNode* surfaceNode;
    if ((modelNode = dynamic_cast<GeometryModel::ModelNode*>(node))) {
        if (index.column() == 0) {
            if (role == Qt::DisplayRole) {
                return "Model";
            }
            if (role == Qt::DecorationRole) {
                return QIcon(":/icons/model.png");
            }
        } else if (index.column() == 1) {
            if (role == Qt::DisplayRole) {
                return modelNode->model()->triangles();
            }
        }
    } else if ((meshNode = dynamic_cast<GeometryModel::MeshNode*>(node))) {
        if (index.column() == 0) {
            if (role == Qt::DisplayRole) {
                return "Mesh";
            }
            if (role == Qt::DecorationRole) {
                return QIcon(":/icons/mesh.png");
            }
        } else if (index.column() == 1) {
            if (role == Qt::DisplayRole) {
                return meshNode->mesh()->triangles();
            }
        }
    } else if ((surfaceNode = dynamic_cast<GeometryModel::SurfaceNode*>(node))) {
        if (index.column() == 0) {
            if (role == Qt::DisplayRole) {
                return "Surface";
            }
            if (role == Qt::DecorationRole) {
                return QIcon(":/icons/surface.png");
            }
        } else if (index.column() == 1) {
            if (role == Qt::DisplayRole) {
                return surfaceNode->surface()->triangles();
            }
        }
    }

    return QVariant();
}
