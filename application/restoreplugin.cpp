#include "restoreplugin.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDialog>
#include <QComboBox>
#include <QFileInfo>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QSizePolicy>
#include <QLabel>

RestorePlugin::RestorePlugin()
    : Plugin("Restore", Plugin::DisplayFormat_Single)
{
    button = new QPushButton("Restore");
    widget()->setLayout(new QVBoxLayout);
    widget()->layout()->addWidget(button);
    connect(button, &QPushButton::clicked, this, &RestorePlugin::restore);
}

RestorePlugin::~RestorePlugin()
{
    delete button;
}

QString RestorePlugin::exec(const QString source, const QString bashrcSource)
{
    return source;
}

void RestorePlugin::restore()
{
    QDir restoreFolder(QDir::homePath() + "/.config/mx-bashrc-config/saved/");
    QStringList fileNames = restoreFolder.entryList(QDir::Files);
    QStringList names = fileNames;
    for(QString fileName : fileNames)
    {
        fileName.prepend(QDir::homePath() + "/.config/mx-bashrc-config/saved/");
    }
    QStringList datesAndTimes;
    for(QString fileName : fileNames)
    {
#define INFO fileInfo.created()
        QFileInfo fileInfo = QFileInfo(QFile(fileName));
        if(!INFO.isValid()) DEBUG << "Invaild creation";
        QString dateAndTime = QString("%1:%2:%3, %4:%5:%6")
                .arg(INFO.date().month())
                .arg(INFO.date().year())
                .arg(INFO.date().day())
                .arg(INFO.time().hour())
                .arg(INFO.time().minute())
                .arg(INFO.time().second());
        datesAndTimes.append(dateAndTime);
    }
    QDialog dialog(widget());
    dialog.setLayout(new QVBoxLayout);
    QComboBox comboBox;
    for (QString dateAndTime : datesAndTimes) {
        static int times = 0;
        comboBox.addItem(dateAndTime, QVariant(times));
        times++;
    }
    dialog.layout()->addWidget(new QLabel("Select Version to Restore"));
    dialog.layout()->addWidget(&comboBox);
    dialog.setMinimumSize(QSize(comboBox.minimumWidth()+30, comboBox.minimumHeight()+30));
    //dynamic_cast<QVBoxLayout*>(dialog.layout())->addStretch();
    dialog.exec();
    if(dialog.result() == QDialog::Rejected) return;
    QString userChoosenFileName = fileNames.at(comboBox.currentData().toInt());
    QFile restore(userChoosenFileName);
    if(!restore.open(QFile::Text | QFile::ReadOnly))
    {
        //error
        return;
    }
    QFile bashrc(BASHRC);
    if(!bashrc.open(QFile::Text | QFile::WriteOnly))
    {
        //error
        return;
    }
    QTextStream bashrcStream(&bashrc);
    QTextStream restoreStream(&restore);
    bashrc.resize(0);
    bashrcStream << restoreStream.readAll();
    bashrc.close();
    restore.close();

}
