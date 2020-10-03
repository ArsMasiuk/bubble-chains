#include "gamebonus.h"
#include "gamescene.h"
#include "gamestock.h"
#include "gameprofile.h"
#include "gamewidget.h"
#include "gametools.h"
#include "gamesound.h"
#include "scene_if.h"

GameBonus * gameBonus = 0;


BonusInfo::BonusInfo(const QRect &_rect, int _score,
            const QString &resource, const QString &_name, const QString &_comment) :
  rect(_rect),
  score(_score),
  name(_name),
  comment(_comment)
{
  pixmap = QPixmap(GameWidget::getResourcePath() + "/bonus/" + resource + ".png")
             .scaled(X48,Y48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

BonusInfo::BonusInfo(const QRect &_rect, int _score, const QPixmap &_pixmap, const QString &_name, const QString &_comment) :
  rect(_rect),
  pixmap(_pixmap),
  score(_score),
  name(_name),
  comment(_comment)
{
}

BonusInfo::BonusInfo(int _score, const QPixmap &_pixmap, const QString &_name, const QString &_comment) :
  pixmap(_pixmap),
  score(_score),
  name(_name),
  comment(_comment)
{
}

BonusInfo::BonusInfo(const QString &_name, const QString &_comment) :
  score(0),
  name(_name),
  comment(_comment)
{
}

void BonusInfo::initFrom(BonusInfo *source)
{
  score = source->score;
  name = source->name;
  comment = source->comment;
  pixmap = source->pixmap;
}


GameBonus::GameBonus() :
    bonusId(-1)
{
  //initGraphics(0);
}

void GameBonus::setupItems(int count)
{
  items.clear();
  for (int i = 0; i < count; i++)
    items.append(ItemBonusInfo());
}

void GameBonus::initGraphics(GameScene *scene)
{
  if (bonusInfo.count())
  {
    qDeleteAll(bonusInfo);
    bonusInfo.clear();
  }

  bonusInfo.append(new BonusInfo(QRect(DX(100),DY(100),DX(660),DY(80)), 500, QPixmap(),
                                 tr("Low"), tr("only items are touched")));

  bonusInfo.append(new BonusInfo(QRect(DX(100),DY(200),DX(660),DY(80)), 1000, "power2",
                                 tr("Middle"), tr("item + single target or single block")));

  bonusInfo.append(new BonusInfo(QRect(DX(100),DY(300),DX(660),DY(80)), 1500, "power3",
                                 tr("Strong"), tr("item + double target or double block")));

  bonusInfo.append(new BonusInfo(QRect(DX(100),DY(400),DX(660),DY(80)), 2000, "power4",
                                 tr("Elimination"), tr("item + double target and block")));

  bonusInfo.append(new BonusInfo(QRect(DX(100),DY(500),DX(660),DY(80)), 2500, "power5",
                                 tr("Demolition"), tr("total annihilation")));

  // define bonus rects
  if (scene)
    updateRects(scene->gameItems().count());
}

void GameBonus::updateRects(int cnt)
{
  bonusRects.clear();
  if (!cnt) return;

  int x = 0, y = DY(710);
  int dx = DX(860) / cnt;
  for (int i = 0; i < cnt; i++)
  {
    bonusRects.append(QRect(x,y, dx-1,Y56));
    x += dx+1;
  }
}

void GameBonus::readProfile(LevelPackInfo *lpi, GameScene *scene)
{
  // set counters
  for (int i = 0; i < items.count(); i++)
  {
    ItemBonusInfo &localInfo = items[i];
    localInfo.levelCount = 0;
    localInfo.count = lpi->items.count() > i ? lpi->items.at(i).localCount : 0;
    localInfo.totalCount = lpi->items.count() > i ? lpi->items.at(i).totalCount : 0;
  }

  diff = lpi->diff;
  puzzle = lpi->mode;

  bonusId = -1;

  //bonus rects moved to InitGraphics

  initGraphics(scene);
}

void GameBonus::writeProfile(LevelPackInfo *lpi)
{
  for (int i = 0; i < items.count(); i++)
  {
    if (lpi->items.count() == i)
      lpi->items.append(ItemInfo());

    lpi->items[i].totalCount = items.at(i).totalCount;
    lpi->items[i].localCount = items.at(i).count;
  }
}

void GameBonus::checkMouseActions(GameScene *scene, const QPoint &pos)
{
  // items
  for (int i = 0; i < bonusRects.count(); i++)
  {
    if (bonusRects.at(i).contains(pos))
    {
      showBonusMenu(scene, scene->gameItems().at(i));
      return;
    }
  }

  // bonuses
  if (gameBonus->isActive())
    for (int i = 0; i < bonusInfo.count(); i++)
    {
      BonusInfo *bi = bonusInfo.at(i);
      if (! bi->isEmpty() && bi->rect.contains(pos))
      {
//        QMessageBox::about(0,"fuck","should not be here");
        activateBonus(scene, bi, i);
        hideBonusMenu(scene);
        return;
      }
    }

  if (bonusId >= 0)
    hideBonusMenu(scene);
}

bool GameBonus::checkMouseHover(const QPoint &pos)
{
  if (bonusId >= 0)
    return false;

  if (QRect(0,DY(710),DX(860),DY(58)).contains(pos))
  {
      scene->showHint(tr("Item Elimination: "
                      "Click to remove all the similar items (if you got 500+ of them)"));
      return true;
  }

  return false;
}


void GameBonus::showBonusMenu(GameScene *scene, int idx)
{
  if (bonusId == idx)
  {
    hideBonusMenu(scene);
    return;
  }

  bonusId = idx;

  scene->disableTimersButAdvance();
}

void GameBonus::hideBonusMenu(GameScene *scene)
{
  bonusId = -1;

  scene->enableTimers();
}

void GameBonus::drawBonusMenu(GameScene *scene, QPainter &p)
{
  int currentBonus = items.at(bonusId).count;

  p.setOpacity(0.9);
  p.setPen(QPen(Qt::white, 2));
  p.setBrush(Qt::black);
#if QT_VERSION >= 0x040400
  p.drawRoundedRect(0+1,0+1,DX(860)-1,DY(700)-1, 5,5);
#else
  p.drawRect(0+1,0+1,DX(860)-1,DY(700)-1);
#endif

  p.setOpacity(1);

  p.drawPixmap(DX(20),DY(20), GameItem::itemPixmap(bonusId));

  p.setFont(gameStock->Font20);

  p.drawText(DX(80),DY(50), QString::number(currentBonus));
  p.drawText(0,0,DX(860),DY(50), Qt::AlignHCenter | Qt::AlignBottom, "Choose available power");

  // draw items
  for (int i = 0; i < bonusInfo.count(); i++)
  {
    drawBonusInfo(scene, p, bonusInfo.at(i), i);
  }
}

void GameBonus::updateItems(GameScene *scene, QPainter &p)
{
  // item counters
  p.setOpacity(1);
  for (int i = 0; i < scene->gameItems().count(); i++)
  {
    const QPoint &point = bonusRects.at(i).topLeft();

    int idx = scene->gameItems().at(i);
    if (idx < items.count())
    {
        if (bonusId == idx)
        {
          scene->drawTransRect(p, bonusRects.at(i), Qt::white, Qt::gray, 0.3);
        }


        const ItemBonusInfo &info = items.at(idx);

        p.setOpacity(info.count < 500 ? 0.5 : 1);

        p.drawPixmap(point + QPoint(2,2), GameItem::itemPixmap(idx));

        //p.setOpacity(1);

//        p.setPen(QPen(info.count < 500 ? Qt::white :
//                      info.count < 1000 ? Qt::green :
//                      info.count < 1500 ? Qt::yellow :
//                      info.count < 2000 ? Qt::cyan :
//                      info.count < 2500 ? Qt::magenta :
//                      Qt::red));
        p.setPen(QPen(info.count < 500 ? Qt::white : Qt::green));
        p.setFont(gameStock->Font20);
        p.drawText(point.x()+DX(50),point.y(),DX(76),DY(30),
                   Qt::AlignRight | Qt::AlignTop,
                   QString("%1").arg(info.count, 5, 10, QChar('0')));

        p.setPen(QPen(Qt::white));
        p.setFont(gameStock->Font8);
        p.drawText(point.x()+DX(50),point.y()+DY(30),DX(76),DY(20),
                   Qt::AlignRight | Qt::AlignTop,
                   QString("%1").arg(info.levelCount));
    }
  }
}


void GameBonus::drawBonusInfo(GameScene *scene, QPainter &p, BonusInfo *info, int idx)
{
  if (info->isEmpty())
    return;

  int score = info->score;
  if (diff == 1) score /= 2;
  else if (diff == 3) score *= 2;

  int currentBonus = items.at(bonusId).count;
  bool active = score <= currentBonus;

  // border
  if (active && info->rect.contains( scene->cursorPosition() ))
  {
    scene->drawTransRect(p, info->rect, Qt::white, Qt::darkGray, 0.1);
  }

  p.setOpacity(active ? 1 : 0.3);

  p.drawPixmap(info->rect.topLeft() + QPoint(DX(10),DY(10)),
               idx ? info->pixmap :
               GameItem::itemPixmap(bonusId)   );

  p.setFont(gameStock->Font20);

  p.setPen(Qt::white);
  p.drawText(QRect(info->rect.topLeft() + QPoint(DX(120),DY(10)), QSize(DX(100),DY(30))),
             Qt::AlignHCenter,
             QString::number(score)
             );

  p.setPen(Qt::yellow);
  p.drawText(QRect(info->rect.topLeft() + QPoint(DX(300),DY(10)), QSize(DX(400),DY(30))),
             Qt::AlignLeft,
             info->name
             );

  p.setFont(gameStock->Font12);

  p.setPen(Qt::darkGray);
  p.drawText(QRect(info->rect.topLeft() + QPoint(DX(300),DY(50)), QSize(DX(400),DY(20))),
             Qt::AlignLeft,
             info->comment
             );

  if (!active)
  {
    p.setOpacity(1);
    p.setPen(Qt::red);
    p.drawText(QRect(info->rect.topLeft() + QPoint(DX(120),DY(50)), QSize(DX(100),DY(30))),
               Qt::AlignHCenter,
               tr("%1 left").arg(score - currentBonus)
               );
  }

}


void GameBonus::activateBonus(GameScene *scene, BonusInfo *info, int idx)
{
  int score = info->score;
  if (diff == 1) score /= 2;
  else if (diff == 3) score *= 2;

  int currentBonus = items.at(bonusId).count;
  if (score > currentBonus)
    return;

  items[bonusId].spendScore(score);

  sndEngine->playSound(GameSound::sndBigHammer);

  int cols = scene->numCols();
  int rows = scene->numRows();

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      PlaceInfo &pi = scene->data(i,j);
      if (pi.itemCanBeHighlighted())
      {
        if (pi.item->id() == bonusId)
        {
          switch (idx)
          {
            case 0:
              if (!pi.hasBlock())
                scene->removeAndCountItemOnly(i,j);
              break;

            case 1:
              scene->removeAndCountItem(i,j);
              break;

            case 2:
              scene->removeAndCountItem(i,j);
            break;

            case 3:
              scene->removeAndCountItem(i,j);
            break;

            case 4:
              scene->removeAndCountItem(i,j);
              break;
          }

          if (idx >= 0)
          {
            for (int k = 0; k < idx; k++)
            {
              pi.setUnprocessed();
              scene->removeAndCountItem(i,j);
            }

            const QPixmap &pm = GameItem::itemPixmap(bonusId);
            int cx = scene->col2x(j), ry = scene->row2y(i);

            scene->createPixmapPopup(cx, ry, 5, 5, pm, 25);
            if (idx > 0)
              scene->createPixmapPopup(cx, ry, -5, 5, pm, 25);
            if (idx > 1)
              scene->createPixmapPopup(cx, ry, -5, -5, pm, 25);
            if (idx > 2)
              scene->createPixmapPopup(cx, ry, 5, -5, pm, 25);
          }
        }
      }
    }
  }

}

int GameBonus::collectedOnLevel() const
{
  int s = 0;
  for (int i = 0; i < items.count(); i++)
  {
    s += items.at(i).levelCount;
  }
  return s;
}

int GameBonus::collectedTotal() const
{
  int s = 0;
  for (int i = 0; i < items.count(); i++)
  {
    s += items.at(i).totalCount;
  }
  return s;
}
