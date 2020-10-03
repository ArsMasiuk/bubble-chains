#ifndef SCENEPRIVATE_H
#define SCENEPRIVATE_H

#include <QtCore>

////////////////////////////////////////////////////////////////////////////////

class ScenePrivate
{
  friend class GameScene;

  QList<QPoint> chain;
  int chainId;
  int longest_chain;

  int newItemFlags;

  class ProgressBar *timeBar;

  class GamePanelControl *panelControl;
  class LevelWinDialog *levelWinDialog;
  class LevelFailDialog *levelFailDialog;
  class GameWinDialog *gameWinDialog;

  ScenePrivate();
  ~ScenePrivate();

  void init()
  {
    chain.clear();
  }
};

////////////////////////////////////////////////////////////////////////////////

#endif // SCENEPRIVATE_H
