#ifndef GAMEDIALOGS_H
#define GAMEDIALOGS_H

#include "gamepanel.h"

////////////////////////////////////////////////////////////////////////////////

class LevelWinDialog : public GamePanel
{
  Q_OBJECT
public:
  LevelWinDialog(class GamePanelControl *panelControl);

  void setScore(int score_) { score = score_; }
  void setTime(int time_) { time = time_; }
  void setLevel(int level_) { level = level_; }

signals:
  void okClicked();
  void cancelClicked();

protected:
  virtual void paintContent(QPainter &p);
  virtual void subcontrolClicked(int id);

  class GameButton *buttonOk, *buttonCancel;

  int score, time, level;
};

////////////////////////////////////////////////////////////////////////////////

class LevelFailDialog : public LevelWinDialog
{
  Q_OBJECT
public:
  LevelFailDialog(class GamePanelControl *panelControl);

protected:
  virtual void paintContent(QPainter &p);
};

////////////////////////////////////////////////////////////////////////////////

class GameWinDialog : public LevelWinDialog
{
  Q_OBJECT
public:
  GameWinDialog(class GamePanelControl *panelControl);

protected:
  virtual void paintContent(QPainter &p);
};

////////////////////////////////////////////////////////////////////////////////

#endif // GAMEDIALOGS_H
