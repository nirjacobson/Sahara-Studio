#include "linkwidget.h"

LinkWidget::LinkWidget(QWidget* parent)
    : QLabel(parent)
{
    setStyleSheet("QLabel { color: blue; text-decoration: underline; }");
    setCursor(Qt::PointingHandCursor);
}

void LinkWidget::mousePressEvent(QMouseEvent* e)
{
    QLabel::mousePressEvent(e);

    emit clicked();
}
