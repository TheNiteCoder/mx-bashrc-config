#ifndef CONFIG_H
#define CONFIG_H

#include <QDebug>
#include <QDir>
#define APPNAME "MX Bashrc Config"
#define MAJOR_VERSION "0"
#define MINOR_VERSION "1"
#define VERSION "0.1"
#define BASHRC QDir::homePath() + "/.bashrc"
#define DEBUG qDebug()

#endif // CONFIG_H
