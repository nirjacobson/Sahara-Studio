#ifndef NODEDETAILWIDGET_H
#define NODEDETAILWIDGET_H

#include <QWidget>
#include <QtMath>

#include "application.h"
#include "scene/node/node.h"
#include "commands/positionnodecommand.h"
#include "commands/rotatenodecommand.h"
#include "commands/scalenodecommand.h"

namespace Ui {
class NodeDetailWidget;
}

class NodeDetailWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit NodeDetailWidget(MainWindow* window, QWidget *parent = nullptr);
        ~NodeDetailWidget();

        void setNode(Sahara::Node* node);

    public slots:
        void updateFields();

    private:
        MainWindow* _window;
        Ui::NodeDetailWidget *ui;

        Sahara::Node* _node;

        bool _lockScale;

        void populateFieldsFromNode();

    private slots:
        void nodeUpdated();
        void doubleSpinBoxValueChanged(const double);
        void loctnXDoubleSpinBoxValueChanged(const double value);
        void loctnYDoubleSpinBoxValueChanged(const double value);
        void loctnZDoubleSpinBoxValueChanged(const double value);
        void rottnXDoubleSpinBoxValueChanged(const double value);
        void rottnYDoubleSpinBoxValueChanged(const double value);
        void rottnZDoubleSpinBoxValueChanged(const double value);
        void scaleXDoubleSpinBoxValueChanged(const double value);
        void scaleYDoubleSpinBoxValueChanged(const double value);
        void scaleZDoubleSpinBoxValueChanged(const double value);
        void lockScaleButtonClicked();
};

#endif // NODEDETAILWIDGET_H
