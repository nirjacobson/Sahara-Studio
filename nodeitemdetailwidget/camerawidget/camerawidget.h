#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>

#include <Sahara/scene/asset/camera.h>
#include "commands/updatecameracommand.h"

class MainWindow;

namespace Ui {
class CameraWidget;
}

class CameraWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit CameraWidget(MainWindow* window, QWidget *parent = nullptr);
        ~CameraWidget();

        void setCamera(Sahara::Camera* camera);

    public slots:
        void updateFields();

    private:
        MainWindow* _window;
        Ui::CameraWidget *ui;

        Sahara::Camera* _camera;

        void populateFieldsFromCamera();

    private slots:
        void perspectiveRadioButtonClicked();
        void orthographicRadioButtonClicked();
        void aspectChanged(double value);
        void xfovChanged(double value);
        void xmagChanged(double value);
        void znearChanged(double value);
        void zfarChanged(double value);
};

#endif // CAMERAWIDGET_H
