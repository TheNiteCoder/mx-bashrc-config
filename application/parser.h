#ifndef PARSER_H
#define PARSER_H

#include <QObject>



struct Alias
{
    Alias(QString a, QString c, int p = -1)
    {
        alias = a;
        command = c;
        pos = p;
    }
    Alias(){}
    QString alias, command;
    int pos;
};
class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QWidget *win = nullptr);

    QString removeComments(QString str);
    QStringList removeComments(QStringList list);
    QList<Alias> getAliases();
    int setAlias(Alias a);
    int setAliases(QList<Alias> la);
    Alias getAlias(QString str, bool command = false);
    QStringList toList(QString str);
    QString toStr(QStringList list);
signals:
    void showWarning(QString);
public slots:
private:
    QString bashrc;
    QWidget* window;
};

#endif // PARSER_H
