#ifndef CLOCKTOOL_H
#define CLOCKTOOL_H

#include "gametools.h"

class ClockTool : public GameTool
{
    Q_OBJECT
public:
    ClockTool(int x, int y, int score);
    virtual ~ClockTool() {}

    virtual ToolType type() const { return AutoTool; }

    virtual bool checkToolClick();
};

#endif // CLOCKTOOL_H
