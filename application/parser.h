#ifndef PARSER_H
#define PARSER_H

#include <QObject>

struct Alias
{
    QString alias, command;
    int pos;
};
class Window;
class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(Window* window);

    QString removeComments(QString str);
    QStringList removeComments(QStringList list);
    QList<Alias> getAliases();
    QStringList toList(QString str);
    QString toStr(QStringList list);
signals:

public slots:
private:
    QString bashrc;
    Window* win;
};

#endif // PARSER_H
