#ifndef HAMMERTOOL_H
#define HAMMERTOOL_H

#include "gametools.h"

class HammerTool : public GameTool
{
    Q_OBJECT
public:
  HammerTool(int x, int y, int score);
  virtual ~HammerTool() {}

  ToolAction checkItemState(int row, int col);
  bool checkItemClick(int row, int col);
};

#endif // HAMMERTOOL_H
