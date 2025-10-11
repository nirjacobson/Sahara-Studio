#ifndef SELECTWIDGET_H
#define SELECTWIDGET_H

#include <QWidget>

#include "../../toolswidget/tool/select.h"

namespace Ui {
class SelectWidget;
}

class SelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectWidget(QWidget *parent = nullptr);
    ~SelectWidget();

signals:
    void selectionModeChanged(const Select::Mode mode);

private:
    Ui::SelectWidget *ui;

private slots:
    void selectionModeDidChange(const int idx);
};

#endif // SELECTWIDGET_H
