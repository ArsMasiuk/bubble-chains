#ifndef GAMETARGETS_H
#define GAMETARGETS_H

#include "gametargetcontrol.h"

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////

class GameBasicTarget : public GameTarget
{
public:
    GameBasicTarget();

    static void init();

    virtual QString hint() const
    { return QObject::tr("Basic Target: can be filled by any type of chains."); }

protected:
    static QPixmap *pixOn, *pixOff;
};

////////////////////////////////////////////////////////////////////////////////

class GameNumberTarget : public GameTarget
{
public:
    GameNumberTarget(int id);

    virtual void repaint();

    static void init();

    virtual CheckResult checkChain(int size, int id, int score);

    virtual QString hint() const
    { return QObject::tr("Number Target: can be filled only by chains not shorter than %1 items.").arg(myId); }

protected:
    int myId;

    static QPixmap *pixOn, *pixOff;
};

////////////////////////////////////////////////////////////////////////////////

class GameColorTarget : public GameTarget
{
public:
    GameColorTarget(int id);

    static void init();

    virtual CheckResult checkChain(int size, int id, int score);

    virtual QString hint() const
    { return QObject::tr("Color Target: can be filled only by chains of the appropriate color."); }

protected:
    int myId;

    static QPixmap *pixOn[6], *pixOff[6];
};

////////////////////////////////////////////////////////////////////////////////

class GameFallTarget : public GameTarget
{
public:
    GameFallTarget();
    virtual ~GameFallTarget();

    static void init();

    virtual CheckResult checkChain(int size, int id, int score);

    virtual QString hint() const
    { return QObject::tr("Fall Target: can be filled only by Falling items which reached the bottom line."); }

protected:
    static QPixmap *pixOn, *pixOff;
};

////////////////////////////////////////////////////////////////////////////////

#endif // GAMETARGETS_H
