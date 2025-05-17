#ifndef UPDATESCENEAMBIENTLIGHTCOMMAND_H
#define UPDATESCENEAMBIENTLIGHTCOMMAND_H

#include <QUndoCommand>
#include <Sahara/scene/scene.h>
#include <Sahara/scene/asset/light/ambientlight.h>

class MainWindow;

class UpdateSceneAmbientLightCommand : public QUndoCommand
{
public:
    enum Parameter {
        Color,
        Strength
    };

    UpdateSceneAmbientLightCommand(MainWindow* window, Sahara::Scene* scene, const QColor& ambientColor);
    UpdateSceneAmbientLightCommand(MainWindow* window, Sahara::Scene* scene, float ambientStrength);

private:
    MainWindow* _window;
    Sahara::Scene* _scene;
    Parameter _parameter;

    QColor _ambientColorBefore;
    QColor _ambientColorAfter;

    float _ambientStrengthBefore;
    float _ambientStrengthAfter;

    // QUndoCommand interface
public:
    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand* other);
};

#endif // UPDATESCENEAMBIENTLIGHTCOMMAND_H
