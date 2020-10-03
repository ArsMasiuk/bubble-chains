#include "sceneprivate.h"

#include "progressbar.h"

#include "gameitem.h"

#include <QtGui>

ScenePrivate::ScenePrivate()
{
  QLinearGradient gr1(0,0,40,0);
  gr1.setColorAt(0, QColor(0x031849));
  gr1.setColorAt(0.5, QColor(0x0079ff));
  gr1.setColorAt(1, QColor(0x031849));

  timeBar = new ProgressBar(QRect(1024-30-40,60, 40,630));
  timeBar->setAutoMaxAdjust(true);
  timeBar->setUpSpeed(25);
  timeBar->setBrush(gr1);

  // flags
  newItemFlags = ITEM_JOCKER | ITEM_FALLER;

  // dialogs and controllers are created in GameScene()
}

ScenePrivate::~ScenePrivate()
{
  delete timeBar;
}

