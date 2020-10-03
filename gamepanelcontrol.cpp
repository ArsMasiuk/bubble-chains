#include "gamepanelcontrol.h"
#include "gamepanel.h"

GamePanelControl::GamePanelControl(QObject *parent) : QObject(parent)
{
}

void GamePanelControl::addPanel(GamePanel *panel)
{
  panels.append(panel);
}

void GamePanelControl::activatePanel(GamePanel *panel, bool activate)
{
  bool block = panel->isBlocking();
  QList<GamePanel*> &list = block ? activeBlockingPanels : activeNonBlockingPanels;

  if (activate)
  {
    if (activePanels.contains(panel))
      return;

    if (list.isEmpty() && block)
      emit blocked();

    list.append(panel);
    activePanels.append(panel);
  }
  else
  {
    if (!activePanels.contains(panel))
      return;

    list.removeAll(panel);
    activePanels.removeAll(panel);

    if (!block && list.isEmpty())
      emit unblocked();
  }
}

void GamePanelControl::paint(QPainter &p)
{
  for (int i = 0; i < activePanels.count(); i++)
  {
    GamePanel *panel = activePanels.at(i);
    panel->paint(p);
  }
}


bool GamePanelControl::mouseMove(QPoint pos, int buttons)
{
  bool hover = false;
  for (int i = 0; i < activePanels.count(); i++)
  {
    GamePanel *panel = activePanels.at(i);
    if (hover)
    {
      panel->setHover(false);
      continue;
    }

    if (panel->mouseMove(pos, buttons))
      hover = true;
  }

  return hover;
}

bool GamePanelControl::mouseClick(QPoint pos, int buttons)
{
  for (int i = 0; i < activePanels.count(); i++)
  {
    GamePanel *panel = activePanels.at(i);
    if (panel->mouseClick(pos, buttons))
      return true;
  }

  return false;
}

