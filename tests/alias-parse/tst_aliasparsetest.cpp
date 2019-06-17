#include <QString>
#include <QtTest>
#include <QDebug>

/*
 * This unit test doesn't make full use of the classes provided with the application
 * This means that there can't be to extenive testing be done with this unit test
 */

#define DEBUG_WATCH(x) qDebug() << #x << " = " << x

class AliasParseTest : public QObject
{
    Q_OBJECT

public:
    AliasParseTest();

private Q_SLOTS:
    void testCase1();
};

AliasParseTest::AliasParseTest()
{
}

void AliasParseTest::testCase1()
{
    QString data = "alias a='hello'\n"
                   "echo Hello\n"
                   "alias b=\"hello\"";

    struct Alias
    {
        QString alias;
        QString command;
    };

    QList<Alias> aliases;

    for(const QString str : data.split('\n'))
    {
        QString line = str;
        if(line.contains('#'))
        {
            line = line.mid(0, line.indexOf('#')-1);
            DEBUG_WATCH(line);
        }
        while(true)
        {
            if(!line.contains("alias"))
            {
                break;
            }
            Alias alias;
            int aliasStart = line.indexOf(QRegularExpression("\\S"), line.indexOf("alias") + 5);
            int aliasEnd = line.indexOf('=', aliasStart)-1;
            alias.alias = line.mid(aliasStart, aliasEnd - aliasStart);
        }
    }
}

QTEST_APPLESS_MAIN(AliasParseTest)

#include "tst_aliasparsetest.moc"
