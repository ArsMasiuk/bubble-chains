#include "clocktool.h"
#include "gamesound.h"
#include "gamebonus.h"

#include "scene_if.h"

ClockTool::ClockTool(int x, int y, int score) : GameTool(x,y, score, "clock")
{
  bonusInfo = new BonusInfo(1000, myPixmap, tr("Clock"),
                            tr("Adds +60 seconds to the timer"));
}

bool ClockTool::checkToolClick()
{
  scene->addTime(60);

  sndEngine->playSound(GameSound::sndClock);

  return true;
}

