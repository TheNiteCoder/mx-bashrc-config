#ifndef CONFIG_H
#define CONFIG_H
#include <QDebug>
#include <QDir>
#define BASHRC QDir::homePath() + "/.bashrc"
#define APP_NAME QString("MX Bashrc")
#define DEBUG_ACTIVE true
#define DEBUG if(DEBUG_ACTIVE) //put code in {} and change to false if you want debug off
#define DEBUG_MSG if(DEBUG_ACTIVE) qDebug()
#define VERSION "0.1"
#endif // CONFIG_H
