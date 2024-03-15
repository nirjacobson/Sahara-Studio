#include "updatesceneambientlightcommand.h"
#include "mainwindow.h"

UpdateSceneAmbientLightCommand::UpdateSceneAmbientLightCommand(MainWindow* window, Sahara::Scene* scene, const QColor& ambientColor)
    : _window(window)
    , _scene(scene)
    , _parameter(Parameter::Color)
    , _ambientColorBefore(_scene->ambientLight().color())
    , _ambientColorAfter(ambientColor)
{
    setText("update ambient light");
}

UpdateSceneAmbientLightCommand::UpdateSceneAmbientLightCommand(MainWindow* window, Sahara::Scene* scene, float ambientStrength)
    : _window(window)
    , _scene(scene)
    , _parameter(Parameter::Strength)
    , _ambientStrengthBefore(_scene->ambientLight().strength())
    , _ambientStrengthAfter(ambientStrength)
{
    setText("update ambient light");
}

void UpdateSceneAmbientLightCommand::undo()
{
    switch (_parameter) {
        case Color:
            _scene->ambientLight().setColor(_ambientColorBefore);
            break;
        case Strength:
            _scene->ambientLight().setStrength(_ambientStrengthBefore);
            break;
    }

    _window->updateSceneFields();
}

void UpdateSceneAmbientLightCommand::redo()
{
    switch (_parameter) {
        case Color:
            _scene->ambientLight().setColor(_ambientColorAfter);
            break;
        case Strength:
            _scene->ambientLight().setStrength(_ambientStrengthAfter);
            break;
    }

    _window->updateSceneFields();
}

int UpdateSceneAmbientLightCommand::id() const
{
        return 'A';
}

bool UpdateSceneAmbientLightCommand::mergeWith(const QUndoCommand* other)
{
    if (_parameter == Parameter::Color) {
        return false;
    }

    const UpdateSceneAmbientLightCommand* usalc;
    if ((usalc = dynamic_cast<const UpdateSceneAmbientLightCommand*>(other))) {
        if (usalc->_scene == _scene && usalc->_parameter == _parameter) {
            _ambientStrengthAfter = usalc->_ambientStrengthAfter;
            return true;
        }
    }

    return false;
}

