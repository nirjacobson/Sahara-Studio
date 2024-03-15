#include "application.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    a.setStyle("fusion");

    MainWindow w(nullptr, &a);
    w.show();

    return a.exec();
}
