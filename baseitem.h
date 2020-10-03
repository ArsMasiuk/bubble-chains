#ifndef BASEITEM_H
#define BASEITEM_H

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class BaseItem
{
public:
  BaseItem(qreal opacity, int steps);
  virtual ~BaseItem() {}

  virtual bool advance() = 0;

  virtual void paint(QPainter &painter) = 0;

protected:
  qreal myOpacity, op_step;
  int myStep;
};

////////////////////////////////////////////////////////////////////////////////

class PixmapItem : public BaseItem
{
public:
    PixmapItem(int x, int y, int dx, int dy, const QPixmap &pm, int steps);
    virtual ~PixmapItem() {}

    bool advance();

    void paint(QPainter &painter);

protected:
    int myX, myY;
    int myDx, myDy;
    QPixmap myPixmap;
};

////////////////////////////////////////////////////////////////////////////////

class TextItem : public BaseItem
{
public:
    TextItem(QRect rect, const QString &text, int textFlags, const QFont &font, QColor color,
             qreal opacity, int staysteps, int steps, int dx = 0, int dy = 0);
    virtual ~TextItem() {}

    virtual bool advance();

    virtual void paint(QPainter &painter);

protected:
    QRect myRect;
    QString myText;
    int myFlags;
    QFont myFont;
    QColor myColor;
    int myStaySteps;
    int myDx, myDy;
};

////////////////////////////////////////////////////////////////////////////////

#endif // BASEITEM_H
