#ifndef HINTCONTROL_H
#define HINTCONTROL_H

#include <QGraphicsScene>
#include <QRect>
#include <QPixmap>
#include <QPoint>

#include "scaler.h"

////////////////////////////////////////////////////////////////////////////////

struct HintInfo
{
  HintInfo() : active(true), blocking(false)
  {
  }

  QPoint position;
  QSize size;
  QString content;
  bool active, blocking;
};

////////////////////////////////////////////////////////////////////////////////

class HintPainter
{
public:
  HintPainter(const HintInfo &hint);
  virtual ~HintPainter() {}

  virtual void paint(QPainter &p);

  inline bool isBlocking() const { return myBlock; }

  inline QRect rect() { return QRect(DPOINT(myPos), myPixmap.size()); }

protected:
  QPixmap myPixmap;
  QPoint myPos;

  qreal myOpacity;

  bool myBlock;
};

////////////////////////////////////////////////////////////////////////////////

class HintControl : public QObject
{
public:
    HintControl(QGraphicsScene *parent);
    virtual ~HintControl() {}

    virtual void scheduleHint(const HintInfo &hint);

    virtual bool execute();
    virtual void init();

    virtual void paint(QPainter &p);

    virtual bool mouseClick(QPoint pos, int buttons);

protected:
    QList<class HintPainter*> hints, activeHints;
    bool block;

    QGraphicsScene *scene;
};

////////////////////////////////////////////////////////////////////////////////

#endif // HINTCONTROL_H
