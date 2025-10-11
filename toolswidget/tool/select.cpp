#include "select.h"

Select::Select(Sahara::Scene& scene)
    : Tool(scene)
    , _mode(Mode::OBJECTS)
    , _clickedModel(nullptr)
    , _clickedInstance(nullptr)
{

}

Select::~Select()
{

}

void Select::mouseMoved(const QVector2D& ndc)
{
    QVector4D ray = {
        ndc.x(),
        ndc.y(),
        -1,
        1
    };

    ray = scene().camera().projection().inverted().map(ray);
    ray[2] = -1;
    ray[3] = 0;

    ray = scene().cameraNode().globalTransform().map(ray);

    _ray = QVector3D(ray).normalized();
}

void Select::mousePressed(const QVector2D&)
{
    switch (_mode) {
    case OBJECTS:
        mousePressedObjects();
        break;
    case SURFACES:
        mousePressedSurfaces();
        break;
    }
}

void Select::setMode(const Mode mode)
{
    _mode = mode;
}

void Select::select(Sahara::Node *model)
{
    if (_clickedInstance) {
        _clickedInstance->setFocusSurface(-1);
        _clickedInstance = nullptr;
    }
    if (_clickedModel)
        _clickedModel->setFocus(false);
    _clickedModel = model;
    if (_clickedModel)
        _clickedModel->setFocus(true);
}

void Select::select(Sahara::Model *model, Sahara::Instance *instance, const int surfaceIdx)
{
    if (_clickedModel) {
        _clickedModel->setFocus(false);
        _clickedModel = nullptr;
    }
    if (_clickedInstance)
        _clickedInstance->setFocusSurface(-1);
    _clickedInstance = instance;
    if (_clickedInstance)
        _clickedInstance->setFocusSurface(surfaceIdx);

    emit selectedSurface(model, instance, surfaceIdx);
}

void Select::mousePressedObjects()
{
    QVector3D rayPoint = scene().cameraNode().globalPosition();
    QVector3D raySegment = _ray * 0.2f;

    scene().root().depthFirst([](Sahara::Node&){ return false; }, [&](Sahara::Node& node) {
        if (&node != &scene().cameraNode()) {
            rayPoint = scene().cameraNode().globalPosition();
            for (int i = 0; i < 1000; i++) {
                if (node.intersects(
                        node.globalTransform().inverted().map(rayPoint)
                    )) {
                    select(&node);
                    return true;
                }

                rayPoint += raySegment;
            }
        }

        return false;
    });

    emit updatedScene();
}

void Select::mousePressedSurfaces()
{
    QVector3D rayPoint = scene().cameraNode().globalPosition();
    QVector3D raySegment = _ray * 0.2f;

    scene().root().depthFirst([](Sahara::Node&){ return false; }, [&](Sahara::Node& node) {
        if (&node != &scene().cameraNode()) {
            Sahara::Model* model;

            if ((model = dynamic_cast<Sahara::Model*>(&node.item()))) {
                QList<Sahara::Instance*> orderedInstances = model->instances();
                std::sort(orderedInstances.begin(), orderedInstances.end(), [](Sahara::Instance* a, Sahara::Instance* b) {
                    return a->volume().volume() < b->volume().volume();
                });

                rayPoint = scene().cameraNode().globalPosition();
                for (int i = 0; i < 1000; i++) {
                    for (Sahara::Instance* inst : orderedInstances) {
                        Sahara::InstanceMesh* im;
                        Sahara::InstanceController* ic;
                        if ((im = dynamic_cast<Sahara::InstanceMesh*>(inst))) {
                            QList<QPair<Sahara::Surface*, int>> orderedSurfaces;
                            for (int s = 0; s < im->mesh().count(); s++) {
                                orderedSurfaces.append(QPair<Sahara::Surface*, int>(&im->mesh().surface(s), s));
                            }
                            std::sort(orderedSurfaces.begin(), orderedSurfaces.end(), [](QPair<Sahara::Surface*, int>& a, QPair<Sahara::Surface*, int>& b){
                                return a.first->volume().volume() < b.first->volume().volume();
                            });

                            for (int s = 0; s < orderedSurfaces.size(); s++) {
                                if (orderedSurfaces[s].first->intersects(
                                        (node.globalTransform() * (inst->transform())).inverted().map(rayPoint)
                                        )){
                                    select(model, inst, orderedSurfaces[s].second);
                                    return true;
                                }
                            }
                        } else if ((ic = dynamic_cast<Sahara::InstanceController*>(inst))) {
                            QList<QPair<Sahara::Surface*, int>> orderedSurfaces;
                            for (int s = 0; s < ic->controller().mesh().count(); s++) {
                                orderedSurfaces.append(QPair<Sahara::Surface*, int>(&ic->controller().mesh().surface(s), s));
                            }
                            std::sort(orderedSurfaces.begin(), orderedSurfaces.end(), [](QPair<Sahara::Surface*, int>& a, QPair<Sahara::Surface*, int>& b){
                                return a.first->volume().volume() < b.first->volume().volume();
                            });

                            for (int s = 0; s < orderedSurfaces.size(); s++) {
                                if (orderedSurfaces[s].first->intersects(
                                        (node.globalTransform() * (inst->transform())).inverted().map(rayPoint)
                                        )){
                                    select(model, inst, orderedSurfaces[s].second);
                                    return true;
                                }
                            }
                        }
                    }

                    rayPoint += raySegment;
                }
            }
        }

        return false;
    });

    emit updatedScene();
}
