#include "randomkilltool.h"
#include "gamesound.h"
#include "gamebonus.h"

#include "scene_if.h"

RandomKillTool::RandomKillTool(int x, int y, int score) : GameTool(x,y, score, "bolt")
{
  bonusInfo = new BonusInfo(1000, myPixmap, tr("Bolt"),
                            tr("Randomly removes items from the field"));
}

bool RandomKillTool::checkToolClick()
{
  if (myProgress++ == 10)
  {
    myProgress = 0;
    return false;
  }

  // skip every 2nd frame
//  if (myProgress & 1)
//    return true;


  const PlaceInfo *data = scene->data();

  const int max = MAX_COLS*MAX_ROWS;
  QList<int> list;

  for (int i = 0; i < max; i++)
  {
    if (data[i].itemCanBeHighlighted())
      list.append(i);
  }

  for (int i = 0; i < 5; i++)
  {
    int i1 = list.at(qrand() % list.count());
    list.removeAt(i1);

    int row = i1 / MAX_COLS;
    int col = i1 % MAX_COLS;

    scene->removeAndCountItem(row, col);

    scene->createPixmapPopup(scene->col2x(col), scene->row2y(row), 0, 5, myPixmap, 5);
    scene->createPixmapPopup(scene->col2x(col), scene->row2y(row), 0, -5, myPixmap, 5);
  }

  sndEngine->playSound(GameSound::sndRandomKill);

  return true;
}

