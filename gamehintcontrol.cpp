#include "gamehintcontrol.h"
#include "gameprofile.h"
#include "particlecontrol.h"

extern class ParticleControl *partControl;

GameHintControl::GameHintControl(QGraphicsScene *parent) :
    HintControl(parent)
{
  const int w1 = 420, h1 = 280;
  const int y1 = 240;

  BaseHint.content = tr(
      "<img src=':/images/img64_base.png' align='right' width='64' height='64'>"
      "<span class='head'>Base Targets</span><br><br>"
      "Now, try to start with Base Targets.<br><br>"
      "These targets are destroyed when a certain amount of points is earned.<br><br>"
      "Every collected chain gives you some points depending on its lenght - "
      "the longer chain is, the more points you get.<br><br>"
      "Neither color nor minimal lenght are important for earning points."
      );
  BaseHint.size = QSize(w1,h1);
  BaseHint.position = QPoint((1024-w1)/2, y1);
  hintMap["BaseHint"] = &BaseHint;


  NumberHint.content = tr(
      "<img src=':/images/img64_number.png' align='right' width='64' height='64'>"
      "<span class='head'>Number Targets</span><br><br>"
      "Points for these targets are earned only when a collected chain "
      "has its lenght equal of more than the number on the target.<br><br>"
      "Color of the chains is not important for earning points."
      );
  NumberHint.size = QSize(w1,h1);
  NumberHint.position = QPoint((1024-w1)/2, y1);
  hintMap["NumberHint"] = &NumberHint;


  ColorHint.content = tr(
      "<img src=':/images/img64_color.png' align='right' width='64' height='64'>"
      "<span class='head'>Color Targets</span><br><br>"
      "Points for these targets are earned only when a collected chain "
      "has its color equal to target's color.<br><br>"
      "Lenght of the chains is not important for earning points."
      );
  ColorHint.size = QSize(w1,h1);
  ColorHint.position = QPoint((1024-w1)/2, y1);
  hintMap["ColorHint"] = &ColorHint;


  FallerHint.content = tr(
      "<img src=':/images/img64_faller.png' align='right' width='64' height='64'>"
      "<span class='head'>Fall Targets</span><br><br>"
      "Fall Target is destroyed only when a Falling Item on the field "
      "reaches the very bottom line.<br><br>"
      "Unlike other items, Falling Items can not be combined into chains."
      );
  FallerHint.size = QSize(w1,h1);
  FallerHint.position = QPoint((1024-w1)/2, y1);
  hintMap["FallerHint"] = &FallerHint;


  LevelsHint.content = tr(
      "<span class='head'>Random Targets</span><br><br>"
      "Ok, since now for every level you'll need to solve random targets.<br><br>"
      "GOOD LUCK! :)"
      );
  LevelsHint.size = QSize(w1,h1-40);
  LevelsHint.position = QPoint((1024-w1)/2, y1);
  hintMap["LevelsHint"] = &LevelsHint;

  //////////////////////////////////////////////////////////////////////////////

  LongestChainHint.content = tr(
      "<span class='head'>Longest Chain!</span><br><br>"
      "You've just collected the longest chain.<br><br>"
      "Longest chains allow you getting tools faster.<br>"
      "The longer are chains you're collecting, the less points you need for every new tool.<br><br>"
      "Once collected, it works permanently if you'll win the level."
      );
  LongestChainHint.size = QSize(440, 220);
  LongestChainHint.position = QPoint(100, 768-240);
  hintMap["LongestChainHint"] = &LongestChainHint;

  //////////////////////////////////////////////////////////////////////////////

  AimHint.content = tr(
      "<span class='head'>Aim of the game</span><br><br>"
      "The aim of Bubble Chains is to remove all of the targets on each level.<br><br>"
      "Keep doing this until you have passed the last level and won the game."
      );
  AimHint.size = QSize(w1,h1);
  AimHint.position = QPoint((1024-w1)/2, y1);
  AimHint.blocking = true;
  hintMap["AimHint"] = &AimHint;

  TimeHint.content = tr(
      "<span class='head'>Time is limited</span><br><br>"
      "The time bar shows how much time you have to pass the level.<br><br>"
      "If you don't remove all the targets in time, the level will restart."
      );
  TimeHint.size = QSize(300,200);
  TimeHint.position = QPoint(1024-300-100, y1);
  TimeHint.blocking = true;
  hintMap["TimeHint"] = &TimeHint;

  PlayHint.content = tr(
      "<span class='head'>How to play</span><br><br>"
      "You have to collect color bubbles on the field by combining them into chains of three or more bubbles.<br><br>"
      "To do this, press left mouse button on a bubble and drag the mouse over the surrounding bubbles of the same color."
      "When all the desired bubbles are connected, release the button, and the chain will disappear.<br><br>"
      "Do not release the button until the whole chain is combined. If you want to cancel current combination, just drag the mouse over the bubbles back."
      );
  PlayHint.size = QSize(460,320);
  PlayHint.position = QPoint((1024-460)/2, y1);
  PlayHint.blocking = true;
  hintMap["PlayHint"] = &PlayHint;

  JockerHint.content = tr(
      "<span class='head'>Multi-Items</span><br><br>"
      "Except of the color bubbles, there are special items which can be used as a placeholder for any bubble, making your life easier.<br><br>"
      "<center><img src=':/images/img64_jocker.png'></center>"
      );
  JockerHint.size = QSize(w1,h1);
  JockerHint.position = QPoint((1024-w1)/2, y1);
  JockerHint.blocking = true;
  hintMap["JockerHint"] = &JockerHint;

  TargetHint.content = tr(
      "<span class='head'>Targets</span><br><br>"
      "There are several targets of different types assigned randomly for each level.<br><br>"
      "Targets are initially empty, but getting filled when you're collecting chains of the corresponding type.<br><br>"
      "A filled target disappears - so when all of them are gone, the level is won!"
      );
  TargetHint.size = QSize(w1,h1);
  TargetHint.position = QPoint((1024-w1)/2, y1+160);
  TargetHint.blocking = true;
  hintMap["TargetHint"] = &TargetHint;

  ToolBarHint.content = tr(
      "<span class='head'>Score Bar</span><br><br>"
      "The score bar represents the amount of the score needed for activating a tool. "
      "Tools can help you pass the level significantly faster.<br><br>"
      "A tool is activated when the bar is full.<br><br>"
      "Just collect color chains, and the bar will be rising until it reaches its top.<br><br>"
      );
  ToolBarHint.size = QSize(w1,240);
  ToolBarHint.position = QPoint(100, y1);
  ToolBarHint.blocking = true;
  hintMap["ToolBarHint"] = &ToolBarHint;

  ToolsHint.content = tr(
      "<span class='head'>Tools</span><br><br>"
      "When the score bar is full - you can use the tool.<br><br>"
      "Tools are helping you to pass levels faster.<br><br>"
      "To use the tool, click on it. Some tools are used immediately; "
      "the others you have to point into the field."
      );
  ToolsHint.size = QSize(400,220);
  ToolsHint.position = QPoint(100, 20);
  ToolsHint.blocking = true;
  hintMap["ToolsHint"] = &ToolsHint;

//////////////////////////////////////////////////////////////////////////////

  ToolHint[0].content = tr(
      "<span class='head'>Clock</span><br><br>"
      "Adds +60 seconds to the level timer."
      );
  ToolHint[0].size = QSize(300,100);
  ToolHint[0].position = QPoint(100, 20);
  hintMap["ToolHint0"] = &ToolHint[0];

  ToolHint[1].content = tr(
      "<span class='head'>Item Killer</span><br><br>"
      "Destroys a single item in the field.<br><br>"
      "Point the tool over an item you want to destroy and click left mouse button."
      );
  ToolHint[1].size = QSize(300,180);
  ToolHint[1].position = QPoint(100, 20);
  hintMap["ToolHint1"] = &ToolHint[1];

  ToolHint[2].content = tr(
      "<span class='head'>Explosion</span><br><br>"
      "Explodes a region of 9 cells.<br><br>"
      "Point the tool over items you want to destroy and click left mouse button."
      );
  ToolHint[2].size = QSize(300,180);
  ToolHint[2].position = QPoint(100, 20);
  hintMap["ToolHint2"] = &ToolHint[2];

  ToolHint[3].content = tr(
      "<span class='head'>Mixer</span><br><br>"
      "Randomly shuffles items in the field."
      );
  ToolHint[3].size = QSize(300,100);
  ToolHint[3].position = QPoint(100, 20);
  hintMap["ToolHint3"] = &ToolHint[3];

  ToolHint[4].content = tr(
      "<span class='head'>Bolt</span><br><br>"
      "Randomly removes some items from the field."
      );
  ToolHint[4].size = QSize(300,100);
  ToolHint[4].position = QPoint(100, 20);
  hintMap["ToolHint4"] = &ToolHint[4];

  ToolHint[5].content = tr(
      "<span class='head'>Row Blaster</span><br><br>"
      "Destroys all items in a row.<br><br>"
      "Point the tool over the row you want to destroy and click left mouse button."
      );
  ToolHint[5].size = QSize(300,180);
  ToolHint[5].position = QPoint(100, 20);
  hintMap["ToolHint5"] = &ToolHint[5];

  ToolHint[6].content = tr(
      "<span class='head'>Twin Blaster</span><br><br>"
      "Destroys all the same-type items in the field.<br><br>"
      "Point the tool over the item of the type you want to destroy and click left mouse button."
      );
  ToolHint[6].size = QSize(300,200);
  ToolHint[6].position = QPoint(100, 20);
  hintMap["ToolHint6"] = &ToolHint[6];
}


void GameHintControl::activateHints(const QStringList &activeHints, bool on)
{
  //activateAllHints(!on);

  for (int i = 0; i < activeHints.count(); i++)
  {
    if (hintMap.contains(activeHints.at(i)))
      hintMap[activeHints.at(i)]->active = on;
  }
}

void GameHintControl::activateAllHints(bool on)
{
  QMap<QString, HintInfo*>::iterator it, it_end = hintMap.end();
  for (it = hintMap.begin(); it != it_end; it++)
  {
    it.value()->active = on;
  }
}

void GameHintControl::scheduleHint(HintInfo &hint)
{
  HintControl::scheduleHint(hint);

  // do automatic hint deactivation
  hint.active = false;

  // update profile
  writeProfile(gameProfile->currentPlayer());
}

bool GameHintControl::mouseClick(QPoint pos, int buttons)
{
  // explode region
  for (int i = 0; i < activeHints.count(); i++)
  {
    HintPainter *hw = activeHints.at(i);

    ParticleEmitter *pe = new ParticleEmitter(hw->rect());
    pe->setDuration(20);
    pe->setCount(150);
    pe->setOpacityShift(0.5);
    pe->setCreator(new UniformParticleCreator(0));  // blue
    partControl->addEmitter(pe);
  }

  return HintControl::mouseClick(pos, buttons);
}

void GameHintControl::readProfile(PlayerInfo *pl)
{
  activateAllHints(true);
  activateHints(pl->inactiveHints, false);
}

void GameHintControl::writeProfile(PlayerInfo *pl)
{
  pl->inactiveHints.clear();

  QMap<QString, HintInfo*>::iterator it, it_end = hintMap.end();
  for (it = hintMap.begin(); it != it_end; it++)
  {
    if (it.value()->active == false)
      pl->inactiveHints.append(it.key());
  }
}
