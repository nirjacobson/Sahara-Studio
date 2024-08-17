#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QUndoStack>

class Application : public QApplication
{
public:
    static const QString Organization;
    static const QString ApplicationName;

    Application(int &argc, char **argv);

    QUndoStack& undoStack();

private:
    QUndoStack _undoStack;
};

#endif // APPLICATION_H
