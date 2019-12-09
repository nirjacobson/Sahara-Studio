#ifndef NODEDETAILWIDGET_H
#define NODEDETAILWIDGET_H

#include <QWidget>
#include <QtMath>

#include "scene/node/node.h"

namespace Ui {
class NodeDetailWidget;
}

class NodeDetailWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit NodeDetailWidget(QWidget *parent = nullptr);
        ~NodeDetailWidget();

        void setNode(Sahara::Node* node);

    signals:
        void updated();

    private:
        Ui::NodeDetailWidget *ui;

        Sahara::Node* _node;

        bool _lockScale;

        void setTransformFieldsFromNode();

    private slots:
        void nodeUpdated();
        void doubleSpinBoxValueChanged(const double);
        void scaleXDoubleSpinBoxValueChanged(const double value);
        void scaleYDoubleSpinBoxValueChanged(const double value);
        void scaleZDoubleSpinBoxValueChanged(const double value);
        void lockScaleButtonClicked();
};

#endif // NODEDETAILWIDGET_H
