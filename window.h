#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "config.h"

namespace Ui {
class Window;
}

struct item {
    enum TokenType {
        HOSTNAME_LONG,
        HOSTNAME_SHORT,
        USERNAME,
        SHELL_NAME,
        TERMINAL,
        DIRECTORY,
        DIRECTORY_BASE,
        TIME_SHORT,
        TIME_SECONDS,
        TIME,
        TIME_SECONDS_12HOURS,
        DATE,
        EXIT_STATUS,
        END_PROMPT_CHAR, //# or $
        CHAR,
    };
    TokenType type;
    //additional data
    char CHAR_char;
    enum ColorType {
        RED_BOLD,
        RED,
        GREEN_BOLD,
        GREEN,
        YELLOW_BOLD,
        YELLOW,
        BLUE_BOLD,
        BLUE,
        MAGENTA_BOLD,
        MAGENTA,
        CYAN_BOLD,
        CYAN,
        GRAY_BOLD,
        GRAY,
        WHITE_BOLD,
        WHITE,
        BLACK,
        BLACK_BOLD,
    };
    ColorType color;
};

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
    QPair<QString,bool> getTokenInfo(QString token);

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
