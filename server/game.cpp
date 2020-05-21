#include "game.h"

Game::Game(QMap<size_t, QTcpSocket*> mapPlayers):
    players_(mapPlayers),
    curPlayer(players_.begin()),
    status_(NONE) {

    foreach(QTcpSocket* sock, players_.values()) {
        connect(sock, &QTcpSocket::readyRead, this, &Game::slotReadClient);
        connect(sock, &QTcpSocket::stateChanged, this, &Game::slotPlayerStateHandler);
    }
    setUp();
}

Game::~Game() {
    foreach(QTcpSocket* sock, players_.values()) {
        sock->close();
        delete sock;
    }
}

void Game::hasGotPack() {
    if (history_.empty())
        return;
    Package pack = history_.back().getData();

    /*if (pack.error_.isDefined()) {
        //Save error somewhere;
        qDebug() << "Client error";
        status_ = END;
        GameStatusHandler();
    }*/

    if (pack.gameData_.isDefined()) {
        //if (pack.senderId_ ==  curPlayer.key()) {
        send_(nextPlayer(curPlayer).value(), history_.back());
        GameStatusHandler();
        //}
    }

    if (pack.text_.isDefined()) {
        qDebug() << "We get message: " + pack.text_.getText();
        /*if (pack.text_.getDest() == INT_MAX) {
            QMap<size_t, QTcpSocket*> temp(players_);
            multySend_(players_.values(), NetworkPackage(pack.senderId_, TextMessage(INT_MAX, pack.text_.getText())));
            GameStatusHandler();
        } else {
            //send_(players_[pack.text_.getDest()], NetworkPackage(pack.senderId_, TextMessage(pack.text_.getDest(), pack.text_.getText())));
        }*/
    }
}

void Game::setUp() {
    status_ = GAMING;
    GameStatusHandler();
}

QMap<size_t, QTcpSocket*>::iterator Game::nextPlayer(QMap<size_t, QTcpSocket*>::iterator curPlayer) {
    curPlayer++;
    return (curPlayer == players_.end() ? players_.begin() : curPlayer);
}

void Game::GameStatusHandler() {
    switch(status_) {
        case GameStatus::GAMING: {
            qDebug() << "Gaming mode";
            curPlayer = nextPlayer(curPlayer);
            send_(curPlayer.value(), NetworkPackage(0, TextMessage(curPlayer.key(), "Your turn")));
            send_(nextPlayer(curPlayer).value(), NetworkPackage(0, TextMessage(nextPlayer(curPlayer).key(), "Wait")));
            break;
        }
        case GameStatus::NONE:
            qDebug() << "None-state, you're do somtheng wrong";
            break;
        case GameStatus::END:
            qDebug() << "Game over";
            emit signalGameOver();
            break;
    }
}

void Game::slotPlayerStateHandler() {
    QTcpSocket* sock = static_cast<QTcpSocket*>(QObject::sender());
    qDebug() << "Status of player with id " + QString::number(sock->socketDescriptor()) + " is " + sock->state();
}
