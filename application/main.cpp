#include "window.h"
#include "global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("MX-Linux");
    QApplication::setOrganizationDomain("MX-Linux");
    QApplication::setApplicationName(NAME_BIN);
    QApplication::setApplicationVersion("1.0.0");
    Window w;
    w.show();

    return a.exec();
}
