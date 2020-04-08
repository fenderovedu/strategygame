#include "game.h"

Game::Game(QWidget *parent) : QWidget(parent)
{

}

void Game::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
