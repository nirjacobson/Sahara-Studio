#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QUndoStack>

class Application : public QApplication
{
public:
    Application(int &argc, char **argv);

    QUndoStack& undoStack();

private:
    QUndoStack _undoStack;
};

#endif // APPLICATION_H
