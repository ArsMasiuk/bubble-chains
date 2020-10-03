#include "baseitem.h"

BaseItem::BaseItem(qreal opacity, int steps) :
    myOpacity(opacity),
    myStep(steps)
{
  op_step = myOpacity/(double)steps;
}


////////////////////////////////////////////////////////////////////////////////

PixmapItem::PixmapItem(int x, int y, int dx, int dy, const QPixmap &pm, int steps) :  BaseItem(1, steps),
    myX(x), myY(y),
    myDx(dx), myDy(dy),
    myPixmap(pm)
{
}

bool PixmapItem::advance()
{
  if (myStep-- <= 0)
    return false;

  myX += myDx;
  myY += myDy;

  myOpacity -= op_step;

  return true;
}

void PixmapItem::paint(QPainter &painter)
{
  painter.setOpacity(myOpacity);
  painter.drawPixmap(myX, myY, myPixmap);
}

////////////////////////////////////////////////////////////////////////////////

TextItem::TextItem(QRect rect, const QString &text, int textFlags, const QFont &font, QColor color,
         qreal opacity, int staysteps, int steps, int dx, int dy) :
  BaseItem(opacity, steps),
  myRect(rect),
  myText(text),
  myFlags(textFlags),
  myFont(font),
  myColor(color),
  myStaySteps(staysteps),
  myDx(dx), myDy(dy)
{
  op_step = myOpacity/(double)steps;
}

bool TextItem::advance()
{
  if (myStaySteps)
  {
    myStaySteps--;
    return true;
  }

  if (myStep-- <= 0)
    return false;

  myRect.moveLeft(myRect.left() + myDx);
  myRect.moveTop(myRect.top() + myDy);

  myOpacity -= op_step;

  return true;
}

void TextItem::paint(QPainter &painter)
{
  painter.setOpacity(myOpacity);
  painter.setFont(myFont);
  painter.setPen(QPen(myColor));
  painter.drawText(myRect, myFlags, myText);
}

