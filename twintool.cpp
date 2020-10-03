#include "twintool.h"
#include "gamesound.h"
#include "gamebonus.h"

#include "scene_if.h"

TwinTool::TwinTool(int x, int y, int score) : GameTool(x,y, score, "twins")
{
  bonusInfo = new BonusInfo(1000, myPixmap, tr("Twin Blaster"),
                            tr("Removes all the identical items from the field"));
}

GameTool::ToolAction TwinTool::checkItemState(int row, int col)
{
  PlaceInfo &pi = scene->data(row,col);
  PlaceInfo &pi1 = scene->data(myToolset->toolRow(), myToolset->toolCol());

  if (pi.itemCanBeHighlighted() && pi1.itemCanBeHighlighted())
  {
    if (pi1.item->id() == pi.item->id())
      return ToolActive;
  }

  return ToolOutOfRange;
}

bool TwinTool::checkItemClick(int row, int col)
{
  PlaceInfo &pi = scene->data(row,col);
  if (pi.itemCanBeHighlighted())
  {
    quint8 id = pi.item->id();
    int rows = scene->numRows();
    int cols = scene->numCols();

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
        PlaceInfo &pi1 = scene->data(i,j);
        if (pi1.itemCanBeHighlighted())
        {
          if (pi1.item->id() == id)
          {
            // remove block
            scene->removeAndCountItem(i,j);

            scene->createPixmapPopup(scene->col2x(j), scene->row2y(i), 0, 5, myPixmap, 20);
            scene->createPixmapPopup(scene->col2x(j), scene->row2y(i), 0, -5, myPixmap, 20);
          }
        }
      }
    }

    sndEngine->playSound(GameSound::sndTwin);

    return true;
  }

  return false;
}
