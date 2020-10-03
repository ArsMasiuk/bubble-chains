#include "gamescene.h"
#include "gameprofile.h"
#include "gamestock.h"
#include "gamebonus.h"
#include "gametools.h"
#include "gamesound.h"
#include "sceneprivate.h"
#include "gamedialogs.h"


void GameScene::showStatictics(STAT_CODE code)
{
  LevelPackInfo *lpi = gameProfile->currentPlayer()->currentLevelPackInfo();

  switch (code)
  {
    case STAT_LEVEL_WON:
    {
      priv->levelWinDialog->setLevel(level);
      priv->levelWinDialog->setTime(time);
      priv->levelWinDialog->setScore(score);
      priv->levelWinDialog->show();
      break;
    }

    case STAT_LEVEL_FAIL:
    {
      priv->levelFailDialog->setLevel(level);
      priv->levelFailDialog->show();
      break;
    }

    case STAT_GAME_WON:
    {
      priv->gameWinDialog->setScore(score);
      priv->gameWinDialog->show();
      break;
    }

  }

  stat_active = true;

  advanceTimer->start();
  enableInput();

  if (code == STAT_LEVEL_WON || code == STAT_GAME_WON)
  {
    QTimer::singleShot(1000, this, SLOT(bonusTimeLeft()));

    lpi->localScore += time * BONUS_FOR_SECOND;
    lpi->score += time * BONUS_FOR_SECOND;
//    lpi->next_tool = toolset->activeToolIndex(lpi->score);

    // every 2nd level give a bonus
//    if ((level-1) % 2 == 1 && !lpi->isPuzzle())
//    {
//      toolset->nextToolActivated();
//      int idx = toolset->nextToolIndex();
//      lpi->next_tool = idx;
//      if (idx != ALL_TOOLS)
//        stat->tool = toolset->tool(idx-1);
//    }
    return;
  }
}

void GameScene::bonusTimeLeft()
{
  if (!stat_active)
    return;

  score += BONUS_FOR_SECOND;
//  toolset->addScore(BONUS_FOR_SECOND);

  sndEngine->playSound(GameSound::sndBonus);

  if (time > 0) {
    --time;
    QTimer::singleShot(10, this, SLOT(bonusTimeLeft()));
  }

  //priv->levelWinDialog->setTime(time);
  priv->levelWinDialog->setScore(score);
  priv->gameWinDialog->setScore(score);
}
