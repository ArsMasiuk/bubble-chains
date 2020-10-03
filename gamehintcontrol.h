#ifndef GAMEHINTCONTROL_H
#define GAMEHINTCONTROL_H

#include "hintcontrol.h"

struct PlayerInfo;

class GameHintControl : public HintControl
{
Q_OBJECT
public:
    explicit GameHintControl(QGraphicsScene *parent = 0);

    void activateHints(const QStringList &activeHints, bool on = true);
    void activateAllHints(bool on = true);

    void scheduleHint(HintInfo &hint);

    virtual bool mouseClick(QPoint pos, int buttons);

    void readProfile(PlayerInfo *pl);
    void writeProfile(PlayerInfo *pl);

    HintInfo BaseHint, NumberHint, ColorHint, FallerHint, LevelsHint;

    HintInfo AimHint, TimeHint, PlayHint, JockerHint, TargetHint, ToolBarHint;

    HintInfo ToolsHint, LongestChainHint;
    HintInfo ToolHint[7];

private:
    QMap<QString, HintInfo*> hintMap;
};

#endif // GAMEHINTCONTROL_H
