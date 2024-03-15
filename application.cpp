#include "application.h"

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
{

}

QUndoStack& Application::undoStack() {
    return _undoStack;
}
