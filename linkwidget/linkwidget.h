#ifndef LINKWIDGET_H
#define LINKWIDGET_H

#include <QLabel>
#include <QObject>

class LinkWidget : public QLabel
{
    Q_OBJECT

    public:
        LinkWidget(QWidget* parent = nullptr);

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent* e);
};

#endif // LINKWIDGET_H
