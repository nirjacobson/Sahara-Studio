#ifndef POINTLIGHTWIDGET_H
#define POINTLIGHTWIDGET_H

#include <QWidget>

#include "scene/asset/light/pointlight.h"
#include "commands/updatepointlightcommand.h"

class MainWindow;

namespace Ui {
class PointLightWidget;
}

class PointLightWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit PointLightWidget(MainWindow* window, QWidget *parent = nullptr);
        ~PointLightWidget();

        void setPointLight(Sahara::PointLight* pointLight);

    public slots:
        void updateFields();

    private:
        MainWindow* _window;
        Ui::PointLightWidget *ui;

        Sahara::PointLight* _pointLight;

        void populateFieldsFromPointLight();

    private slots:
        void colorChanged(const QColor& color);
        void constantAttenuationChanged(double value);
        void linearAttenuationChanged(double value);
        void quadraticAttenuationChanged(double value);
};

#endif // POINTLIGHTWIDGET_H
