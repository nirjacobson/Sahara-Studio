#include "updatepointlightcommand.h"
#include "mainwindow.h"

UpdatePointLightCommand::UpdatePointLightCommand(MainWindow* window, Sahara::PointLight* pointLight, const QColor& color)
    : _window(window)
    , _pointLight(pointLight)
    , _parameter(Parameter::Color)
    , _colorBefore(pointLight->color())
    , _colorAfter(color)
{
    setText("update point light");
}

UpdatePointLightCommand::UpdatePointLightCommand(MainWindow* window, Sahara::PointLight* pointLight, Parameter parameter, float value)
    : _window(window)
    , _pointLight(pointLight)
    , _parameter(parameter)
    , _valueAfter(value)
{
    setText("update point light");

    switch (_parameter) {
        case Color:
            break;
        case ConstantAttenuation:
            _valueBefore = _pointLight->constantAttenuation();
            break;
        case LinearAttenuation:
            _valueBefore = _pointLight->linearAttenuation();
            break;
        case QuadraticAttenuation:
            _valueBefore = _pointLight->quadraticAttenuation();
            break;
        }
}

void UpdatePointLightCommand::undo()
{
    switch (_parameter) {
        case Color:
            _pointLight->setColor(_colorBefore);
            break;
        case ConstantAttenuation:
            _pointLight->setConstantAttenuation(_valueBefore);
            break;
        case LinearAttenuation:
            _pointLight->setLinearAttenuation(_valueBefore);
            break;
        case QuadraticAttenuation:
            _pointLight->setQuadraticAttenuation(_valueBefore);
            break;
    }

    _window->updatePointLightFields();
}

void UpdatePointLightCommand::redo()
{
    switch (_parameter) {
    case Color:
            _pointLight->setColor(_colorAfter);
            break;
    case ConstantAttenuation:
            _pointLight->setConstantAttenuation(_valueAfter);
            break;
    case LinearAttenuation:
            _pointLight->setLinearAttenuation(_valueAfter);
            break;
    case QuadraticAttenuation:
            _pointLight->setQuadraticAttenuation(_valueAfter);
            break;
    }

    _window->updatePointLightFields();
}

int UpdatePointLightCommand::id() const
{
    return 'P';
}

bool UpdatePointLightCommand::mergeWith(const QUndoCommand* other)
{
    if (_parameter == Parameter::Color) {
        return false;
    }

    const UpdatePointLightCommand* uplc;
    if ((uplc = dynamic_cast<const UpdatePointLightCommand*>(other))) {
        if (uplc->_pointLight == _pointLight && uplc->_parameter == _parameter) {
            _valueAfter = uplc->_valueAfter;
            return true;
        }
    }

    return false;
}
