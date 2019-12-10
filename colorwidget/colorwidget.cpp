#include "colorwidget.h"

ColorWidget::ColorWidget(QWidget *parent) : QWidget(parent)
  , _colorDialog(this)
{

}

const QColor&ColorWidget::value() const
{
    return _color;
}

void ColorWidget::setValue(const QColor& color)
{
    _color = color;
    repaint();

    emit valueChanged(color);
}

void ColorWidget::mousePressEvent(QMouseEvent* event)
{
    _colorDialog.open(this, SLOT(colorSelected(const QColor&)));

    QWidget::mousePressEvent(event);
}

void ColorWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;

    pen.setColor(QColor(_color.red() * 3/4, _color.green() * 3/4, _color.blue() * 3/4));
    pen.setWidth(4);

    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(_color);

    painter.setPen(pen);

    painter.fillRect(rect(), brush);
    painter.drawRect(rect());
}

void ColorWidget::colorSelected(const QColor& color)
{
    _color = color;
    repaint();

    emit valueChanged(color);
}
