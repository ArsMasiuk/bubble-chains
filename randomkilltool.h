#ifndef RANDOMKILLTOOL_H
#define RANDOMKILLTOOL_H

#include "gametools.h"

class RandomKillTool : public GameTool
{
    Q_OBJECT
public:
  RandomKillTool(int x, int y, int score);
  virtual ~RandomKillTool() {}

  virtual ToolType type() const { return AutoTool; }

  virtual bool checkToolClick();
};

#endif // RANDOMKILLTOOL_H
