#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QColor>
#include <QColorDialog>
#include <QPainter>

class ColorWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit ColorWidget(QWidget *parent = nullptr);

        const QColor& value() const;
        void setValue(const QColor& color);

    signals:
        void valueChanged(const QColor& color);

    protected:
        void mousePressEvent(QMouseEvent* event);
        void paintEvent(QPaintEvent* event);

    private:
        QColor _color;
        QColorDialog _colorDialog;

    private slots:
        void colorSelected(const QColor& color);
};

#endif // COLORWIDGET_H
