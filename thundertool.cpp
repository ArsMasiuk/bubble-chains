#include "thundertool.h"
#include "gameitem.h"
#include "gamesound.h"
#include "gamebonus.h"

#include "scene_if.h"

ThunderTool::ThunderTool(int x, int y, int score) : GameTool(x,y, score, "cross")
{
  bonusInfo = new BonusInfo(1000, myPixmap, tr("Row Blaster"),
                            tr("Removes all the items in a row"));
}

GameTool::ToolAction ThunderTool::checkItemState(int row, int col)
{
  int tr = myToolset->toolRow(), tc = myToolset->toolCol();

  PlaceInfo &pi1 = scene->data(tr,tc);
  if (pi1.empty())
    return ToolOutOfRange;

  if (row == tr)
  {
    PlaceInfo &pi = scene->data(row,col);
    if (pi.itemCanBeHighlighted())
    {
      if (!pi1.itemCanBeHighlighted())
        return ToolInactive;

      return ToolActive;
    }
    if (!pi.empty())
      return ToolInactive;
  }

  return ToolOutOfRange;
}


bool ThunderTool::checkItemClick(int row, int col)
{
  PlaceInfo &pi = scene->data(row,col);

  if (!pi.itemCanBeHighlighted())
    return false;

  myRow = row;
  myProgress = 1;

  return true;
}

bool ThunderTool::checkToolClick()
{
  if (myProgress >= MAX_COLS)
  {
    myProgress = 0;
    return false;
  }

  int cx = 5;

  for (int j = myProgress-1; j <= myProgress; j++)
  {
    PlaceInfo &pi = scene->data(myRow,j);

    if (pi.itemCanBeHighlighted())
    {
      scene->removeAndCountItem(myRow,j);

      scene->createPixmapPopup(scene->col2x(j), scene->row2y(myRow), 0, cx, myPixmap, 10);
    }

    cx = -cx;
  }

  sndEngine->playSound(GameSound::sndRow);

  myProgress += 2;

  return true;
}
