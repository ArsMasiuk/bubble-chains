#include "hammertool.h"
#include "gameitem.h"
#include "gamesound.h"
#include "gamebonus.h"

#include "scene_if.h"

HammerTool::HammerTool(int x, int y, int score) : GameTool(x, y, score, "star")
{
  bonusInfo = new BonusInfo(1000, myPixmap, tr("Item Killer"),
                            tr("Destroys an item at a certain position"));
}

GameTool::ToolAction HammerTool::checkItemState(int row, int col)
{
  if (row == myToolset->toolRow() && col == myToolset->toolCol())
  {
    PlaceInfo &pi = scene->data(row,col);
    if (pi.itemCanBeHighlighted())
      return ToolActive;
    if (!pi.empty())
      return ToolInactive;
  }

  return ToolOutOfRange;
}

bool HammerTool::checkItemClick(int row, int col)
{
  PlaceInfo &pi = scene->data(row,col);
  if (pi.itemCanBeHighlighted())
  {
    // this only schedules a removal
    scene->removeAndCountItem(row,col);

    scene->createPixmapPopup(scene->col2x(col), scene->row2y(row), 0, 5, myPixmap, 5);
    scene->createPixmapPopup(scene->col2x(col), scene->row2y(row), 0, -5, myPixmap, 5);

    sndEngine->playSound(GameSound::sndHammer);

    return true;
  }

  return false;
}
