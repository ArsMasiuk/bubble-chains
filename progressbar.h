#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QtGui>

class ProgressBar
{
public:
  ProgressBar(const QRect &pos);

  void setMax(double max);
  void setValue(double value);
  void setAutoMaxAdjust(bool set) { myAutoMax = set; }
  inline void addValue(double value) { setValue(myVal + value); }

  inline double maxValue() const { return myMax; }
  inline double value() const { return myVal; }

  inline bool isFull() const { return myVal >= myMax; }

  void setUpSpeed(int speed);

  void setBrush(const QBrush &brush);

  void paint(QPainter &p);

protected:
  double myMax, myVal, myDelta;
  bool myAutoMax;
  QRect myRect;
  int myUpSpeed, myUpCount, myUpValue;
  QBrush myBrush;

  QPixmap myBuffer;
};

#endif // PROGRESSBAR_H
