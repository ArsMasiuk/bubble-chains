#ifndef BOMBTOOL_H
#define BOMBTOOL_H

#include "gametools.h"

class BombTool : public GameTool
{
  Q_OBJECT
public:
  BombTool(int x, int y, int score);
  virtual ~BombTool() {}

  ToolAction checkItemState(int row, int col);
  bool checkItemClick(int row, int col);
};

#endif // BOMBTOOL_H
