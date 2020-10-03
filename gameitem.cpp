#include "gameitem.h"
#include "gamestock.h"
#include "scaler.h"

QMap<int,int> GameItem::itemsCount;

GameItem::GameItem(quint8 id) :
  myId(id),
  myState(IdleState),
  myXoff(0), myYoff(0),
  myDx(0), myDy(0),
  myOpacity(1)
{
  itemsCount[id] = itemsCount[id]+1;
}

GameItem::~GameItem()
{
  if (itemsCount[myId] > 0)
    itemsCount[myId] = itemsCount[myId]-1;
}

void GameItem::idle()
{
  myState = IdleState;
  setOpacity(1);
  myXoff = myYoff = 0;
  myDx = myDy = 0;
}

void GameItem::die()
{
  myState = DyingState;
  setOpacity(1);
  myCount = 0;
}

void GameItem::scheduleDeath()
{
  myState = NotAliveState;
}

void GameItem::born()
{
  myState = BornState;
  setOpacity(0);
  myCount = 5;
}

void GameItem::fall(int dx, int dy)
{
  myState = FallingState;
  setOpacity(1);
  myDx = dx;
  myDy = dy;
  myXoff = myYoff = 0;
  myCount = 5;
}

void GameItem::select()
{
  myState = SelectedState;
  setOpacity(1);
  myCount = 10;
}

void GameItem::advance()
{
  switch (myState)
  {
    case DyingState:
      myOpacity -= 0.2;
      break;

    case FallingState:
      if (!--myCount)
        idle();
      else
      {
        myXoff = DX(-11)*myDx*myCount;
        myYoff = DY(-11)*myDy*myCount;
      }
      break;

    case BornState:
      myOpacity += 0.2;
      if (!--myCount)
        idle();
      break;

    case SelectedState:
      myOpacity -= 0.05;
      if (!--myCount)
      {
        myOpacity = 1;
        myCount = 10;
      }
      break;

    default: ;
  }
}

const QPixmap& GameItem::itemPixmap(int id)
{
  switch (id)
  {
    case ITEM_JOCKER:
      return gameStock->ItemJocker;
    case ITEM_FALLER:
      return gameStock->ItemFaller;
    default:;
  }

  return gameStock->Items.at(id);
}

void GameItem::draw(QPainter &p, int x, int y)
{
  p.setOpacity(myOpacity);
  p.drawPixmap(x + myXoff, y + myYoff, itemPixmap(myId));
}
