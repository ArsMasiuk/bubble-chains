#include "gamescene.h"
#include "gameitem.h"
#include "gamemenu.h"
#include "gametools.h"
#include "gamebonus.h"
#include "gameprofile.h"
#include "gametargetcontrol.h"
#include "gamepanelcontrol.h"
#include "gamehintcontrol.h"
#include "sceneprivate.h"
#include "progressbar.h"

////////////////////////////////////////////////////////////////////////////////

void GameScene::keyPressEvent ( QKeyEvent * keyEvent )
{
  if (!menu->isActive())
  {
    switch (keyEvent->key())
    {
      case Qt::Key_Escape:
      {
          if (gameBonus->isActive())
          {
            gameBonus->hideBonusMenu(this);
            break;
          }

//          if (stat_active)
//          {
//            if (stat->code == STAT_GAME_WON)
//            {
//              exitToMainMenu();
//              break;
//            }
//
//            runNextLevel();
//            break;
//          }

          pauseGame();
          break;
      }

    case Qt::Key_F1:
      menu->showHelp();
      break;

#if 0
      // that's only for debugging reasons
      case Qt::Key_1:
      case Qt::Key_2:
      case Qt::Key_3:
      case Qt::Key_4:
      case Qt::Key_5:
      case Qt::Key_6:
      {
          int id = keyEvent->key() - Qt::Key_1;
          targetControl->checkChain(10,id,10-2);
          break;
      }

      case Qt::Key_9:
        score += 1000;
        break;

      case Qt::Key_0:
        toolset->addScore(40);
        break;

      case Qt::Key_W:
        levelWon();
        break;

      case Qt::Key_X:
        level = max_level;
        levelWon();
        break;

      case Qt::Key_F:
        time = 2;
        break;

      case Qt::Key_H:
        hintAvailableMoves();
        break;

      case Qt::Key_J:
        hintMove = false;
        break;
#endif
      default:;
    }
  }
  else {
      QGraphicsScene::keyPressEvent(keyEvent);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GameScene::mousePressEvent ( QGraphicsSceneMouseEvent *mouseEvent )
{
  QGraphicsScene::mousePressEvent(mouseEvent);
  if (mouseEvent->isAccepted())
    return;

  mouseEvent->accept();

  QPoint pos = mouseEvent->scenePos().toPoint();

  //qDebug() << "click pos: " << pos;

  // windows
  priv->panelControl->mouseClick(pos, mouseEvent->buttons());

  // hints
  hintControl->mouseClick(pos, mouseEvent->buttons());

  // click on menu shortcuts
  if (mouseEvent->button() == Qt::LeftButton)
      menu->checkMenuMouseActions(pos);

  if (inputDisabled || paintState)
    return;

  if (mouseEvent->button() == Qt::LeftButton)
  {
    // lets add more randomness
    qsrand(QDateTime::currentDateTime().toTime_t());

    // find position
    lastClickPos = pos;
  }
}

////////////////////////////////////////////////////////////////////////////////

void GameScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent *mouseEvent )
{
  QGraphicsScene::mouseReleaseEvent(mouseEvent);
  if (mouseEvent->isAccepted())
    return;

  mouseEvent->accept();

  if (mouseEvent->button() == Qt::LeftButton)
    lastReleasePos = mouseEvent->scenePos().toPoint();
  else
    lastReleasePos = QPoint();
}

////////////////////////////////////////////////////////////////////////////////

void GameScene::mouseMoveEvent ( QGraphicsSceneMouseEvent *mouseEvent )
{
  QGraphicsScene::mouseMoveEvent(mouseEvent);
  if (mouseEvent->isAccepted())
    return;

  // scene
  cursorPos = mouseEvent->scenePos().toPoint();

  mouseEvent->accept();

  // windows
  if (priv->panelControl->mouseMove(cursorPos, mouseEvent->buttons()))
    return;

  if (inputDisabled || paintState)
    return;

  // check dragging
  if (mouseEvent->buttons() & Qt::LeftButton)
  {
    lastDragPos = cursorPos;
  } else
    lastDragPos = QPoint();

//  // check for hovers
//  if (toolset->checkMouseHover(cursorPos))
//    return;
//  if (gameBonus->checkMouseHover(cursorPos))
//    return;

  removeHint();
}

