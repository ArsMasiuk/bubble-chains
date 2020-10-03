#include "gametargets.h"
#include "gameitem.h"
#include "gamestock.h"

////////////////////////////////////////////////////////////////////////////////

QPixmap* GameBasicTarget::pixOn = 0;
QPixmap* GameBasicTarget::pixOff = 0;

GameBasicTarget::GameBasicTarget() : GameTarget(pixOn, pixOff)
{
}

void GameBasicTarget::init()
{
  GameTarget::init("base", &pixOn, &pixOff);
}

////////////////////////////////////////////////////////////////////////////////

QPixmap* GameNumberTarget::pixOn = 0;
QPixmap* GameNumberTarget::pixOff = 0;

GameNumberTarget::GameNumberTarget(int id) : GameTarget(pixOn, pixOff), myId(id)
{
}

void GameNumberTarget::init()
{
  GameTarget::init("number", &pixOn, &pixOff);
}

void GameNumberTarget::repaint()
{
  GameTarget::repaint();

  QPainter p(&myPixmapBuffer);
  p.setPen(Qt::white);
  p.setFont(gameStock->Font20);
  p.setOpacity(0.8);
  p.drawText(0,0,myPixmapBuffer.width(),myPixmapBuffer.height(),
             Qt::AlignCenter, QString("%1").arg(myId));
  p.end();
}

GameTarget::CheckResult GameNumberTarget::checkChain(int size, int id, int score)
{
  if (size >= myId)
  {
    return GameTarget::checkChain(size,id,score);
  }

  return Ignored;
}

////////////////////////////////////////////////////////////////////////////////

QPixmap* GameColorTarget::pixOn[6] = {0};
QPixmap* GameColorTarget::pixOff[6] = {0};

GameColorTarget::GameColorTarget(int id) :
    GameTarget(pixOn[id], pixOff[id]),
    myId(id)
{
}

void GameColorTarget::init()
{
  GameTarget::init("blue", &pixOn[0], &pixOff[0]);
  GameTarget::init("yellow", &pixOn[1], &pixOff[1]);
  GameTarget::init("red", &pixOn[2], &pixOff[2]);
  GameTarget::init("green", &pixOn[3], &pixOff[3]);
  GameTarget::init("magenta", &pixOn[4], &pixOff[4]);
  GameTarget::init("cyan", &pixOn[5], &pixOff[5]);
}

GameTarget::CheckResult GameColorTarget::checkChain(int size, int id, int score)
{
  if (id == myId)
  {
    return GameTarget::checkChain(size,id,score);
  }

  return Ignored;
}

////////////////////////////////////////////////////////////////////////////////

QPixmap* GameFallTarget::pixOn = 0;
QPixmap* GameFallTarget::pixOff = 0;

GameFallTarget::GameFallTarget() : GameTarget(pixOn, pixOff, FALLER_SCORE)
{
}

GameFallTarget::~GameFallTarget()
{
}

void GameFallTarget::init()
{
  GameTarget::init("faller", &pixOn, &pixOff);
}

GameTarget::CheckResult GameFallTarget::checkChain(int size, int id, int score)
{
  if (id != ITEM_FALLER)
    return Ignored;

  return GameTarget::checkChain(size,id,score);
}
