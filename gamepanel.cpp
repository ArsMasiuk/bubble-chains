#include "gamepanel.h"
#include "gamepanelcontrol.h"

#include "scaler.h"

GamePanel::GamePanel(GamePanel *parentPanel, const QRect &rect) :
    QObject(parentPanel),
    myControl(0),
    myRect(rect),
    myParent(parentPanel),
    myBlock(false),
    myHidden(false),
    myInsensitive(false),
    myBackBorder(true)
{
  parentPanel->subPanels.append(this);
  myRect.moveTo(parentPanel->myRect.topLeft() + myRect.topLeft());

  init();
}

GamePanel::GamePanel(GamePanelControl *panelControl, const QRect &rect) :
    QObject(panelControl),
    myControl(panelControl),
    myRect(rect),
    myParent(0),
    myBlock(false),
    myHidden(false),
    myInsensitive(false),
    myBackBorder(true)
{
  panelControl->addPanel(this);

  init();
}

void GamePanel::init()
{
  myBrush = QBrush(Qt::transparent);
  myHoverBrush = QBrush(Qt::black);
  myPen = myHoverPen = QPen(Qt::white, 2);

  myOpacity = myHoverOpacity = 1;

  myHover = false;
  myPaintHover = true;

  myId = -1;
}

void GamePanel::setPosition(QPoint pos)
{
  if (myParent)
    myRect.moveTo(myParent->myRect.topLeft() + pos);
  else
    myRect.moveTo(pos);
}

void GamePanel::setHover(bool set)
{
  myHover = false;
  for (int i = 0; i < subPanels.count(); i++)
  {
    GamePanel *panel = subPanels.at(i);
    panel->setHover(false);
  }
}

void GamePanel::show()
{
  if (myControl)
    myControl->activatePanel(this, true);

  // remove hover
  setHover(false);
}

void GamePanel::close()
{
  if (myControl)
    myControl->activatePanel(this, false);
}

void GamePanel::paint(QPainter &p)
{
  // paint itself
  if (!myHidden)
    paintContent(p);

  // paint subpanels if any
  for (int i = 0; i < subPanels.count(); i++)
  {
    GamePanel *panel = subPanels.at(i);
    panel->paint(p);
  }
}

void GamePanel::paintContent(QPainter &p)
{
  if (myBackBorder)
  {
    if (myHover && myPaintHover)
    {
      p.setOpacity(myHoverOpacity);
      p.setPen(myHoverPen);
      p.setBrush(myHoverBrush);
    }
    else
    {
      p.setOpacity(myOpacity);
      p.setPen(myPen);
      p.setBrush(myBrush);
    }

    p.drawRect(DRECT(myRect));
  }
}

bool GamePanel::mouseMove(QPoint pos, int buttons)
{
  myHover = false;
  for (int i = 0; i < subPanels.count(); i++)
  {
    GamePanel *panel = subPanels.at(i);
    if (myHover)
    {
      panel->myHover = false;
      continue;
    }

    if (panel->mouseMove(pos, buttons))
      myHover = true;
  }

  if (myHover)
    return true;

  // else, check own
  myHover = DRECT(myRect).contains(pos);
  if (myHover)
    return mouseMoveContent(pos, buttons);

  return false;
}

bool GamePanel::mouseClick(QPoint pos, int buttons)
{
  for (int i = 0; i < subPanels.count(); i++)
  {
    GamePanel *panel = subPanels.at(i);
    if (panel->mouseClick(pos, buttons))
      return true;
  }

  // else, check own
  myHover = DRECT(myRect).contains(pos);
  if (myHover)
    return mouseClickContent(pos, buttons);

  return false;
}

bool GamePanel::mouseMoveContent(QPoint pos, int buttons)
{
  return true;
}

bool GamePanel::mouseClickContent(QPoint pos, int buttons)
{
  if (myId >= 0/* && buttons & Qt::LeftButton*/)
  {
    if (myParent)
      myParent->subcontrolClicked(myId);

    return true;
  }

  return false;
}

void GamePanel::subcontrolClicked(int id)
{

}


void GamePanel::drawHCentered(QPainter &p, int y, const QString &text)
{
  QRect r(myRect);
  r.setTop(y);

  p.drawText(DRECT(r), Qt::AlignHCenter | Qt::AlignTop, text);
}
