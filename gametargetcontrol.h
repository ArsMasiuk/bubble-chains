#ifndef GAMETARGETCONTROL_H
#define GAMETARGETCONTROL_H

#include <QtGui>

#include "scaler.h"

////////////////////////////////////////////////////////////////////////////////

const int TARGET_SIZE = 64;

class GameTarget
{
  friend class GameTargetControl;

public:
  enum CheckResult { Ignored, Used, Completed };

  GameTarget(QPixmap *pixOn, QPixmap *pixOff, int targetScore = 30);
  virtual ~GameTarget();

  virtual void repaint();
  virtual void paint(QPainter &p);

  void setOrder(int order);

  virtual void setPos(const QPoint &coords);
  QRect rect() const { return QRect( QPoint(DX(myX),DY(myY)), myPixmapBuffer.size() ); }
  bool isHover(const QPoint &pos) const;

  virtual CheckResult checkChain(int size, int id, int score);

  virtual QString hint() const { return ""; }

protected:
  static void init(const QString &name,
                   QPixmap **pixOn, QPixmap **pixOff
                   );

  QPixmap *myPixmapOn, *myPixmapOff;

  int myX,myY;
  QPixmap myPixmapBuffer;

  int myScore, myTargetScore;

  struct GameTargetPrivate *priv;
};

////////////////////////////////////////////////////////////////////////////////

class GameTargetControl
{
public:
  enum TargetsType {
    TT_BASE, TT_COLOR, TT_NUMBER, TT_FALLING,
    TT_RANDOM
  };

    GameTargetControl();
    ~GameTargetControl();

    inline int targetsLeft() const { return myTargets.count(); }
    inline int targetsCreated() const { return myTargetsCreated; }

    void initGraphics();

    void createTargets(TargetsType types = TT_RANDOM);

    void checkChain(int size, int id, int score);
    bool checkMouseHover(const QPoint &pos);

    void paint(QPainter &p);

protected:
    void reposite();

    int myTargetsCreated;
    QList<GameTarget*> myTargets;

    struct GameTargetControlPrivate *priv;
};

extern GameTargetControl * targetControl;

////////////////////////////////////////////////////////////////////////////////

#endif // GAMETARGETCONTROL_H
