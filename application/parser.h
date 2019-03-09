#ifndef PARSER_H
#define PARSER_H

#include <QObject>

struct Alias
{
    QString alias, command;
};
class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser();

    QString removeComments(QString str);
    QStringList removeComments(QStringList list);
    QList<Alias> getAliases();
    QStringList toList(QString str);
    QString toStr(QStringList list);
signals:
    void showWarning(QString);
public slots:
private:
    QString bashrc;
};

#endif // PARSER_H
