#ifndef BASENETWORK_H
#define BASENETWORK_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>

#include <sstream>

#include "networkpackage.h"

class BaseNetwork : public QWidget
{
    Q_OBJECT
public:
    BaseNetwork(QWidget* = nullptr);

protected slots:
    void slotReadClient();

protected:
    void send_(QTcpSocket*, const NetworkPackage&);
    virtual void hasGotPack() = 0;

    QVector <NetworkPackage> history_;
    quint64 nextBlockSize;
};

#endif // BASENETWORK_H
