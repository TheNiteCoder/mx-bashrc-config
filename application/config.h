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
#define DO_SEPERATE_FILE 1
#define PROGRAM_BASHRC QDir::homePath() + "/.config/mx-bashrc-config/bashrc.bash"

#endif // CONFIG_H
