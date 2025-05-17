#ifndef UPDATEMODELMATERIALCOMMAND_H
#define UPDATEMODELMATERIALCOMMAND_H

#include <QUndoCommand>

#include <Sahara/scene/asset/material.h>

class MainWindow;

class UpdateModelMaterialCommand : public QUndoCommand
{
public:
    enum Parameter {
        EmissionColor,
        AmbientColor,
        DiffuseColor,
        SpecularColor,
        Shininess,
        Image
    };

    UpdateModelMaterialCommand(MainWindow* window, Sahara::Material* material, Parameter parameter, const QColor& value);
    UpdateModelMaterialCommand(MainWindow* window, Sahara::Material* material, float shininess);
    UpdateModelMaterialCommand(MainWindow* window, Sahara::Material* material, const QString& imageURI);

private:
    MainWindow* _window;
    Sahara::Material* _material;
    Parameter _parameter;

    QColor _colorBefore;
    QColor _colorAfter;

    float _valueBefore;
    float _valueAfter;

    QString _uriBefore;
    QString _uriAfter;

    // QUndoCommand interface
public:
    void undo();
    void redo();
    int id() const;
    bool mergeWith(const QUndoCommand* other);
};

#endif // UPDATEMODELMATERIALCOMMAND_H
