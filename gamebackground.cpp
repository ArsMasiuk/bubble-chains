#include "gamebackground.h"
#include "gamewidget.h"
#include "gameprofile.h"

GameBackground * gameBackground = 0;

enum BgBlend { BB_NONE, BB_ALPHA, BB_BLOCKS };

struct BgPrivate
{
  QString bgPath;
  QStringList bgList;
  int currentBgIdx, transBgIdx;
  QPixmap currentBg, transBg;
  int frame;
  BgBlend blend;

  BgPrivate() : blend(BB_ALPHA)
  {
  }

  void init()
  {
    frame = 0;
    currentBgIdx = transBgIdx = -1;
    bgPath = GameWidget::getResourcePath() + "schemes/" + gameProfile->currentTheme();
    QStringList ext; ext << "bg*.png" << "bg*.jpg";
    bgList = QDir(bgPath).entryList(ext);

    // read current and trans
    if (bgList.isEmpty())
      return;

    currentBgIdx = qrand() % bgList.count();
    currentBg = QPixmap(bgPath + "/" + bgList.at(currentBgIdx));
    loadTransBg();
  }

  void loadTransBg()
  {
    if (bgList.isEmpty())
      return;

    if (bgList.count() == 1)
      transBgIdx = 0;
    else
      do {
        transBgIdx = qrand() % bgList.count();
      } while (currentBgIdx == transBgIdx);

    transBg = QPixmap(bgPath + "/" + bgList.at(transBgIdx));
  }

  void draw(QPainter &p, const QRect &r)
  {
    p.setOpacity(1);
    p.drawPixmap(r, currentBg);

    if (++frame < 100)
      return;

      switch (blend)
      {
        case BB_ALPHA:
          if (frame < 480)
            return;
          if (frame >= 480 && frame < 500)
          {
            p.setOpacity((double)(frame-480)/20.0);
            p.drawPixmap(0,0,transBg);
            return;
          }
          break;

        case BB_BLOCKS:
          if (frame < 500)
          {
            //p.setOpacity(1);
            for (int i = 0; i < 10; i++)
            {
              QPainter p1(&currentBg);
              int x = qrand() % 64;
              int y = qrand() % 48;
              p1.drawPixmap(x*16,y*16, transBg, x*16,y*16,16,16);
              return;
            }

          }
          break;

        default: ;
      }

      p.drawPixmap(0,0,transBg);
      frame = 0;
      currentBg = transBg;
      currentBgIdx = transBgIdx;
      loadTransBg();
    }

};

GameBackground::GameBackground()
{
  priv = new BgPrivate();
}

void GameBackground::readProfile(PlayerInfo */*pl*/)
{
  priv->init();
}

void GameBackground::draw(QPainter &p, int /*frame*/, const QRect &r)
{
  priv->draw(p, r);
}

