#include <QApplication>

#include "gametools.h"

#include "scene_if.h"
#include "gamewidget.h"
#include "gamestock.h"
#include "gamebonus.h"
#include "gameprofile.h"
#include "gamesound.h"

#include "hammertool.h"
//#include "bighammertool.h"
//#include "smallhammertool.h"
#include "bombtool.h"
#include "thundertool.h"
#include "clocktool.h"
#include "randomkilltool.h"
#include "mixertool.h"
//#include "unblocktool.h"
#include "twintool.h"

#include "consttools.h"

#include "progressbar.h"

#include "particlecontrol.h"
#include "gamehintcontrol.h"

ToolSet * toolset = nullptr;

////////////////////////////////////////////////////////////////////////////////

#define TOOL_STAGES 20

int GameTool::stage;

GameTool::GameTool(int x, int y, int score) :
    myPos(x,y),
    myScore(score),
    myToolset(0),
    myProgress(0),
    myActive(false)
{
}

GameTool::GameTool(int x, int y, int score, const QString &resource) :
    myPos(x,y),
    myScore(score),
    myToolset(0),
    myProgress(0),
    myActive(false)
{
  myPixmap = QPixmap(GameWidget::getResourcePath() + "/tools/" + resource + ".png")
             .scaled(DX(48),DY(48), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

GameTool::~GameTool()
{
}

void GameTool::setCurrent()
{
  stage = TOOL_STAGES;
}

void GameTool::advanceAndPaint(QPainter &p, int currentScore)
{
  if (!myActive)
  {
//    if (currentScore >= myScore && !myToolset->isPuzzle())
//    {
//      setActive(true);
//      animateActivation();
//      myToolset->nextToolActivated();
//    }
    return;
  }

  p.setOpacity(currentScore >= myScore ? 1 : 0.5);

//  p.setPen(QPen(Qt::white));
//
//  p.drawText(QRect(myPos.x(), myPos.y()-DY(12), DX(48), DY(20)),
//             Qt::AlignHCenter | Qt::AlignTop, QString::number(myScore));

  if (myToolset->current() == this)
  {
    p.setOpacity((1.0/TOOL_STAGES) * stage);
    if (!--stage) stage = TOOL_STAGES;
  }

  p.drawPixmap(myPos, myPixmap);
}

void GameTool::animateActivation()
{
  scene->createPixmapPopup(myPos.x(), myPos.y(), -1, -1, myPixmap, 20);
  scene->createPixmapPopup(myPos.x(), myPos.y(), 1, 1, myPixmap, 20);
  scene->createPixmapPopup(myPos.x(), myPos.y(), -1, 1, myPixmap, 20);
  scene->createPixmapPopup(myPos.x(), myPos.y(), 1, -1, myPixmap, 20);
}

QString GameTool::hint() const
{
  return QString("%1: %2").arg(bonusInfo->name, bonusInfo->comment);
}

////////////////////////////////////////////////////////////////////////////////

#define MAX_TOOL_SCORE  100

struct ToolSetPrivate
{
  ToolSetPrivate() : myActiveToolIndex(0), maxCount(MAX_TOOL_SCORE)
  {
    QLinearGradient gr2(0,0,40,0);
    gr2.setColorAt(0, QColor(0x034912));
    gr2.setColorAt(0.5, QColor(0x00ff07));
    gr2.setColorAt(1, QColor(0x034912));

    countBar = new ProgressBar(QRect(30,60, 40,630));
    countBar->setUpSpeed(10);
    countBar->setBrush(gr2);

    countBar->setMax(maxCount);
  }

  ~ToolSetPrivate()
  {
    delete countBar;
  }

  int myActiveToolIndex;
  int maxCount;

  ProgressBar *countBar;

  HammerTool *hammerTool;
  BombTool *bombTool;
  ThunderTool *thunderTool;
  ClockTool *clockTool;
  RandomKillTool *randomKillTool;
  MixerTool *mixerTool;
  TwinTool *twinTool;
  //class MagBonus *magBonus;
  //class TimerBonus *timerBonus;
  //class ScoreBonus *scoreBonus;
  //class ClockBonus *clockBonus;
};

////////////////////////////////////////////////////////////////////////////////

ToolSet::ToolSet() : toolScore(0), currentTool(0)
{
  priv = new ToolSetPrivate();

  initGraphics();

  // active by default
//  smallHammerTool->setActive();
//  unblockTool->setActive();
}

ToolSet::~ToolSet()
{
  qDeleteAll(tools);
}

//int ToolSet::bonusClock() const { return clockBonus->value(); }
//int ToolSet::bonusScore() const { return scoreBonus->value(); }
//int ToolSet::bonusTimer() const { return timerBonus->value(); }
//int ToolSet::bonusMag() const { return magBonus->value(); }

void ToolSet::initGraphics()
{
//  int magBonusVal = 0;
//  int timerBonusVal = 0;
//  int clockBonusVal = 0;
//  int scoreBonusVal = 0;

  //QList<bool> activ;

  if (tools.count())
  {
//    magBonusVal = magBonus->value();
//    timerBonusVal = timerBonus->value();
//    clockBonusVal = clockBonus->value();
//    scoreBonusVal = scoreBonus->value();

//    for (int i = 0; i < tools.count(); i++)
//      activ.append(tools.at(i)->isActive());

    qDeleteAll(tools);
    tools.clear();
  }

  int x1 = DX(26);
  int y1 = DY(6);

  priv->clockTool = new ClockTool(x1, y1, MAX_TOOL_SCORE);
  priv->mixerTool = new MixerTool(x1, y1, MAX_TOOL_SCORE);
  priv->hammerTool = new HammerTool(x1, y1, MAX_TOOL_SCORE);
  priv->bombTool = new BombTool(x1, y1, MAX_TOOL_SCORE);
  priv->randomKillTool = new RandomKillTool(x1, y1, MAX_TOOL_SCORE);
  priv->thunderTool = new ThunderTool(x1, y1, MAX_TOOL_SCORE);
  priv->twinTool = new TwinTool(x1, y1, MAX_TOOL_SCORE);


  // permanent tools
//  y += DY(100);
//  magBonus = new MagBonus(x1,y, 10000);
//  timerBonus = new TimerBonus(x2,y, 12000);
//  y += dy;
//  clockBonus = new ClockBonus(x1,y, 15000);
//  scoreBonus = new ScoreBonus(x2,y, 20000);

  // order in which the tools are given
  tools.append(priv->clockTool);
  tools.append(priv->hammerTool);
  tools.append(priv->bombTool);
  tools.append(priv->mixerTool);
  tools.append(priv->randomKillTool);
  tools.append(priv->thunderTool);
  tools.append(priv->twinTool);

//  tools.append(magBonus);
//  tools.append(timerBonus);
//  tools.append(clockBonus);
//  tools.append(scoreBonus);

  // update bonus values
//  magBonus->setValue(magBonusVal);
//  timerBonus->setValue(timerBonusVal);
//  clockBonus->setValue(clockBonusVal);
//  scoreBonus->setValue(scoreBonusVal);

  for (int i = 0; i < tools.count(); i++)
  {
    GameTool *tool = tools.at(i);
    tool->setBase(this);
    tool->setActive(priv->myActiveToolIndex == i);
  }
}

void ToolSet::readProfile(LevelPackInfo *lpi)
{
  // this is to enable animation
  setScore(0);
  setScore(lpi->toolScore);

  currentTool = 0;

  priv->myActiveToolIndex = lpi->activeToolIndex;
  if (priv->myActiveToolIndex < 0 && priv->myActiveToolIndex >= tools.count())
    priv->myActiveToolIndex = 0;

  for (int i = 0; i < tools.count(); i++)
  {
    tools.at(i)->setActive(priv->myActiveToolIndex == i);
  }

//  scoreBonus->setValue(lpi->score_bonus);
//  clockBonus->setValue(lpi->time_bonus);
//  timerBonus->setValue(lpi->speed_bonus);
//  magBonus->setValue(lpi->speed_mag_bonus);
}

void ToolSet::writeProfile(LevelPackInfo *lpi)
{
  lpi->toolScore = toolScore;
  lpi->activeToolIndex = priv->myActiveToolIndex;

//  lpi->score_bonus = scoreBonus->value();
//  lpi->time_bonus = clockBonus->value();
//  lpi->speed_bonus = timerBonus->value();
//  lpi->speed_mag_bonus = magBonus->value();
}

void ToolSet::activateRandomTool()
{
  priv->myActiveToolIndex = qrand() % tools.count();

  for (int i = 0; i < tools.count(); i++)
  {
    tools.at(i)->setActive(priv->myActiveToolIndex == i);
  }
}

void ToolSet::updateScore()
{
  static bool activated = false;

  toolScore = (int)priv->countBar->value();

  if (priv->countBar->isFull())
  {
    if (!activated)
    {
      activated = true;
      tools.at(priv->myActiveToolIndex)->animateActivation();

      sndEngine->playSound(GameSound::sndTool);

      //hintControl->scheduleHint(hintControl->ToolsHint);
      hintControl->scheduleHint(hintControl->ToolHint[priv->myActiveToolIndex]);
    }
  } else
    activated = false;
}

void ToolSet::addScore(int score)
{
  priv->countBar->addValue(score);
  updateScore();
}

void ToolSet::setScore(int score)
{
  priv->countBar->setValue(score);
  updateScore();
}

void ToolSet::setLongestChain(int value)
{
  priv->maxCount = MAX_TOOL_SCORE - (value-3)*2;
  priv->countBar->setMax(priv->maxCount);

  for (int i = 0; i < tools.count(); i++)
  {
    GameTool *tool = tools.at(i);
    tool->setScore(priv->maxCount);
  }

  updateScore();
}

//void ToolSet::nextToolActivated()
//{
//  if (puzzle || next_tool == ALL_TOOLS)
//    return;
//
//  if (next_tool < tools.count())
//  {
//    GameTool *tool = tools.at(next_tool);
//    tool->setActive(true);
//    tool->animateActivation();
//    sndEngine->playSound(GameSound::sndTool);
//
//    next_tool++;
//  }
//  else
//    next_tool = ALL_TOOLS;
//}

//int ToolSet::activeToolIndex(int score)
//{
//  int idx = 2;
//  if (puzzle) return idx;
//
//  while (idx < tools.count() && tools.at(idx)->score() <= score)
//    idx++;
//  if (idx == tools.count())
//    idx = 0;
//  return idx;
//}

void ToolSet::updateTools(QPainter &p)
{
  // countBar
  priv->countBar->paint(p);

  // tool
  p.setFont(gameStock->Font8);

  tools.at(priv->myActiveToolIndex)->advanceAndPaint(p, toolScore);

//  for (int i = 0; i < tools.count(); i++)
//  {
//    GameTool *tool = tools.at(i);
//    tool->advanceAndPaint(p, toolScore);
//  }
}

bool ToolSet::checkMouseActions(const QPoint &pos)
{
  for (int i = 0; i < tools.count(); i++)
  {
    GameTool *tool = tools.at(i);
    if (!tool->isActive())
      continue;

    if (QRect(tool->pos(), QSize(DX(48),DY(48))).contains(pos))
    {
      if (tool == currentTool)
        break;

      if (tool->score() <= toolScore)
      {
        currentTool = tool;
        currentTool->setCurrent();
        QApplication::setOverrideCursor(currentTool->pixmap());

        if (tool->type() != GameTool::ManualTool)
        {
          if (tool->checkToolClick())
          {
            toolAboutExecute();

            if (!tool->inProgress())
            {
              toolExecuted();
            }
            return true;
          }
        }
        else  // manual tool
        {

        }

        myOverRow = myOverCol = -1;
        return true;
      }

      break;
    }
  }

  currentTool = 0;
  scene->setDefaultGameCursor();
  return false;
}

void ToolSet::progressCurrent()
{
  if (!(currentTool->checkToolClick()))
  {
    toolExecuted();
  }
}

void ToolSet::toolAboutExecute()
{
  setScore(0);

  // emit particles
  ParticleEmitter *pe = new ParticleEmitter(DX(40), DY(350));
  pe->setDuration(80);
  pe->setCount(40);
  pe->setEmitRadius(DX(20), DY(350));
  pe->setOpacityShift(0.3);
  pe->setCreator(new UniformParticleCreator(3));  // green
  partControl->addEmitter(pe);
}

void ToolSet::toolExecuted()
{
  myOverRow = myOverCol = -1;
  currentTool = 0;
  setScore(0);
  activateRandomTool();
  scene->setDefaultGameCursor();
}

bool ToolSet::checkItemClick(int row, int col)
{
  if (!currentTool->checkItemClick(row, col))
    return false;

  toolAboutExecute();

  if (!currentTool->inProgress())
    toolExecuted();

  return true;
}

GameTool::ToolAction ToolSet::checkItemState(int row, int col)
{
  if (!currentTool || myOverCol == -1 || myOverRow == -1)
    return GameTool::ToolOutOfRange;

  if (currentTool->inProgress())
    return GameTool::ToolOutOfRange;

  return currentTool->checkItemState(row, col);
}

void ToolSet::checkMouseMoved(int row, int col)
{
  if (col >= 0 && col < scene->numCols() && row >= 0 && row < scene->numRows())
  {
    myOverRow = row;
    myOverCol = col;
  }
  else
    myOverRow = myOverCol = -1;
}

bool ToolSet::checkMouseHover(const QPoint &pos)
{
  for (int i = 0; i < tools.count(); i++)
  {
    GameTool *tool = tools.at(i);

    if (!tool->isActive())
      continue;

    if (QRect(tool->pos(), QSize(DX(48),DY(48))).contains(pos))
    {
      scene->showHint(tool->hint());
      return true;
    }
  }

  return false;
}
