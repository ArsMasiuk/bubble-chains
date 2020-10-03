#ifndef MIXERTOOL_H
#define MIXERTOOL_H

#include "gametools.h"

class MixerTool : public GameTool
{
    Q_OBJECT
public:
    MixerTool(int x, int y, int score);
    virtual ~MixerTool() {}

    virtual ToolType type() const { return AutoTool; }

    virtual bool checkToolClick();
};

#endif // MIXERTOOL_H
