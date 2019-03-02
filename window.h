#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "config.h"
#include <QFile>

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

    //Enums
    enum ColorPolicy
    {
        NoColor,
        ForceColor,
        Default,
    };
    //Typedefs
    typedef QList<QPair<QString, QString>> Aliases;

    //Rtn Helpers
    Aliases getAliases();
    Aliases getAliasesFromTable();
    QList<int> getAliasLines();
    //Helpers(mostly for readability)
    void setupConfig(); //sets up the settings so they are the same as the ones in the bashrc
    void connectAll();
    void setupPromptTokens();
    void otherSetup();

    //Event Handlers
    void closeEvent(QCloseEvent *);
private slots:

    void help(); //slot for displaying help
    void about(); //slot for displaying about
    void reformat(); //reformats and bkup ~/.bashrc to ~/.bashrc.bkup
    void apply(); //applies current config to ~/.bashrc
    void onAliasButtonAdd(); //slot for ui->pushButton_plus
    void onAliasButtonRemove(); //slot for ui->pushButton_minus
    void onPromptButtonAdd();
    void onPromptButtonRemove();
    void restore();
private:
    Ui::Window *ui; //Ui pointer
};

#endif // WINDOW_H
