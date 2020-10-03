#ifndef GAMESTOCK_H
#define GAMESTOCK_H

#include <QtGui>

class GameStock;
extern GameStock * gameStock;

class GameStock
{
public:
    GameStock();

    QFont Font8;
    QFont Font12;
    QFont Font16;
    QFont Font20;
    QFont Font40;
    QFont Font60;

    QCursor GameCursor;

    QPixmap GameLogoBig;
    QPixmap MenuBackground;

    QPixmap Concrete;
    QPixmap Target1;
    QPixmap Target2;
    QPixmap Block1;
    QPixmap Block2;

    QPixmap ItemJocker;
    QPixmap ItemFaller;

    QList<QPixmap> Items;

    QPixmap Clock;
    QPixmap Score;

    QPixmap Ok;
    QPixmap Cancel;

    static void init() { gameStock = new GameStock(); }

    void initGraphics();
};

#endif // GAMESTOCK_H
