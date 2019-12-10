#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>

#include "scene/asset/camera.h"

namespace Ui {
class CameraWidget;
}

class CameraWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit CameraWidget(QWidget *parent = nullptr);
        ~CameraWidget();

        void setCamera(Sahara::Camera* camera);

    public slots:
        void updateFields();

    private:
        Ui::CameraWidget *ui;

        Sahara::Camera* _camera;

        void populateFieldsFromCamera();

    private slots:
        void perspectiveRadioButtonClicked();
        void orthographicRadioButtonClicked();
        void doubleSpinBoxValueChanged();
};

#endif // CAMERAWIDGET_H
