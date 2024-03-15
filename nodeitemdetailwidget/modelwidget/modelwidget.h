#ifndef MODELWIDGET_H
#define MODELWIDGET_H

#include <QWidget>

#include "scene/model.h"
#include "geometrymodel.h"
#include "geometryitemmodel.h"
#include "commands/setmodelanimationcommand.h"

class MainWindow;

namespace Ui {
class ModelWidget;
}

class ModelWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit ModelWidget(MainWindow* window, QWidget *parent = nullptr);
        ~ModelWidget();

        void setModel(Sahara::Model* model);

        void updateFields();

    private:
        MainWindow* _window;
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
