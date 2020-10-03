#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QtGui>

#include "defines.h"

const int MAX_ITEMS_COUNT     = 6;

const int ITEM_JOCKER         = (1 << 4);
const int ITEM_FALLER         = (1 << 5);

const int ITEM_SCORE          = 1;
const int TARGET_SCORE        = 3;
const int FALLER_SCORE        = 10;


class GameItem
{
  public:
    enum ItemState { IdleState, NotAliveState, DyingState, FallingState, BornState,
                     SelectedState };

    GameItem(quint8 id);
    ~GameItem();

    inline quint8 id() const { return myId; }
    inline void setId(quint8 newid) { myId = newid; }

    inline int dx() const { return myXoff; }
    inline int dy() const { return myYoff; }

    inline qreal opacity() const { return myOpacity; }
    inline void setOpacity(qreal val) { myOpacity = val; }

    void advance();

    void idle();
    inline bool isIdle() const { return myState == IdleState; }

    void die();
    inline bool isDying() const { return myState == DyingState; }

    void scheduleDeath();
    inline bool isNotAlive() const { return myState == NotAliveState; }

    void fall(int dx, int dy);
    inline bool isFalling() const { return myState == FallingState; }

    void born();
    inline bool isBorning() const { return myState == BornState; }

    void select();
    inline bool isSelected() const { return myState == SelectedState; }

    inline const QPixmap& itemPixmap() const { return itemPixmap(myId); }
    static const QPixmap& itemPixmap(int id);

    static int count(int id) { return itemsCount[id]; }
    static void dropCount() { itemsCount.clear(); }

    void draw(QPainter &p, int x, int y);

  protected:

    quint8 myId;
    ItemState myState;

    int myCount;

    int myXoff, myYoff;
    int myDx, myDy;

    qreal myOpacity;

    static QMap<int,int> itemsCount;
};

#endif // GAMEITEM_H
