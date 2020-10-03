#include "gametargetcontrol.h"
#include "gametargets.h"
#include "gamestock.h"
#include "gamesound.h"

#include "scene_if.h"

////////////////////////////////////////////////////////////////////////////////

struct GameTargetPrivate
{
  enum { GT_INIT, GT_APPEAR, GT_ALIVE, GT_DYING, GT_DEAD };

  GameTargetPrivate() : stage(0), state(GT_INIT), order(0), checkpoint(0), dy(0)
  {
  }

  inline void die()
  {
    state = GT_DYING;
  }

  inline bool isDead() const
  {
    return (state == GT_DEAD);
  }

  inline bool isAlive() const
  {
    return (state == GT_ALIVE);
  }

  int stage;
  int state;
  int order;

  int checkpoint;
  int dy;
};

GameTarget::GameTarget(QPixmap *pixOn, QPixmap *pixOff, int targetScore) :
    myPixmapOn(pixOn),
    myPixmapOff(pixOff),
    myScore(0),
    myTargetScore(targetScore)
{
  priv = new GameTargetPrivate;
  repaint();
}

GameTarget::~GameTarget()
{
  delete priv;
}

void GameTarget::setOrder(int order)
{
  priv->order = order;
}

void GameTarget::repaint()
{
  // copy background image
  myPixmapBuffer = *myPixmapOff;

  // copy visible part of 2nd image
  int dy = (float)myScore / (float)myTargetScore * myPixmapBuffer.height();
  int off = myPixmapBuffer.height() - dy;
  QPainter p(&myPixmapBuffer);
  p.setOpacity(1);
  p.drawPixmap(QPoint(0,off), *myPixmapOn, QRect(0,off,myPixmapBuffer.width(),dy));
  p.end();
}

void GameTarget::paint(QPainter &p)
{
  if (priv->state == GameTargetPrivate::GT_INIT)
  {
    myY = 770 + priv->order * 5;
    priv->stage = -priv->order * 5;
    priv->dy = -8;
    priv->checkpoint = 730;
    priv->state = GameTargetPrivate::GT_APPEAR;
  }

  if (priv->state == GameTargetPrivate::GT_APPEAR)
  {
    // check if point reached
    if ((myY <= priv->checkpoint && priv->dy < 0) ||
        (myY >= priv->checkpoint && priv->dy > 0))
    {
      switch (priv->checkpoint)
      {
        case 730:
          priv->checkpoint = 650;
          priv->dy = -4;
          break;

        case 650:
          priv->checkpoint = 700;
          priv->dy = 4;
          break;

        case 700:
          priv->state = GameTargetPrivate::GT_ALIVE;
          myY = 700;
          priv->dy = 0;
          break;
      }
    }

    myY += priv->dy;

    if (priv->stage >= 30)
      p.setOpacity(1);
    else
      p.setOpacity(priv->stage++/30.0);

    p.drawPixmap(DX(myX),DY(myY), myPixmapBuffer);
  }

  if (priv->state == GameTargetPrivate::GT_ALIVE)
  {
    priv->stage = 0;
    p.setOpacity(1);
    p.drawPixmap(DX(myX),DY(myY), myPixmapBuffer);
    return;
  }

  if (priv->state == GameTargetPrivate::GT_DYING)
  {
    if (++priv->stage >= 50)
      priv->state = GameTargetPrivate::GT_DEAD;

    myY += 2;

    p.setOpacity((50.0-priv->stage)/50.0);
    p.drawPixmap(DX(myX),DY(myY), myPixmapBuffer);
    return;
  }
}

void GameTarget::setPos(const QPoint &coords)
{
  myX = coords.x();
  myY = coords.y();
}

void GameTarget::init(const QString &name,
                   QPixmap **pixOn, QPixmap **pixOff
                   )
{
  if (!*pixOn)
    *pixOn = new QPixmap();
  if (!*pixOff)
    *pixOff = new QPixmap();

  int dx = DX(TARGET_SIZE);
  int dy = DY(TARGET_SIZE);

  scene->loadPixmap(**pixOn, QString("target_%1_on.png").arg(name), dx,dy);
  scene->loadPixmap(**pixOff, QString("target_%1_off.png").arg(name), dx,dy);
}

GameTarget::CheckResult GameTarget::checkChain(int /*size*/, int /*id*/, int score)
{
  myScore += score;
  repaint();
  return myScore >= myTargetScore ? Completed : Used;
}

bool GameTarget::isHover(const QPoint &pos) const
{
  return rect().contains(pos);
}

////////////////////////////////////////////////////////////////////////////////

struct GameTargetControlPrivate
{
  GameTargetControlPrivate()
  {
  }
};

GameTargetControl * targetControl;

GameTargetControl::GameTargetControl()
{
  priv = new GameTargetControlPrivate();
}

GameTargetControl::~GameTargetControl() { delete priv; }

void GameTargetControl::initGraphics()
{
  // init static stuff
  GameBasicTarget::init();
  GameNumberTarget::init();
  GameColorTarget::init();
  GameFallTarget::init();

  // repaint current items
  reposite();
}

void GameTargetControl::reposite()
{
//  int y = DY(380);
//  int dy = DY(TARGET_SIZE+10);
//  int x = DX(930);
  int dx = TARGET_SIZE+10;
  int x = (1024 - dx*myTargets.count())/2;
  int y = 700;
  for (int i = 0; i < myTargets.count(); i++)
  {
    myTargets.at(i)->setPos(QPoint(x, y));
    myTargets.at(i)->repaint();
    x += dx;
//    y += dy;
  }
}

void GameTargetControl::createTargets(TargetsType types)
{
  // clean older
  qDeleteAll(myTargets);
  myTargets.clear();

  if (types == TT_RANDOM)
  {
    types = (TargetsType) (qrand() % TT_RANDOM);
  }

  int count = 5;
  myTargetsCreated = count;

//  int max_colors = count/2;
//  int max_numbers = count*2;

  GameBasicTarget *t;

  for (int i = 0; i < count; i++)
  {
    switch (types)
    {
      default:
        t = new GameBasicTarget();
        break;

      case TT_FALLING:
        t = (GameBasicTarget*) new GameFallTarget();
        break;

      case TT_COLOR:
        t = (GameBasicTarget*) new GameColorTarget(qrand() % 6);
        break;

      case TT_NUMBER:
        t = (GameBasicTarget*) new GameNumberTarget(qrand() % 5 + 4);
        break;
    }

    t->setOrder(i);
    myTargets.append(t);

#ifdef AAA
    int type = qrand() % 3;

    if (type == 2)  // number
    {
      int chain = qrand() % 5 + 4; // 4..8
      if (max_numbers-chain < 0)
        type = 0;
      else
      {
        max_numbers-=chain;
        GameNumberTarget *t = new GameNumberTarget(chain);
        myTargets.append(t);
        continue;
      }
    }

    if (type == 1)  // color
    {
      if (max_colors == 0)
        type = 0;
      else
      {
        max_colors--;
        GameColorTarget *t = new GameColorTarget(qrand() % 6);
        myTargets.append(t);
        continue;
      }
    }


    // basic
    GameBasicTarget *t = new GameBasicTarget();
    myTargets.append(t);

#else

    //myTargets.append(new GameFallTarget());

#endif

  }

  reposite();
}

void GameTargetControl::paint(QPainter &p)
{
  p.setOpacity(1);
  for (int i = 0; i < myTargets.count(); i++)
  {
    GameTarget *target = myTargets.at(i);
    target->paint(p);
    if (target->priv->isDead())
    {
      myTargets.removeAt(i--);
      delete target;
    }
  }
}

void GameTargetControl::checkChain(int size, int id, int score)
{
  for (int i = 0; i < myTargets.count(); i++)
  {
    GameTarget *target = myTargets.at(i);
    if (!target->priv->isAlive())
      continue;

    switch (target->checkChain(size,id,score))
    {
      case GameTarget::Completed:
        // kill item
        target->priv->die();
        sndEngine->playSound(GameSound::sndTarget);
        // fall down

      case GameTarget::Used:
        // add score popup
        scene->createStaticPopup(target->rect(),
                          QString::number(score),
                          Qt::AlignCenter,
                          gameStock->Font20,
                          Qt::white,
                          1,
                          0, 50,
                          0, -2);
        return;

      default:;
    }
  }
}

bool GameTargetControl::checkMouseHover(const QPoint &pos)
{
  for (int i = 0; i < myTargets.count(); i++)
  {
    GameTarget *target = myTargets.at(i);
    if (!target->priv->isAlive())
      continue;

    if (target->isHover(pos))
    {
      scene->showHint(target->hint());
      return true;
    }
  }

  return false;
}
