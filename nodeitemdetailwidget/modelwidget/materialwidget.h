#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <QWidget>
#include <QFileInfo>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>

#include "scene/asset/material.h"
#include "colorwidget/colorwidget.h"
#include "linkwidget/linkwidget.h"

namespace Ui {
class MaterialWidget;
}

class MaterialWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit MaterialWidget(QWidget *parent = nullptr);
        ~MaterialWidget();

        void setMaterial(Sahara::Material* material);

    private:
        Ui::MaterialWidget *ui;

        Sahara::Material* _material;

        void populateFieldsFromMaterial();

    private slots:

        void fieldValueChanged();
        void imageFileLinkClicked();
};

#endif // MATERIALWIDGET_H
