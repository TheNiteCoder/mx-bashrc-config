#include "window.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();

    return app.exec();
}
