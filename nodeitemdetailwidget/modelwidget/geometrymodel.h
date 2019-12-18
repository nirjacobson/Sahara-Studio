#ifndef GEOMETRYMODEL_H
#define GEOMETRYMODEL_H

#include <QList>

#include "scene/model.h"

class GeometryModel
{
    public:
        class Node {
            public:
                Node();
                virtual ~Node();

                Node* parent();
                int index() const;
                int count() const;
                Node* child(const int index);
                void addChild(Node* child);

            private:
                Node* _parent;
                int _index;
                QList<Node*> _children;
        };

        class MeshNode : public Node {
            public:
                MeshNode(const Sahara::Mesh* mesh);

                const Sahara::Mesh* mesh() const;

            private:
                const Sahara::Mesh* _mesh;
        };

        class SurfaceNode : public Node {
            public:
                SurfaceNode(const Sahara::Surface* surface);

                const Sahara::Surface* surface() const;

            private:
                const Sahara::Surface* _surface;
        };

        class ModelNode : public Node {
            public:
                ModelNode(const Sahara::Model* model);

                const Sahara::Model* model() const;

            private:
                const Sahara::Model* _model;
        };

        GeometryModel(const Sahara::Model& model);

        Node* root();

    private:
        ModelNode _root;
};

#endif // GEOMETRYMODEL_H
