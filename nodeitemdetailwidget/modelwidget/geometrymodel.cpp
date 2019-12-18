#include "geometrymodel.h"

GeometryModel::Node::Node()
    : _parent(nullptr)
{

}

GeometryModel::Node::~Node()
{
    for (int i = 0; i < _children.size(); i++) {
        delete _children.at(i);
    }
}

GeometryModel::Node* GeometryModel::Node::parent()
{
    return _parent;
}

int GeometryModel::Node::index() const
{
    return _index;
}

int GeometryModel::Node::count() const
{
    return _children.size();
}

GeometryModel::Node* GeometryModel::Node::child(const int index)
{
    return _children.at(index);
}

void GeometryModel::Node::addChild(GeometryModel::Node* child)
{
    child->_parent = this;
    child->_index = _children.size();

    _children.append(child);
}

GeometryModel::GeometryModel(const Sahara::Model& model)
    : _root(&model)
{
    for (Sahara::MeshDict::const_iterator i = model.meshes().begin(); i != model.meshes().end(); i++) {
        MeshNode* meshNode = new MeshNode(i.value());
        for (int j = 0; j < i.value()->count(); j++) {
            meshNode->addChild(new SurfaceNode(&i.value()->surface(j)));
        }
        _root.addChild(meshNode);
    }
}

GeometryModel::Node* GeometryModel::root()
{
    return &_root;
}

GeometryModel::MeshNode::MeshNode(const Sahara::Mesh* mesh)
    : _mesh(mesh)
{

}

const Sahara::Mesh* GeometryModel::MeshNode::mesh() const
{
    return _mesh;
}

GeometryModel::SurfaceNode::SurfaceNode(const Sahara::Surface* surface)
    : _surface(surface)
{

}

const Sahara::Surface* GeometryModel::SurfaceNode::surface() const
{
    return _surface;
}

GeometryModel::ModelNode::ModelNode(const Sahara::Model* model)
    : _model(model)
{

}

const Sahara::Model* GeometryModel::ModelNode::model() const
{
    return _model;
}
