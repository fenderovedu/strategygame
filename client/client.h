#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QResizeEvent>
#include "source/game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Client *ui;
};
#endif // CLIENT_H
