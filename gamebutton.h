#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include "gamepanel.h"

class GameButton : public GamePanel
{
public:
  GameButton(GamePanel *parent, const QRect &rect, const QString &text);
  virtual ~GameButton() {}

  void setText(const QString &text) { myText = text; }

  void setTextColor(QColor color) { myTextColor = color; }
  void setTextHoverColor(QColor color) { myTextHoverColor = color; }

  void setFont(QFont *font) { myFont = font; }

  void setPicture(QPixmap *pm) { myPicture = pm; }

  virtual void paintContent(QPainter &p);

protected:
  QString myText;
  QColor myTextColor, myTextHoverColor;
  QFont *myFont;
  QPixmap *myPicture;
};

#endif // GAMEBUTTON_H
