#include "scenegraphitemmodel.h"

SceneGraphItemModel::SceneGraphItemModel(Sahara::Scene& scene, QObject* parent)
    : QAbstractItemModel (parent)
    , _scene(scene)
{

}

QModelIndex SceneGraphItemModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        return createIndex(row, column, &_scene.root());
    }

    Sahara::Node* parentNode = static_cast<Sahara::Node*>(parent.internalPointer());

    if (row >= parentNode->children())
        return QModelIndex();

    return createIndex(row, column, &parentNode->child(row));
}

QModelIndex SceneGraphItemModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    Sahara::Node* childNode = static_cast<Sahara::Node*>(child.internalPointer());
    Sahara::Node& parentNode = childNode->parent();

    if (childNode == &_scene.root())
        return QModelIndex();

    return createIndex(parentNode.index(), 0, &parentNode);
}

int SceneGraphItemModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return 1;

    Sahara::Node* parentNode = static_cast<Sahara::Node*>(parent.internalPointer());

    return parentNode->children();
}

int SceneGraphItemModel::columnCount(const QModelIndex&) const
{
    return 1;
}

QVariant SceneGraphItemModel::data(const QModelIndex& index, int role) const
{
    if (index.column() != 0)
        return QVariant();

    Sahara::Node* node = static_cast<Sahara::Node*>(index.internalPointer());

    if (node == &_scene.root()) {
        if (role == Qt::DisplayRole) {
            return "Scene";
        } else if (role == Qt::DecorationRole) {
            return QIcon(":/icons/scene.png");
        }
    }

    if (role == Qt::DisplayRole) {
        return node->name();
    }

    if (role == Qt::DecorationRole) {

        if (dynamic_cast<const Sahara::Camera*>(&node->item())) {
            return QIcon(":/icons/camera.png");
        } else if (dynamic_cast<const Sahara::Light*>(&node->item()) ) {
            return QIcon(":/icons/light.png");
        } else if (dynamic_cast<const Sahara::Model*>(&node->item())) {
            return QIcon(":/icons/model.png");
        } else {
            return QIcon();
        }
    }

    return QVariant();
}

bool SceneGraphItemModel::setData(const QModelIndex& index, const QVariant& value, int)
{
    if (index.column() != 0)
        return false;

    Sahara::Node* node = static_cast<Sahara::Node*>(index.internalPointer());
    node->setName(value.toString());

    return true;
}

QVariant SceneGraphItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (section == 0) {
                return "";
            }
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags SceneGraphItemModel::flags(const QModelIndex& index) const
{
    Sahara::Node* node = static_cast<Sahara::Node*>(index.internalPointer());

    if (index.column() == 0) {
        if (node != &_scene.root()) {
            return {
                Qt::ItemIsEnabled,
                Qt::ItemIsSelectable,
                Qt::ItemIsEditable
            };
        }
    }

    return QAbstractItemModel::flags(index);
}

Sahara::Node* SceneGraphItemModel::addItem(const QModelIndex& index, const QString& name, Sahara::NodeItem* item)
{
    Sahara::Node* node = static_cast<Sahara::Node*>(index.internalPointer());

    beginInsertRows(index, node->children(), node->children());

    Sahara::Node* child = new Sahara::Node(name, item, QMatrix4x4());

    node->addChild(child);

    endInsertRows();

    return child;
}

void SceneGraphItemModel::addItem(const QModelIndex& index, Sahara::Node* node) {
    Sahara::Node* parentNode = static_cast<Sahara::Node*>(index.internalPointer());

    beginInsertRows(index, parentNode->children(), parentNode->children());

    parentNode->addChild(node);

    endInsertRows();
}

Sahara::Node* SceneGraphItemModel::removeItem(const QModelIndex& index)
{
    Sahara::Node* node = static_cast<Sahara::Node*>(index.internalPointer());

    beginRemoveRows(index.parent(), node->index(), node->index());

    node->remove();

    endRemoveRows();

    return node;
}

void SceneGraphItemModel::removeItem(const QModelIndex& parent, Sahara::Node* childNode)
{
    beginRemoveRows(parent, childNode->index(), childNode->index());

    childNode->remove();

    endRemoveRows();
}

QModelIndex SceneGraphItemModel::focusIndex() const
{
    return findFocusIndex(index(0, 0, QModelIndex()));
}

QModelIndex SceneGraphItemModel::findFocusIndex(QModelIndex parent) const
{
    Sahara::Node* node = static_cast<Sahara::Node*>(parent.internalPointer());

    if (node && node->hasFocus()) {
        return parent;
    }

    for (int i = 0; i < rowCount(parent); i++) {
        QModelIndex focusIndex = findFocusIndex(index(i, 0, parent));
        if (focusIndex.isValid())
            return focusIndex;
    }

    return QModelIndex();
}

