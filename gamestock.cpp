#include "gamestock.h"
#include "gameitem.h"

#include "scaler.h"
#include "scene_if.h"

GameStock * gameStock = 0;

GameStock::GameStock()
{
  initGraphics();
}

void GameStock::initGraphics()
{
  Font8 = QFont("helvetica", qMax(DY(8), 8));
  Font12 = QFont("helvetica", DY(12));
  Font16 = QFont("helvetica", DY(16));
  Font20 = QFont("helvetica", DY(20));
  QFontMetrics fm20(Font20);
  if (fm20.boundingRect("0000000").width() > DX(120))
      Font20.setPointSize(DY(16));
  Font40 = QFont("helvetica", DY(40));
  Font60 = QFont("helvetica", DY(60));

  GameCursor = QCursor(QPixmap(":/images/cursor_wand.png")
                       .scaled(DX(32),DY(32), Qt::IgnoreAspectRatio, Qt::SmoothTransformation), 0, 0);

  GameLogoBig = QPixmap(":/images/logo.png");
  MenuBackground = QPixmap(":/images/bg_menu.jpg")
                   .scaled(WIDTH,HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


  scene->loadPixmap(Concrete, "con1.png", X56,Y56);

  scene->loadPixmap(Clock, "clock.png", X56,Y56);
  scene->loadPixmap(Score, "score.png", X56,Y56);

  scene->loadPixmap(Ok, "button_ok.png", X56,Y56);
  scene->loadPixmap(Cancel, "button_cancel.png", X56,Y56);

  scene->loadPixmap(ItemJocker, "item_jocker.png", X48,Y48);
  scene->loadPixmap(ItemFaller, "item_faller.png", X48,Y48);

  Items.clear();
  for (int i = 0; i < MAX_ITEMS_COUNT; i++) {
    QString name = QString("item%1.png").arg(i+1);
    QPixmap pm;
    scene->loadPixmap(pm, name, X48,Y48);
    Items.append(pm);
  }

}
