#include "application.h"

const QString Application::Organization = "Nir Jacobson";
const QString Application::ApplicationName = "Sahara Studio";
const QString Application::SettingsFile = "settings.ini";

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
{

}

QUndoStack& Application::undoStack() {
    return _undoStack;
}
