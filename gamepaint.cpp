#include "gamescene.h"
#include "gameitem.h"
#include "baseitem.h"
#include "gametools.h"
#include "gamemenu.h"
#include "gamestock.h"
#include "gameprofile.h"
#include "gamebonus.h"
#include "gamebackground.h"
#include "gametargetcontrol.h"
#include "gamepanelcontrol.h"
#include "gamehintcontrol.h"
#include "particlecontrol.h"
#include "sceneprivate.h"
#include "progressbar.h"

void GameScene::drawBackground(QPainter *painter, const QRectF &)
{
  if (myLock)
    return;

  QPainter &p(*painter);
  p.setRenderHint(QPainter::Antialiasing);

  paintState = true;

  p.setOpacity(1);

  if (menu->isActive() && !gameProfile->isGamePaused())
  {
    p.drawPixmap(0,0, gameStock->MenuBackground);
  }
  else
  {
    // background
    p.drawPixmap(0,0,bgpicture);

    // concrete & target layer
    if (level_start)
    {
      drawLevelStartup(p);
    }
    else
    {
      p.setOpacity(1);

      for (int i = 0; i < rows; i++)
      {
        //int rowy = row2y(i);

        for (int j = 0; j < cols; j++)
        {
          PlaceInfo &pi = data(i,j);

          if (pi.empty())
            continue;

          //int colx = col2x(j);

//          if (pi.hasTarget())
//            p.drawPixmap(colx, rowy, pi.place & Target1 ? target1 : target2);
        }
      }

      // items & blocks layer
      for (int i = 0; i < rows; i++)
      {
        int rowy = row2y(i);

        for (int j = 0; j < cols; j++)
        {
          PlaceInfo &pi = data(i,j);

          int colx = col2x(j);

          if (pi.hasItem())
          {
            // selection
            if (pi.item->isSelected())
            {
              p.setOpacity(0.3);
              p.setBrush(Qt::yellow);
    #if QT_VERSION >= 0x040400
              p.drawRoundedRect(colx+1, rowy+1, X56-2, Y56-2, 5, 5);
    #else
              p.drawRect(colx+1, rowy+1, X56-2, Y56-2);
    #endif
            }

            // item
            pi.item->draw(p, item_col2x(j), item_row2y(i));
//            p.setOpacity(pi.item->opacity());
//            p.drawPixmap(item_col2x(j) + pi.item->dx(), item_row2y(i) + pi.item->dy(),
//                         allItems.at(pi.item->id()));
          }

//          if (pi.hasBlock())
//          {
//            p.setOpacity(1);
//            p.drawPixmap(colx, rowy, pi.place & Block1 ? block1 : block2);
//          }

          // check tools
          GameTool::ToolAction state = toolset->checkItemState(i, j);
          if (state != GameTool::ToolOutOfRange)
          {
            static float op = 0.8;
            static float dop = -0.002;
            op += dop;
            if (op <= 0.3 || op >= 0.8) dop = -dop;

            p.setOpacity(op);

            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(QColor(state & GameTool::ToolActive ? Qt::green : Qt::red), 2));
  #if QT_VERSION >= 0x040400
            p.drawRoundedRect(colx+1, rowy+1, X56-2, Y56-2, 5, 5);
  #else
            p.drawRect(colx+1, rowy+1, X56-2, Y56-2);
  #endif
          }

        }
      }

      // draw chain
      int cnt = priv->chain.count();
      if (cnt)
      {
        static float op = 0.3;
        static float dop = -0.02;
        op += dop;
        if (op <= 0.1 || op >= 0.3) dop = -dop;

        p.setFont(gameStock->Font20);
        p.setBrush(cnt > 3 ? Qt::green : cnt == 3 ? Qt::yellow : Qt::red);

        for (int i = 0; i < cnt; i++)
        {
          p.setOpacity(op);

          const QPoint &coord = priv->chain.at(i);
          QRect r(col2x(coord.x()), row2y(coord.y()), X56, Y56);

          p.drawRoundedRect(r, 5, 5);

          p.setOpacity(1);
          p.setPen(i > 2 ? Qt::green : i == 2 ? Qt::yellow : Qt::red);
          p.drawText(r, Qt::AlignCenter, QString::number(i+1));
        }

      }

    }

    // hint layer
    //drawHint(p);

    // HUD layer
    updateCounters(p);

    // tools
    toolset->updateTools(p);

    // bonus
    //gameBonus->updateItems(this, p);

    // targets
    targetControl->paint(p);

    // temp layer
    for (int i = 0; i < tempItems.count(); i++)
    {
      tempItems.at(i)->paint(p);
    }

    // text hint
    if (hintText.length() && !stat_active)
    {
      drawTextHint(p);
    }

    // bonus window
//    if (gameBonus->isActive())
//    {
//      gameBonus->drawBonusMenu(this, p);
//    }

    // statistics
//    if (stat_active)
//    {
//      drawStatInfo(p);
//    }

    // windows
    priv->panelControl->paint(p);

    // tips
    hintControl->paint(p);

    // particles
    partControl->paint(p);

  }

  // menu
  if (menu->isActive())
  {
    if (gameProfile->isGamePaused())
    {
      p.setOpacity(1);
      p.setPen(QPen(Qt::black));
      p.setBrush(QBrush(Qt::black));

      // background
      p.setOpacity(0.7);
      p.drawRect(0, 0, WIDTH, HEIGHT);
    }

      // logo
      p.setOpacity(0.7);

      if (gameProfile->isGameStarted())
      {
        p.drawPixmap(LEFT_CENTER(gameStock->GameLogoBig.width(),WIDTH),
                     10,
                     gameStock->GameLogoBig
                     );
      }

      // user name and info
      if (menu->isVisible())
      {
        p.setBrush(QBrush(QColor(0x009cdf)));
        p.setPen(QPen(Qt::cyan));

        int w = DX(200);
        p.drawRect(menu->x()-w,DY(330), w,DY(242));

        p.setFont(gameStock->Font8);
        p.setPen(QPen(Qt::white));

        int dy = DY(350), ddy = DY(60), dx = menu->x()-w+DX(10);
        p.drawText(dx, dy, tr("Player"));
        dy += ddy;
        p.drawText(dx, dy, tr("Level Pack"));
        dy += ddy;
        p.drawText(dx, dy, tr("Level"));
        dy += ddy;
        p.drawText(dx, dy, tr("Score"));

        PlayerInfo *pl = gameProfile->currentPlayer();
        LevelPackInfo *lpi = pl->currentLevelPackInfo();

        p.setFont(gameStock->Font20);
        p.setPen(QPen(QColor(0xaad6e9)));

        dy = DY(380);

        p.drawText(menu->buttonProfile.bottomLeft(), QString("%1").arg(pl->name));
        dy += ddy;
        p.drawText(menu->buttonLevelpack.bottomLeft(), QString("%1").arg(gameProfile->levelPackName(pl->currentlevelpack)));
        dy += ddy;

        int lvl = (lpi->level > max_level) ? max_level : lpi->level;
        p.drawText(dx, dy, QString("%1/%2").arg(lvl).arg(max_level));
        dy += ddy;
        p.drawText(dx, dy, QString("%1").arg(lpi->score));
      }

      p.setOpacity(1);
  }

  paintState = false;
}


void GameScene::updateCounters(QPainter &p)
{
  // global counters
  p.setPen(QPen(Qt::white));
  p.setOpacity(1);
  p.setFont(gameStock->Font20);

  // longest chain
  p.drawText(0,DY(730), X100,Y20, Qt::AlignCenter,
             QString::number(priv->longest_chain));

  // score & level
  p.drawText(DX(924),DY(730), X100,Y20, Qt::AlignCenter,
             QString("%1").arg(score, 7, 10, QChar('0')));
  p.drawText(DX(820),DY(730), X100,Y20, Qt::AlignCenter,
             QString::number(level));

  // time
  p.setPen(QPen(time < 30 ? Qt::red : time < 60 ? Qt::yellow : Qt::white));
  p.drawText(DX(924),DY(30), X100,Y20, Qt::AlignCenter,
             QString("%1:%2").arg(time/60, 2, 10, QChar('0')).arg(time%60, 2, 10, QChar('0')));

//  p.setPen(QPen(bonus > 1 ? Qt::white : Qt::gray));
//  p.drawText(X910,DY(200), QString("x%1").arg(bonus));
//  if (bonus_time)
//  {
//    p.setPen(QPen(bonus_time > 7 ? Qt::green : bonus_time > 3 ? Qt::yellow : Qt::red));
//    p.drawText(DX(970),DY(180),DX(46),DY(20),
//                      Qt::AlignVCenter | Qt::AlignRight,
//                      QString("%1").arg(bonus_time, 2, 10, QChar('0')));
//  }

  // progress counters
  priv->timeBar->setValue(time);
  priv->timeBar->paint(p);
}

void GameScene::drawHint(QPainter &p)
{
  static const QColor colors[] =
  { Qt::blue, Qt::yellow, Qt::red, Qt::green, Qt::yellow, Qt::cyan, Qt::magenta };
  static int idx1 = qrand() % 7, idx2 = qrand() % 7;

  if (hintMove && availFrom.x() >= 0 && availTo.x() >= 0)
  {
    p.setPen(QPen(colors[idx1], 4));
    p.setBrush(colors[idx2]);
    p.setOpacity(hintOpacity);
    hintOpacity += hintDelta;
    if (hintOpacity >= 0.5 || hintOpacity <= 0)
    {
      hintDelta = -hintDelta;
      if (hintDelta > 0)
      {
        idx1 = qrand() % 7;
        idx2 = qrand() % 7;
      }
    }

#if QT_VERSION >= 0x040400
    p.drawRoundedRect(col2x(availFrom.x())+1, row2y(availFrom.y())+1, X56-2, Y56-2, 5, 5);
    p.drawRoundedRect(col2x(availTo.x())+1, row2y(availTo.y())+1, X56-2, Y56-2, 5, 5);
#else
    p.drawRect(col2x(availFrom.x())+1, row2y(availFrom.y())+1, X56-2, Y56-2);
    p.drawRect(col2x(availTo.x())+1, row2y(availTo.y())+1, X56-2, Y56-2);
#endif
  }
}

////////////////////////////////////////////////////////////////////////////////

void GameScene::drawLevelStartup(QPainter &p)
{
  // items
  static int cnt_init = rows*cols/4;
  static int cnt_inc = 4;
  static int cnt = cnt_init;
  static int c2 = cols/2-1;

  for (int i = 0; i < rows; i++)
  {
    int xoff = -c2*cnt*cnt_inc;
    int rowy = row2y(i);

    for (int j = 0; j < cols; j++)
    {
      if (i >= ls_y && j >= ls_x)
        goto _brak;

      PlaceInfo &pi = data(i,j);

      if (!pi.empty())
      {
        int colx = col2x(j)+xoff;

        p.setOpacity(0.8);
        p.drawPixmap(colx, rowy, gameStock->Concrete);

        if (pi.hasItem())
        {
          pi.item->draw(p, item_col2x(j)+xoff, item_row2y(i));
        }

      }

      xoff+=cnt*cnt_inc;
    }
  }

_brak:

  if (cnt) cnt--;

  ls_x += 4;
  if (ls_x >= cols)
  {
    ls_x = 0;
    if (++ls_y >= rows)
    {
      ls_y = 0;
      cnt = cnt_init;

      // force draw concretes on bg
      drawConcretesOnBackground();

      enterLevel();

      return;
    }
  }
}

void GameScene::drawConcretesOnBackground()
{
    QPainter p(&bgpicture);

    p.setOpacity(0.8);
    for (int i = 0; i < rows; i++)
    {
        int rowy = row2y(i);

        for (int j = 0; j < cols; j++)
        {
          PlaceInfo &pi = data(i,j);

          if (pi.empty())
            continue;

          int colx = col2x(j);

          p.drawPixmap(colx, rowy, gameStock->Concrete);
        }
    }
}

void GameScene::drawHUDonBackground()
{
  QPainter p(&bgpicture);

  p.setPen(QPen(Qt::white));
  p.setOpacity(1);

  p.setFont(gameStock->Font12);
  p.drawText(DX(924),DY(700), X100,Y20, Qt::AlignHCenter, tr("Score"));
  p.drawText(DX(924),DY(10), X100,Y20, Qt::AlignHCenter, tr("Time"));
  p.drawText(DX(820),DY(700), X100,Y20, Qt::AlignHCenter, tr("Level"));
//  p.drawText(X910,DY(160), X100,Y20, Qt::AlignLeft, tr("Bonus"));

//  p.setFont(gameStock->Font12);
  p.drawText(0,DY(700), X100,Y20, Qt::AlignHCenter, tr("Longest Chain"));
  //p.drawText(0,DY(670), WIDTH,Y20, Qt::AlignHCenter, tr("Targets"));
}
