#ifndef TWINKILLTOOL_H
#define TWINKILLTOOL_H

#include "gametools.h"

class TwinTool : public GameTool
{
  Q_OBJECT
public:
  TwinTool(int x, int y, int score);
  virtual ~TwinTool() {}

  ToolAction checkItemState(int row, int col);
  bool checkItemClick(int row, int col);
};

#endif // TWINKILLTOOL_H
