#include "progressbar.h"

#include "scaler.h"

ProgressBar::ProgressBar(const QRect &pos) :
    myMax(100), myVal(0), myDelta(0),
    myAutoMax(false),
    myRect(pos),
    myUpSpeed(0), myUpCount(0), myUpValue(0)
{
  setBrush(Qt::blue);
}

void ProgressBar::setMax(double max)
{
  if (myMax == max) return;

  myMax = max;

  if (myVal > myMax)
  {
    if (myAutoMax)
      myMax = myVal;
    else
      myVal = myMax;
  }
}

void ProgressBar::setValue(double value)
{
  if (myVal == value) return;

  myUpValue = myVal;  // myUpValue is the "from" value to slide
  myUpCount = value > myVal ? myUpSpeed : 0;  // no down slide

  myVal = value;

  if (myVal > myMax)
  {
    if (myAutoMax)
      myMax = myVal;
    else
      myVal = myMax;
  }

  //myDelta = (myVal/myMax) * myRect.height();
}

void ProgressBar::setUpSpeed(int speed)
{
  if (myUpSpeed == speed) return;

  myUpSpeed = speed;
}

void ProgressBar::paint(QPainter &p)
{
  QRect baseRect = DRECT(myRect);

  p.setOpacity(0.3);
  p.setPen(QPen(Qt::white, 2));
  p.setBrush(QColor(0x162d50));

  p.drawRoundedRect(baseRect, 5,5);

  if (myVal == 0) return;

  double val/* = myDelta*/;

  if (myUpCount)
  {
    myUpCount--;
    double d = (myVal-myUpValue)/myUpSpeed * (myUpSpeed-myUpCount);
    val = (myUpValue+d)/myMax * myRect.height();
  } else
    val = (myVal/myMax) * myRect.height();

  QRect dataRect(baseRect);
  dataRect.setTop(baseRect.bottom() - DY(val));

  if (myUpCount) {
    if (myUpCount > myUpSpeed/2) // dim
      p.setOpacity((myUpCount-myUpSpeed/2)*0.4/myUpSpeed + 0.4);
    else // sharp
      p.setOpacity((myUpSpeed-myUpCount)*0.4/myUpSpeed + 0.4);
  }
  else
    p.setOpacity(0.8);

//  p.setPen(Qt::NoPen);
//  p.setBrush(myBrush);
//
//  p.drawRect(dataRect.adjusted(2,2,-2,-2));
  p.drawPixmap(dataRect.adjusted(2,2,-2,-2), myBuffer);
}

void ProgressBar::setBrush(const QBrush &brush)
{
  myBrush = brush;

  // pre-render background
  myBuffer = QPixmap(QSize(DX(myRect.width()),1));
  QPainter pnt(&myBuffer);
  pnt.fillRect(myBuffer.rect(), myBrush);
  pnt.end();
}
