#ifndef CONSTTOOLS_H
#define CONSTTOOLS_H

#include "gametools.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class ConstBonus : public GameTool
{
  Q_OBJECT
public:
  ConstBonus(int x, int y, int score, const QString &resource);
  virtual ~ConstBonus() {}

  virtual ToolType type() const { return ConstTool; }

  inline int value() const { return bonusValue; }
  inline void setValue(int val) { bonusValue = val; }

protected:
  QPixmap smallPixmap;
  int bonusValue;
};

////////////////////////////////////////////////////////////////////////////////

class MagBonus : public ConstBonus
{
    Q_OBJECT
public:
    MagBonus(int x, int y, int score);
    virtual ~MagBonus() {}

    virtual bool checkToolClick();
    virtual void advanceAndPaint(QPainter &p, int currentScore);
};

////////////////////////////////////////////////////////////////////////////////

class TimerBonus : public ConstBonus
{
    Q_OBJECT
public:
    TimerBonus(int x, int y, int score);
    virtual ~TimerBonus() {}

    virtual bool checkToolClick();
    virtual void advanceAndPaint(QPainter &p, int currentScore);
};

////////////////////////////////////////////////////////////////////////////////

class ScoreBonus : public ConstBonus
{
    Q_OBJECT
public:
    ScoreBonus(int x, int y, int score);
    virtual ~ScoreBonus() {}

    virtual bool checkToolClick();
    virtual void advanceAndPaint(QPainter &p, int currentScore);
};

////////////////////////////////////////////////////////////////////////////////

class ClockBonus : public ConstBonus
{
    Q_OBJECT
public:
    ClockBonus(int x, int y, int score);
    virtual ~ClockBonus() {}

    virtual bool checkToolClick();
    virtual void advanceAndPaint(QPainter &p, int currentScore);
};

////////////////////////////////////////////////////////////////////////////////

#endif // CONSTTOOLS_H
