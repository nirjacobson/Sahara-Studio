#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QVector2D>

#include <Sahara/scene/scene.h>

class Tool : public QObject
{
    Q_OBJECT

    public:
        Tool(Sahara::Scene& scene, QObject* parent=nullptr);
        virtual ~Tool();

        virtual void mouseMoved(const QVector2D& ndc) = 0;
        virtual void mousePressed(const QVector2D& ndc) = 0;

        void setScene(Sahara::Scene& scene);

    signals:
        void released();

    protected:
        Sahara::Scene& scene();

    private:
        Sahara::Scene* _scene;
};

#endif // TOOL_H
