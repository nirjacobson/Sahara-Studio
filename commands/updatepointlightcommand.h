#ifndef UPDATEPOINTLIGHTCOMMAND_H
#define UPDATEPOINTLIGHTCOMMAND_H

#include <QUndoCommand>

#include "scene/asset/light/pointlight.h"

class MainWindow;

class UpdatePointLightCommand : public QUndoCommand
{
public:
    enum Parameter {
        Color,
        ConstantAttenuation,
        LinearAttenuation,
        QuadraticAttenuation
    };

    UpdatePointLightCommand(MainWindow* window, Sahara::PointLight* pointLight, const QColor& color);
    UpdatePointLightCommand(MainWindow* window, Sahara::PointLight* pointLight, Parameter parameter, float value);

private:
    MainWindow* _window;
    Sahara::PointLight* _pointLight;
    Parameter _parameter;

    QColor _colorBefore;
    QColor _colorAfter;

    float _valueBefore;
    float _valueAfter;

    // QUndoCommand interface
public:
    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand* other);
};

#endif // UPDATEPOINTLIGHTCOMMAND_H
