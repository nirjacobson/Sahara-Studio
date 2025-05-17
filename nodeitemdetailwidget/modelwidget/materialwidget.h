#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <QWidget>
#include <QFileInfo>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>

#include <Sahara/scene/asset/material.h>
#include "colorwidget/colorwidget.h"
#include "linkwidget/linkwidget.h"
#include "commands/updatemodelmaterialcommand.h"

class MainWindow;

namespace Ui {
class MaterialWidget;
}

class MaterialWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit MaterialWidget(QWidget *parent = nullptr);
        ~MaterialWidget();

        void setWindow(MainWindow* window);

        void setMaterial(Sahara::Material* material);

        void updateFields();

    private:
        MainWindow* _window;
        Ui::MaterialWidget *ui;

        Sahara::Material* _material;

        void populateFieldsFromMaterial();

    private slots:

        void emissionChanged(const QColor& color);
        void ambientChanged(const QColor& color);
        void diffuseChanged(const QColor& color);
        void specularChanged(const QColor& color);
        void shininessChanged(double value);


        void imageFileLinkClicked();
};

#endif // MATERIALWIDGET_H
