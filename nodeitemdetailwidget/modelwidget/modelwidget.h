#ifndef MODELWIDGET_H
#define MODELWIDGET_H

#include <QWidget>

#include "scene/model.h"
#include "geometrymodel.h"
#include "geometryitemmodel.h"

namespace Ui {
class ModelWidget;
}

class ModelWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit ModelWidget(QWidget *parent = nullptr);
        ~ModelWidget();

        void setModel(Sahara::Model* model);

    private:
        Ui::ModelWidget *ui;
        QWidget* _animationsTabWidget;

        Sahara::Model* _model;
        GeometryModel* _geometryModel;
        GeometryItemModel* _geometryItemModel;

    private slots:
        void materialsComboBoxCurrentTextChanged(const QString& text);
        void animationsComboBoxCurrentTextChanged(const QString& text);

};

#endif // MODELWIDGET_H
