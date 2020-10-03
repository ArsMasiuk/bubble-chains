#include "gamebutton.h"
#include "scaler.h"
#include "gamestock.h"

GameButton::GameButton(GamePanel *parent, const QRect &rect, const QString &text) :
    GamePanel(parent, rect),
    myText(text),
    myFont(&gameStock->Font20),
    myPicture(0)
{
}

void GameButton::paintContent(QPainter &p)
{
  GamePanel::paintContent(p);

  if (myHover && myPaintHover)
  {
    p.setPen(myTextHoverColor);
  }
  else
    p.setPen(myTextColor);

  if (myFont)
    p.setFont(*myFont);

  QRect r(DRECT(myRect));

  if (myPicture)
  {
    int dy = (r.height() - myPicture->height()) / 2;
    p.drawPixmap(r.topLeft()+QPoint(DX(5),dy), *myPicture);
    r.setLeft(r.left() + DX(10) + myPicture->width());
  }

  p.drawText(r, Qt::AlignCenter, myText);
}

