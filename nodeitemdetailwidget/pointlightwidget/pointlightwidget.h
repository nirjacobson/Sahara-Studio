#ifndef POINTLIGHTWIDGET_H
#define POINTLIGHTWIDGET_H

#include <QWidget>

#include "scene/asset/pointlight.h"

namespace Ui {
class PointLightWidget;
}

class PointLightWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit PointLightWidget(QWidget *parent = nullptr);
        ~PointLightWidget();

        void setPointLight(Sahara::PointLight* pointLight);

    public slots:
        void updateFields();

    private:
        Ui::PointLightWidget *ui;

        Sahara::PointLight* _pointLight;

        void populateFieldsFromPointLight();

    private slots:
        void fieldValueChanged();
};

#endif // POINTLIGHTWIDGET_H
