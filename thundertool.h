#ifndef THUNDERTOOL_H
#define THUNDERTOOL_H

#include "gametools.h"

class ThunderTool : public GameTool
{
  Q_OBJECT
public:
  ThunderTool(int x, int y, int score);
  virtual ~ThunderTool() {}

  virtual bool checkToolClick();

  ToolAction checkItemState(int row, int col);
  bool checkItemClick(int row, int col);

private:
  int myRow;
};

#endif // THUNDERTOOL_H
