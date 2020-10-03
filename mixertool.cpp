#include "mixertool.h"
#include "gamesound.h"
#include "gamebonus.h"

#include "scene_if.h"

MixerTool::MixerTool(int x, int y, int score) : GameTool(x,y, score, "random")
{
  bonusInfo = new BonusInfo(1000, myPixmap, tr("Mixer"),
                            tr("Randomly shuffles items in the field"));
}

bool MixerTool::checkToolClick()
{
  if (myProgress++ == 10)
  {
    myProgress = 0;
    return false;
  }

  // skip every 2nd frame
//  if (myProgress & 1)
//    return true;


  PlaceInfo *data = const_cast<PlaceInfo*>( scene->data() );

  const int max = MAX_COLS*MAX_ROWS;
  QList<int> list;

  for (int i = 0; i < max; i++)
  {
    if (data[i].itemCanBeHighlighted())
      list.append(i);
  }

  for (int i = 0; i < 20; i++)
  {
    int i1 = list.at(qrand() % list.count());
    list.removeAt(i1);
    //quint8 id1 = data[i1].item->id();

    int i2 = list.at(qrand() % list.count());
    list.removeAt(i2);
    //quint8 id2 = data[i2].item->id();

    GameItem *temp = data[i1].item;
    data[i1].item = data[i2].item;
    data[i2].item = temp;

    int row1 = i1 / MAX_COLS;
    int col1 = i1 % MAX_COLS;

    int row2 = i2 / MAX_COLS;
    int col2 = i2 % MAX_COLS;

    scene->createPixmapPopup(scene->col2x(col1), scene->row2y(row1), 0, 5, myPixmap, 5);
    scene->createPixmapPopup(scene->col2x(col2), scene->row2y(row2), 0, 5, myPixmap, 5);
  }

  sndEngine->playSound(GameSound::sndMixer);

  return true;
}

