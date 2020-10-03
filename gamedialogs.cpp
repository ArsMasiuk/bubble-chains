#include "gamedialogs.h"

#include "scaler.h"
#include "gamestock.h"
#include "gamebutton.h"

////////////////////////////////////////////////////////////////////////////////

class GradientButton : public GameButton
{
public:
  GradientButton(GamePanel *parent, const QRect &rect, const QString &text) :
      GameButton(parent, rect, text)
  {
    setBackground(Qt::NoBrush);
    setPen(QPen(QColor(Qt::white), 2));
    setHoverBackground(QColor(0x2076b1));

    setTextColor(Qt::white);
    setTextHoverColor(Qt::white);

    setFont(&gameStock->Font20);
  }
};

////////////////////////////////////////////////////////////////////////////////

#define ID_CANCEL   0
#define ID_OK       1

////////////////////////////////////////////////////////////////////////////////

LevelWinDialog::LevelWinDialog(GamePanelControl *panelControl) :
    GamePanel(panelControl, QRect(100,10,1024-100*2,768-20-70))
{
  setBlocking();
  setInsensitive();
  //setBackBorderPaint(false);

  setOpacity(0.9);
  setPen(QPen(QColor(0x00ddff), 1));
  setBackground(QColor(0x4b97c4));

  setHoverPaint(false);

  buttonOk = new GradientButton(this, QRect(1024-100-100-100-250,530,250,75), tr("Next Level"));
  buttonOk->setId(ID_OK);
  buttonOk->setPicture(&gameStock->Ok);

  buttonCancel = new GradientButton(this, QRect(100,530,250,75), tr("Exit to Menu"));
  buttonCancel->setId(ID_CANCEL);
  buttonCancel->setPicture(&gameStock->Cancel);
}

void LevelWinDialog::paintContent(QPainter &p)
{
  GamePanel::paintContent(p);

  p.setOpacity(1);
  p.setFont(gameStock->Font40);
  p.setPen(QPen(QColor(0x030055)));

  drawHCentered(p, 50, tr("Level %1 completed!").arg(level));

  p.setFont(gameStock->Font20);

  const int y30 = DY(40);

  const int x1 = DX(300), x2 = DX(380), x3 = DX(600);
  const int y1 = DY(250), y2 = DY(350);

  p.drawPixmap(x1,y1, gameStock->Clock);
  p.drawText(x3,y1+y30, QString("%1:%2").arg(time/60, 2, 10, QChar('0')).arg(time%60, 2, 10, QChar('0')));

  p.drawPixmap(x1,y2, gameStock->Score);
  p.drawText(x3,y2+y30, QString::number(score));

  p.setPen(QPen(Qt::black));
  p.drawText(x2,y1+y30, tr("Time left:"));
  p.drawText(x2,y2+y30, tr("Score:"));
}

void LevelWinDialog::subcontrolClicked(int id)
{
  switch (id)
  {
  case ID_OK:
    emit okClicked();
    close();
    break;
  case ID_CANCEL:
    emit cancelClicked();
    close();
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////

LevelFailDialog::LevelFailDialog(GamePanelControl *panelControl) :
    LevelWinDialog(panelControl)
{
  buttonOk->setText(tr("Restart Level"));
}

void LevelFailDialog::paintContent(QPainter &p)
{
  GamePanel::paintContent(p);

  p.setOpacity(1);
  p.setFont(gameStock->Font40);
  p.setPen(QPen(QColor(0x030055)));

  drawHCentered(p, 50, tr("Level %1 failed :(").arg(level));

  p.setFont(gameStock->Font20);

  drawHCentered(p, 250, tr("Hope next time you'll do it better..."));
}

////////////////////////////////////////////////////////////////////////////////

GameWinDialog::GameWinDialog(GamePanelControl *panelControl) :
    LevelWinDialog(panelControl)
{
  buttonOk->setInvisible();

  buttonCancel->setPosition(QPoint((rect().width()-250)/2, buttonCancel->rect().y()));
}

void GameWinDialog::paintContent(QPainter &p)
{
  GamePanel::paintContent(p);

  p.setOpacity(1);
  p.setFont(gameStock->Font60);
  p.setPen(QPen(QColor(0x030055)));

  drawHCentered(p, 50, tr("You're the Winner!"));

  p.setFont(gameStock->Font40);

  drawHCentered(p, 200, tr("You finished all the levels!"));

  p.setFont(gameStock->Font20);

  const int y30 = DY(40);

  const int x1 = DX(300), x2 = DX(420), x3 = DX(600);
  const int /*y1 = DY(250), */y2 = DY(350)/*, y3 = DY(350)*/;

  p.drawPixmap(x1,y2, gameStock->Score);
  p.drawText(x3,y2+y30, QString::number(score));

  p.setPen(QPen(Qt::black));
  p.drawText(x2,y2+y30, tr("Score:"));
}

////////////////////////////////////////////////////////////////////////////////
