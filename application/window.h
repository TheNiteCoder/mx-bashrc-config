#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "bashrcparser.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();
    void setupAliasTable();
private:
    Ui::Window *ui;
    BashrcParser* parser;
};

#endif // WINDOW_H
