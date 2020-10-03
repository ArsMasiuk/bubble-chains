#include "gamescene.h"
#include "gameitem.h"
#include "baseitem.h"
#include "gametools.h"
#include "gamemenu.h"
#include "gamewidget.h"
#include "gamesound.h"
#include "gameprofile.h"
#include "gamestock.h"
#include "gamebonus.h"
#include "gamebackground.h"
#include "gametargetcontrol.h"
#include "gametargets.h"
#include "gamepanelcontrol.h"
#include "gamedialogs.h"
#include "sceneprivate.h"
#include "progressbar.h"
#include "gamehintcontrol.h"
#include "particlecontrol.h"

////////////////////////////////////////////////////////////////////////////////

ParticleControl *partControl = 0;
GameHintControl *hintControl = 0;

#define DATA data

GameScene::GameScene(QWidget *parent) :
    QGraphicsScene(parent),
    myLock(false)
{
  // stock initialization
  GameStock::init();


  inputDisabled = true;

  priv = new ScenePrivate();

  // dialogs
  priv->panelControl = new GamePanelControl(this);

  priv->levelWinDialog = new LevelWinDialog(priv->panelControl);
  connect(priv->levelWinDialog, SIGNAL(okClicked()), this, SLOT(runNextLevel()));
  connect(priv->levelWinDialog, SIGNAL(cancelClicked()), this, SLOT(exitToMainMenu()));

  priv->levelFailDialog = new LevelFailDialog(priv->panelControl);
  connect(priv->levelFailDialog, SIGNAL(okClicked()), this, SLOT(runNextLevel()));
  connect(priv->levelFailDialog, SIGNAL(cancelClicked()), this, SLOT(exitToMainMenu()));

  priv->gameWinDialog = new GameWinDialog(priv->panelControl);
  connect(priv->gameWinDialog, SIGNAL(cancelClicked()), this, SLOT(exitToMainMenu()));

//  dconfirm = 0;

  // hint control
  hintControl = new GameHintControl(this);

  // particle control
  partControl = new ParticleControl();

  // menu
  menu = new MenuWidget();
  addWidget(menu);
  menu->activate();

  connect(menu, SIGNAL(menuNew()), this, SLOT(on_menuNew()));
  connect(menu, SIGNAL(menuContinue()), this, SLOT(on_menuContinue()));
  connect(menu, SIGNAL(menuExit()), this, SLOT(on_menuExit()));
  connect(menu, SIGNAL(menuPauseBack()), this, SLOT(on_menuPauseBack()));
  connect(menu, SIGNAL(menuRestartLevel()), this, SLOT(on_menuRestartLevel()));
  connect(menu, SIGNAL(menuAbandonGame()), this, SLOT(on_menuAbandonGame()));
  connect(menu, SIGNAL(menuThemeChanged()), this, SLOT(on_menuThemeChanged()));
  connect(menu, SIGNAL(menuGameStart()), this, SLOT(on_menuGameStart()));
  connect(menu, SIGNAL(menuLevelPack()), this, SLOT(on_menuLevelPack()));

  connect(gameProfile, SIGNAL(profileChanged()), this, SLOT(initProfile()));

  // confirm dialogs
  dconfirm = new ConfirmDialog();
  addWidget(dconfirm, Qt::FramelessWindowHint);
  dconfirm->hide();

  // timers

  advanceTimer = new QTimer(this);
  advanceTimer->setInterval(25);
  connect(advanceTimer, SIGNAL(timeout()), this, SLOT(nextCycle()));

  timeTimer = new QTimer(this);
  timeTimer->setInterval(1000);
  connect(timeTimer, SIGNAL(timeout()), this, SLOT(countTime()));

  bonusTimer = new QTimer(this);
  bonusTimer->setInterval(500);
  connect(bonusTimer, SIGNAL(timeout()), this, SLOT(countBonusTime()));

  hintTimer = new QTimer(this);
  hintTimer->setInterval(10000);
  connect(hintTimer, SIGNAL(timeout()), this, SLOT(hintAvailableMoves()));

  // init components
  toolset = new ToolSet();

  gameBonus = new GameBonus();

  targetControl = new GameTargetControl();

  setSceneRect(0,0, WIDTH, HEIGHT);

  // set background
  gameBackground = new GameBackground();
  setBackgroundBrush(Qt::black);

  // update max level for current pack
  //max_level = gameProfile->levelPackCount(gameProfile->currentLevelPack());

  // very first initialization
  initGame();
}

GameScene::~GameScene()
{
  delete toolset;
  delete gameBonus;
  delete gameBackground;
  delete priv;
}

void GameScene::initGame()
{
  myLock = true;

  setDefaultGameCursor();

  rows = cols = 0;

  //initTheme();
  initProfile();

  gameBonus->setupItems(MAX_ITEMS_COUNT);

  myLock = false;
}

void GameScene::initTheme()
{
  loadRandomBackground();
}

bool GameScene::initLevel(int level)
{
  disableTimers();

  qsrand(QTime::currentTime().msec());

  // if level pack not found
  if (max_level == 0)
      return false;

  // init field
  const int max = MAX_COLS*MAX_ROWS;
  for (int i = 0; i < max; i++)
  {
    PlaceInfo &pi = field[i];
    if (pi.item)
      delete pi.item;
    field[i] = PlaceInfo();
  }

  // drop items count
  GameItem::dropCount();

  // drop temporary items
  qDeleteAll(tempItems);
  tempItems.clear();

  // init hint controller
  hintControl->init();

  rows = cols = 0;
  frameCount = 1;

  currentItem = 0;
  currentCol = currentRow = -1;
  targetCol = targetRow = -1;
  lastClickPos = QPoint();
  moveState = MS_IDLE;

  availFrom = QPoint();
  availTo = QPoint();
  hintMove = false;
  hintText = "";

  inputDisabled = true;
  paintState = false;
  stat_active = false;

  // set level
  this->level = level;

  // check if all levels were finished
  if (level > max_level)
  {
    showStatictics(STAT_GAME_WON);
    return true;
  }

  // setup scenario
  initScenario(level);

  // load level from pack
  if (!loadLevel(level))
      return false;

  // set local info from profile
  LevelPackInfo *lpi = gameProfile->currentPlayer()->currentLevelPackInfo();
  score = lpi->score;

  // more time for hard difficulty
//  if (lpi->diff == DIFF_HARD)
//    time = int(time*1.5);

  toolset->readProfile(lpi);
  gameBonus->readProfile(lpi, this);
  gameBackground->readProfile(gameProfile->currentPlayer());

  //toolset->activateRandomTool();

  // add bonus to the timer
  //time += toolset->bonusClock();

  //stat->level_time = time;

  bonus = 1;
  bonus_time = 0;
  time = 10*60;     // 10 minutes per level

  // init privates
  priv->init();
  priv->longest_chain = lpi->longest_chain;

  priv->timeBar->setMax(time);
  priv->timeBar->setValue(0);
  priv->timeBar->setValue(time);

  toolset->setLongestChain(priv->longest_chain);

  // set background
  loadRandomBackground();

  // pre-draw lower layer
  drawHUDonBackground();

  // set cursor
  setDefaultGameCursor();

  // start level
  gameProfile->setGameStarted(true);
  gameProfile->setGamePaused(false);

  createStaticPopup(QPoint(WIDTH/2, HEIGHT/2),
                  tr("Level %1").arg(level),
                  Qt::AlignCenter,
                  gameStock->Font60,
                  Qt::white,
                  1,
                  0, 50,
                  0, -2);

  sndEngine->playSound(GameSound::sndLevelStart);

  // init animation
  ls_x = ls_y = 0;
  level_start = true;

  advanceTimer->start();

  return true;
}

void GameScene::initScenario(int level)
{
  // item flags - jockers since very beginning
  priv->newItemFlags = ITEM_JOCKER;

  if (level >= 7) { // fallers since level 7
    priv->newItemFlags |= ITEM_FALLER;
  }

  // create targets
  GameTargetControl::TargetsType ttypes = GameTargetControl::TT_RANDOM;
  switch (level)
  {
  case 1:
    hintControl->scheduleHint(hintControl->AimHint);
    hintControl->scheduleHint(hintControl->TargetHint);
    hintControl->scheduleHint(hintControl->TimeHint);
    hintControl->scheduleHint(hintControl->ToolBarHint);
    hintControl->scheduleHint(hintControl->ToolsHint);
    hintControl->scheduleHint(hintControl->PlayHint);
    hintControl->scheduleHint(hintControl->JockerHint);
    // BASE hint
    hintControl->scheduleHint(hintControl->BaseHint);
  case 2:
    ttypes = GameTargetControl::TT_BASE;
    break;
  case 3:
    // NUMBER hint
    hintControl->scheduleHint(hintControl->NumberHint);
  case 4:
    ttypes = GameTargetControl::TT_NUMBER;
    break;
  case 5:
    // COLOR hint
    hintControl->scheduleHint(hintControl->ColorHint);
  case 6:
    ttypes = GameTargetControl::TT_COLOR;
    break;
  case 7:
    // FALLER hint
    hintControl->scheduleHint(hintControl->FallerHint);
  case 8:
    ttypes = GameTargetControl::TT_FALLING;
    break;
  case 9:
    // random levels beginning from 9th
    hintControl->scheduleHint(hintControl->LevelsHint);
    break;
  default:
    break;
  }

  targetControl->createTargets(ttypes);
  /*stat->level_targets_all = */targetControl->targetsCreated();
}

void GameScene::initProfile()
{
    initTheme();

    // read hints info
    hintControl->readProfile(gameProfile->currentPlayer());

    // update max level for current pack
    max_level = gameProfile->levelPackCount(gameProfile->currentLevelPack());
    update();
}

void GameScene::initGraphics()
{
  // scaler should be rescaled already

  xoff = DX(92);
  yoff = DY(10);
  int xoff4 = DX(92+4);
  int yoff4 = DY(10+4);

  for (int i = 0; i < MAX_ROWS; i++)
  {
      cellXPos[i] = i*X56+xoff;
      itemXPos[i] = i*X56+xoff4;
  }

  for (int j = 0; j < MAX_COLS; j++)
  {
      cellYPos[j] = j*Y56+yoff;
      itemYPos[j] = j*Y56+yoff4;
  }

  gameStock->initGraphics();

  initTheme();

  toolset->initGraphics();

  menu->initGraphics();

  gameBonus->initGraphics(this);

  targetControl->initGraphics();

  drawConcretesOnBackground();
  drawHUDonBackground();

  // dialogs
//  if (dconfirm) delete dconfirm;
//  dconfirm = new ConfirmDialog(menu);
//  addWidget(dconfirm, Qt::FramelessWindowHint);
//  dconfirm->hide();
}

void GameScene::selectActiveItems(int items)
{
  if (activeItems.count() == items)
    return;

  activeItems.clear();
  for (int i = 0; i < items; i++)
    activeItems.append(i);
}

////////////////////////////////////////////////////////////////////////////////

bool GameScene::loadLevel(int index)
{
    QString fname = GameWidget::getResourcePath() + "levels/" + gameProfile->currentLevelPack() + ".lpk";
    QFile f(fname);
    if (f.open(QIODevice::ReadOnly)) {
        QDataStream ds(&f);

        // signature
        QString sig;
        ds >> sig;

        if (sig == "LPK2") {
            // author, packname and comment
            QString packname, author, comment;
            ds >> packname >> author >> comment;
        }

        if (sig == "LPK1" || sig == "LPK2") {
            // level count
            int levelcnt;
            ds >> levelcnt;

            if (index > levelcnt) {
                f.close();
                return false;
            }

            // read level
            int pos = ds.device()->pos();

            // offset and size
            ds.device()->seek(pos + (index-1)*4*2);
            int offset, size;
            ds >> offset >> size;

            // read level data
            ds.device()->seek(offset);
            QByteArray ba = ds.device()->read(size);

            f.close();

            // parse scene
            return loadScene(ba);
        }
    }
    return false;
}

bool GameScene::loadScene(const QByteArray &ba)
{
  QDataStream ts(ba);
  QString sig; ts >> sig;

  if (sig != "SCN1") {
      qDebug() << "Error: unknown level format.";
      return false;
  }

  // items count
  int items = 6;
  ts >> items;
  items = 6;

  // bail out if no items
  if (items > gameStock->Items.count())
  {
    qDebug() << "Error: insufficient items loaded.";
    return false;
  }

  // select items
  selectActiveItems(items);

  // time
  ts >> time;
  time *= 60;

  ts >> rows;
  ts >> cols;

  for (int r = 0; r < rows; r++)
  {
    for (int i = 0; i < cols; i++)
    {
      PlaceInfo &pi = DATA(r,i);
      ts >> pi.place;

      pi.place &= ~Blocks;
      pi.place &= ~Targets;

      if (pi.freePlaceForItem())
      {
        pi.item = generateGameItem(priv->newItemFlags);
      }

    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

GameItem* GameScene::generateGameItem(int flags, const QSet<int> &skip)
{
  int id = activeItems.at(qrand() % activeItems.count());
  if (skip.contains(id))
    for (int i = 0; i < activeItems.count(); i++)
      if (!skip.contains(id = activeItems.at(i)))
        break;

  do {
    if ((flags & ITEM_JOCKER) && (GameItem::count(ITEM_JOCKER) < 6) && (qrand() % 20 == 10))
    {
      id = ITEM_JOCKER;
      break;
    }

    // no more than 3 items
    if ((flags & ITEM_FALLER) && (GameItem::count(ITEM_FALLER) < 3))
    {
      // but 0 items also not allowed at a time
      if (GameItem::count(ITEM_FALLER) == 0 || (qrand() % 15 == 5))
      {
        id = ITEM_FALLER;
        break;
      }
    }
  } while (0);

  GameItem *item = new GameItem(id);
  return item;
}

////////////////////////////////////////////////////////////////////////////////

bool GameScene::fillEmptyFreeRows()
{
  bool result = false;

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      PlaceInfo &pi1 = DATA(i,j);

      if (pi1.itemCanBeAppeared())
      {
        pi1.item = generateGameItem(priv->newItemFlags);
        pi1.item->born();
        result = true;
      }
    }
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

struct PlaceAvailInfo
{
  bool hasItem, movable;
  int id;
};

#define PLDATA(r,c) pl[(((r) << 4) + (c)) & 0xff]

bool GameScene::checkAvailableMoves()
{
//  qDebug() << "checkAvailableMoves() entered...";

  bool result = false;
  availFrom = QPoint(-1,-1);
  availTo = QPoint(-1,-1);
  hintMove = false;

  // build temp table
  PlaceAvailInfo *pl = new PlaceAvailInfo[MAX_COLS*MAX_ROWS];

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      PlaceAvailInfo &pi = PLDATA(i,j);

      PlaceInfo &pi1 = DATA(i,j);

      pi.hasItem = pi1.hasItem();
      if (pi.hasItem)
      {
        pi.movable = pi1.itemCanBeMoved();
        pi.id = pi1.item->id();
      } else
      {
        pi.movable = false;
        pi.id = -1;
      }
    }
  }

  // check temp table
  for (int i = rows-1; i >= 0; i--)
  {
    for (int j = 0; j < cols-1; j++)
    {
  //    printf("row %d  col %d \n", i, j);

      PlaceAvailInfo pi1 = PLDATA(i,j);
      if (!pi1.movable)
        continue;

      // simulate item movement to the right
      PlaceAvailInfo pi2 = PLDATA(i,j+1);
      if (pi2.movable)
      {
        PlaceAvailInfo tmp1 = pi1;
        PlaceAvailInfo tmp2 = pi2;
        PLDATA(i,j) = tmp2;
        PLDATA(i,j+1) = tmp1;

        int currentRow = i, currentCol = j;
        int targetRow = i, targetCol = j+1;
        bool b1 = checkAvailItemsInRow((PlaceAvailInfo*)pl, targetRow, qMax(0,targetCol-2), qMin(cols-1,targetCol+2));
        bool b2 = checkAvailItemsInColumn((PlaceAvailInfo*)pl, targetCol, qMax(0,targetRow-2), qMin(rows-1,targetRow+2));
        bool b3 = checkAvailItemsInRow((PlaceAvailInfo*)pl, currentRow, qMax(0,currentCol-2), qMin(cols-1,currentCol+2));
        bool b4 = checkAvailItemsInColumn((PlaceAvailInfo*)pl, currentCol, qMax(0,currentRow-2), qMin(rows-1,currentRow+2));

        PLDATA(i,j) = tmp1;
        PLDATA(i,j+1) = tmp2;

        if (b1 || b2 || b3 || b4)
        {
          availFrom = QPoint(currentCol, currentRow);
          availTo = QPoint(targetCol, targetRow);
          result = true;
          goto final;
        }
      }

      // simulate item movement to the top
      if (i == 0) continue;
      pi2 = PLDATA(i-1,j);
      if (pi2.movable)
      {
        PlaceAvailInfo tmp1 = pi1;
        PlaceAvailInfo tmp2 = pi2;
        PLDATA(i,j) = tmp2;
        PLDATA(i-1,j) = tmp1;

        int currentRow = i, currentCol = j;
        int targetRow = i-1, targetCol = j;
        bool b1 = checkAvailItemsInRow(pl, targetRow, qMax(0,targetCol-2), qMin(cols-1,targetCol+2));
        bool b2 = checkAvailItemsInColumn(pl, targetCol, qMax(0,targetRow-2), qMin(rows-1,targetRow+2));
        bool b3 = checkAvailItemsInRow(pl, currentRow, qMax(0,currentCol-2), qMin(cols-1,currentCol+2));
        bool b4 = checkAvailItemsInColumn(pl, currentCol, qMax(0,currentRow-2), qMin(rows-1,currentRow+2));

        PLDATA(i,j) = tmp1;
        PLDATA(i-1,j) = tmp2;

        if (b1 || b2 || b3 || b4)
        {
          availFrom = QPoint(currentCol, currentRow);
          availTo = QPoint(targetCol, targetRow);
          result = true;
          goto final;
        }
      }

    }
  }

final:
//  qDebug() << " availFrom " << availFrom << "   availTo " << availTo;
//  qDebug() << "---------------- checkAvailableMoves() " << result;

  delete [] pl;

  return result;
}

bool GameScene::checkAvailItemsInRow(PlaceAvailInfo *pl, int row, int from, int to)
{
  if (from+2 > to)
    return false;

  PlaceAvailInfo &pi1 = PLDATA(row, from);
  PlaceAvailInfo &pi2 = PLDATA(row, from+1);
  PlaceAvailInfo &pi3 = PLDATA(row, from+2);
  if (pi1.id == pi2.id && pi2.id == pi3.id && pi3.id != -1)
    return true;

  if (from+3 > to)
    return false;

  PlaceAvailInfo &pi4 = PLDATA(row, from+3);
  if (pi2.id == pi3.id && pi3.id == pi4.id && pi3.id != -1)
    return true;

  if (from+4 > to)
    return false;

  PlaceAvailInfo &pi5 = PLDATA(row, from+4);
  if (pi3.id == pi4.id && pi4.id == pi5.id && pi3.id != -1)
    return true;

  return false;
}

bool GameScene::checkAvailItemsInColumn(PlaceAvailInfo *pl, int col, int from, int to)
{
  if (from+2 > to)
    return false;

  PlaceAvailInfo &pi1 = PLDATA(from, col);
  PlaceAvailInfo &pi2 = PLDATA(from+1, col);
  PlaceAvailInfo &pi3 = PLDATA(from+2, col);
  if (pi1.id == pi2.id && pi2.id == pi3.id && pi3.id != -1)
    return true;

  if (from+3 > to)
    return false;

  PlaceAvailInfo &pi4 = PLDATA(from+3, col);
  if (pi2.id == pi3.id && pi3.id == pi4.id && pi3.id != -1)
    return true;

  if (from+4 > to)
    return false;

  PlaceAvailInfo &pi5 = PLDATA(from+4, col);
  if (pi3.id == pi4.id && pi4.id == pi5.id && pi3.id != -1)
    return true;

  return false;
}

void GameScene::refillItems(int deep)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      PlaceInfo &pi1 = DATA(i,j);

      if (pi1.item && (pi1.place & ItemAppear))
      {
        pi1.item->setId(activeItems.at(qrand() % activeItems.count()));
      }
    }
  }

  for (int d = 0; d < deep; d++)
      for (int i = 0; i < rows; i++)
          for (int j = 0; j < cols; j++) {
              PlaceInfo &pi1 = DATA(i, j);
              if (pi1.item && (pi1.place & ItemAppear)) {
                  int ii = i + d;
                  if (ii >= rows) ii = rows - 1;
                  PlaceInfo &pi2 = DATA(ii, j);
                  if (pi2.item)
                      pi2.item->setId(activeItems.at(qrand() % activeItems.count()));
              }
          }
}

void GameScene::hintAvailableMoves()
{
  hintMove = checkAvailableMoves();
  hintOpacity = 0;
  hintDelta = 0.02;
  hintTimer->stop();
}

////////////////////////////////////////////////////////////////////////////////

bool GameScene::checkRemoveItemsInRow(int row, int from, int to, bool remove)
{
  bool result = false;

  for (int j = to; j >= from && j >= 2; j--)
  {
    PlaceInfo &pi = DATA(row,j);
    if (!pi.hasItem())
      continue;
    if (!(pi.item->isIdle() || pi.item->isNotAlive()))
      continue;

    // only 3 and more
    int id = pi.item->id();
    int cnt = 1;
    while (j--)
    {
      PlaceInfo &pi1 = DATA(row,j);
      if (!pi1.hasItem())
        break;
      if (pi1.item->id() != id || !(pi1.item->isIdle() || pi1.item->isNotAlive()))
        break;
      cnt++;
    }
    j++;

    // found 3 or more items
    if (cnt >= 3)
    {
      // schedule for deletion
      if (remove)
        removeItemsRow(row,j,cnt);
      result = true;
    }
  }

  return result;
}


bool GameScene::checkRemoveItemsInColumn(int col, int from, int to, bool remove)
{
  bool result = false;

  for (int i = to; i >= from && i >= 2; i--)
  {
    PlaceInfo &pi = DATA(i,col);
    if (!pi.hasItem())
      continue;
    if (!(pi.item->isIdle() || pi.item->isNotAlive()))
      continue;

    // only 3 and more
    int id = pi.item->id();
    int cnt = 1;
    while (i--)
    {
      PlaceInfo &pi1 = DATA(i,col);
      if (!pi1.hasItem())
        break;
      if (pi1.item->id() != id || !(pi1.item->isIdle() || pi1.item->isNotAlive()))
        break;
      cnt++;
    }
    i++;

    // found 3 or more items
    if (cnt >= 3)
    {
      // schedule for deletion
      if (remove)
        removeItemsColumn(i,col,cnt);
      result = true;
    }
  }

  return result;
}

// checks and schedules removing contiguous items
// returns true if something was removed, else returns false
bool GameScene::checkRemoveItems()
{
  // we're checking from top-right to bottom-left
  bool result = false;

  // horizontal check
  for (int i = 0; i < rows; i++)
  {
    if (checkRemoveItemsInRow(i, 0, cols-1))
      result = true;
  }

  // vertical check
  for (int i = 0; i < cols; i++)
  {
    if (checkRemoveItemsInColumn(i, 0, rows-1))
      result = true;
  }

  return result;
}

void GameScene::removeItemsRow(int row, int col, int cnt)
{
  for (int j = col; j < col+cnt; j++)
  {
    removeAndCountItem(row,j);
  }
}

void GameScene::removeItemsColumn(int row, int col, int cnt)
{
  for (int i = row; i < row+cnt; i++)
  {
    removeAndCountItem(i,col);
  }
}


void GameScene::removeScheduledItems()
{
  bool removed = false;

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      PlaceInfo &pi = DATA(i,j);

      if (!pi.hasItem())
        continue;

      if (pi.item->isNotAlive())
      {
        // create ghost (prepended)
        createPixmapPopup(item_col2x(j), item_row2y(i),
                          (qrand() % 3)-1, 10 + (qrand() % 5)-2,
                          pi.item->itemPixmap(),
                          50,
                          true);

        // delete item
        delete pi.item;
        pi.item = 0;

        removed = true;
      }
    }
  }

  if (removed)
    // play items removed
    sndEngine->playSound(GameSound::sndItemDisappear);
}

void GameScene::advanceTempItems()
{
  // advance temp items
  for (int i = 0; i < tempItems.count(); i++)
  {
    BaseItem *item = tempItems.at(i);

    if (!item->advance())
    {
      delete item;
      tempItems[i] = 0;
    }
  }

  tempItems.removeAll(0);

  // advance particles
  partControl->advance();
}

////////////////////////////////////////////////////////////////////////////////

bool GameScene::checkFallItems()
{
  // we're checking from bottom-right to top-left
  bool result = false;

  // horizontal check - row 0 is skipped for now
  for (int i = rows-1; i > 0; i--)
  {
    for (int j = 0; j < cols; j++)
    {
      PlaceInfo &pi = DATA(i,j);
      if (!pi.freePlaceForItem())
        continue;

      // cant fall into target cell
      if (pi.hasTarget())
        continue;


      // check if something at the top
      PlaceInfo &pi1 = DATA(i-1,j);
      if (pi1.itemCanBeMoved())
      {
        scheduleItemMove(i-1,j, i,j);
        result = true;
        continue;
      }

      // check top-left
      if (j > 0) {
        PlaceInfo &pi1 = DATA(i-1,j-1);
        if (pi1.itemCanBeMoved())
        {
          scheduleItemMove(i-1,j-1, i,j);
          result = true;
          continue;
        }
      }

      // check top-right
      if (j < cols-1) {
        PlaceInfo &pi1 = DATA(i-1,j+1);
        if (pi1.itemCanBeMoved())
        {
          if (DATA(i,j+1).freePlaceForItem() == false)
          {
            scheduleItemMove(i-1,j+1, i,j);
            result = true;
            continue;
          }
        }
      }

    }
  }

  return result;
}

void GameScene::scheduleItemMove(int r1,int c1, int r2,int c2)
{
  // for now, just simple move this item
  PlaceInfo &pi1 = DATA(r1,c1);
  PlaceInfo &pi2 = DATA(r2,c2);

  pi2.item = pi1.item;

  pi2.item->fall(c2-c1, r2-r1);

  pi1.item = 0;
}

// checks if falling targets about to be removed
void GameScene::checkFallingTargets()
{
  int i = rows-1;

  for (int j = 0; j < cols; j++)
  {
    PlaceInfo &pi = DATA(i,j);
    if (!pi.item)
      continue;
    if (pi.item->id() != ITEM_FALLER)
      continue;

    // item felt down - count it
    int upscore = FALLER_SCORE;
    score += upscore;

    // update target
    targetControl->checkChain(1, ITEM_FALLER, upscore);

    // update progbar
    toolset->addScore(upscore);

    // and remove
    pi.setProcessed();
    pi.item->scheduleDeath();
  }
}

////////////////////////////////////////////////////////////////////////////////

void GameScene::nextCycle()
{
  //static bool check_moves = true;

  // advance temp items
  advanceTempItems();

  // check hints
  if (!level_start && !stat_active)
  {
    bool hints = hintControl->execute();
    if (hints && timeTimer->isActive())
    {
      timeTimer->stop();
      disableInput();
    }
    else
    if (!hints && !timeTimer->isActive())
    {
      timeTimer->start();
      enableInput();
    }
  }

  // frame loop
  if (!--frameCount)
  {
    frameCount = 5;

    // check if statistic is active
    if (stat_active)
    {
      checkMouseActions();
    }
    else
    // check finished
    if (targetControl->targetsLeft() <= 0)
    {
      levelWon();
      //return;
    }
    else
    // check if tool is in progress
    if (toolset->current() && toolset->current()->inProgress())
    {
      toolset->progressCurrent();
    }
    else
    {
      // order is important!

      checkFallingTargets();

      /*bool is_fill = */fillEmptyFreeRows();

      /*bool is_fall = */checkFallItems();

//      if (moveState == MS_MOVING || moveState == MS_RETURN)
//        checkItemMovement();

      //bool is_remove = false; //checkRemoveItems();
      //removeScheduledItems();


      // check if there are moves available
//      if (is_fill || is_fall || is_remove)
//      {
//        check_moves = true;
//        hintMove = false;
//        hintTimer->stop();
//      }
//      else
//      if (!is_fill && !is_fall && !is_remove)
//      {
//        if (check_moves)
//        {
//          if (!hintTimer->isActive())
//          {
//            hintTimer->setInterval(10000);
//            hintTimer->start();
//          }
//
//          check_moves = false;
//          int refilldeep = 0;
//          while (!checkAvailableMoves())
//          {
//            // generate new items on appear sites
//            refillItems(refilldeep++);
//            if (refilldeep > MAX_ROWS) break;
//          }
//        }
//      }

      // check if current item changed its state
//      if (currentItem)
//        if (!currentItem->isIdle() && !currentItem->isSelected())
//        {
//          currentItem = 0;
//          moveState = MS_IDLE;
//        }

//      if (moveState == MS_IDLE || moveState == MS_SELECTED)
//        checkMouseActions();

    }

  }

  if (moveState == MS_IDLE || moveState == MS_SELECTED)
    checkMouseActions();

  removeScheduledItems();


  // advance and repaint
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      PlaceInfo &pi = DATA(i,j);

      pi.setUnprocessed();

      if (pi.hasItem())
        pi.item->advance();
    }
  }

  update();
}


void GameScene::checkMouseActions()
{
  if (!lastClickPos.isNull())
  {
    // has been clicked
    checkMouseClick();
    lastClickPos = QPoint();
    return;
  }

  if (!lastDragPos.isNull())
  {
    // has been dragged
    checkMouseDrag();
    lastDragPos = QPoint();
    return;
  }

  if (!lastReleasePos.isNull())
  {
    // has been released
    checkMouseRelease();
    lastReleasePos = QPoint();
    return;
  }

  // check for hovers
  if (targetControl->checkMouseHover(cursorPos))
    return;
  if (toolset->checkMouseHover(cursorPos))
    return;
//  if (gameBonus->checkMouseHover(cursorPos))
//    return;

  // check tool activity
  if (toolset->current() && !(toolset->current()->inProgress()))
  {
    int c = int(cursorPos.x() >= xoff ? (cursorPos.x()-xoff) / X56 : -1);
    int r = int(cursorPos.y() >= yoff ? (cursorPos.y()-yoff) / Y56 : -1);

    toolset->checkMouseMoved(r, c);
  }

  removeHint();
}

void GameScene::checkMouseClick()
{
    //qDebug() << "checkMouseClick()";

  // check statistic
  if (stat_active)
  {
    return;
  }

  // item click
  QPoint itemPos = itemCoord(lastClickPos);
  bool item_click = isItemCoord(itemPos);
  if (item_click)
  {
      //qDebug() << "ITEM CLICKED";

    // if tool is active
    if (toolset->current())
    {
      if (toolset->checkItemClick(itemPos.y(), itemPos.x()))
      {
        unsetCurrentItem();
      }
      return;
    }

    // else, handle chain
    PlaceInfo pi = DATA(itemPos.y(), itemPos.x());
    if (pi.item && pi.item->isIdle())
    {
      if (pi.item->id() != ITEM_FALLER)
      {
        priv->chain.append(itemPos);
        priv->chainId = pi.item->id();
        sndEngine->playSound(GameSound::sndUnblock);
      }
    }

    return;
  }

  // tool click
  if (toolset->checkMouseActions(lastClickPos))
  {
    unsetCurrentItem();
    return;
  }

  // check bonus click
  //gameBonus->checkMouseActions(this, lastClickPos);
}

void GameScene::checkMouseRelease()
{
  int cnt = priv->chain.count();
  if (cnt >= 3)
  {
    for (int i = 0; i < cnt; i++)
    {
      const QPoint &pos = priv->chain.at(i);
      removeAndCountItem(pos.y(), pos.x());
    }

    // add score
    int upscore = cnt-2;

    score += upscore;

    //gameBonus->addItemScore(priv->chainId, upscore);

    // add score popup
    createStaticPopup(QPoint(WIDTH/2, HEIGHT/2),
                      tr("%1 items").arg(cnt),
                      Qt::AlignCenter,
                      gameStock->Font20,
                      Qt::white,
                      1,
                      0, 40,
                      0, -1);

    // pass this to the targets
    targetControl->checkChain(cnt, priv->chainId, upscore);

    // check for longest one
    if (cnt > priv->longest_chain)
    {
      priv->longest_chain = cnt;
      toolset->setLongestChain(priv->longest_chain);

      createStaticPopup(QPoint(WIDTH/2, HEIGHT/2+Y48),
                      tr("Longest Chain!"),
                      Qt::AlignCenter,
                      gameStock->Font40,
                      Qt::white,
                      1,
                      30, 80,
                      0, -1);

      sndEngine->playSound(GameSound::sndBonusGot);

      hintControl->scheduleHint(hintControl->LongestChainHint);
    }

    // update progbar
    toolset->addScore(upscore);
  }

  // clear
  priv->init();
  moveState = MS_IDLE;
}

void GameScene::checkMouseDrag()
{
  if (priv->chain.isEmpty())
    return;

  QPoint itemPos = itemCoord(lastDragPos);
  if (!isItemCoord(itemPos))
    return;

  const QPoint &last = priv->chain.last();
  if (itemPos == last)
    return;

  if (priv->chain.count() >= 2)
  {
    if (priv->chain.at(priv->chain.count()-2) == itemPos)
    {
      // remove last entry
      priv->chain.removeLast();

      // setup chain code again
      for (int i = 0; i < priv->chain.count(); i++)
      {
        int id = DATA(priv->chain.at(i).y(), priv->chain.at(i).x()).item->id();
        if (id != ITEM_JOCKER)
        {
          priv->chainId = id;
          return;
        }
        // if not found, then it's back to jocker...
        priv->chainId = ITEM_JOCKER;
      }
      return;
    }

    // else, skip if exists
    if (priv->chain.contains(itemPos))
      return;
  }

  int dx = qAbs(itemPos.x()-last.x());
  int dy = qAbs(itemPos.y()-last.y());
  if (dx > 1 || dy > 1)
    return;

  PlaceInfo &pi = DATA(itemPos.y(), itemPos.x());
  if (pi.item && pi.item->isIdle())
  {
    int id = pi.item->id();

    if (id == ITEM_FALLER)
      return;

    if (priv->chainId == ITEM_JOCKER)
    {
      priv->chain.append(itemPos);
      priv->chainId = id;
      sndEngine->playSound(GameSound::sndUnblock);
      return;
    }

    if (id == priv->chainId || id == ITEM_JOCKER)
    {
      priv->chain.append(itemPos);
      sndEngine->playSound(GameSound::sndUnblock);
      return;
    }
  }
}


void GameScene::unsetCurrentItem()
{
    if (currentItem)
    {
      currentItem->idle();
      currentItem = 0;
      moveState = MS_IDLE;
    }
}

void GameScene::executeMovement()
{
  PlaceInfo &pi1 = DATA(targetRow,targetCol);
  PlaceInfo &pi2 = DATA(currentRow,currentCol);

  pi2.item = pi1.item;
  pi2.item->fall(currentCol-targetCol, currentRow-targetRow);

  pi1.item = currentItem;
  pi1.item->fall(targetCol-currentCol, targetRow-currentRow);
}

bool GameScene::checkItemMovement()
{
  bool res = false;

  if (moveState == MS_MOVING)
  {
    // check if can be moved
    bool b1 = checkRemoveItemsInRow(targetRow, qMax(0,targetCol-2), qMin(cols-1,targetCol+2));
    bool b2 = checkRemoveItemsInColumn(targetCol, qMax(0,targetRow-2), qMin(rows-1,targetRow+2));
    bool b3 = checkRemoveItemsInRow(currentRow, qMax(0,currentCol-2), qMin(cols-1,currentCol+2));
    bool b4 = checkRemoveItemsInColumn(currentCol, qMax(0,currentRow-2), qMin(rows-1,currentRow+2));
    if (b1 || b2 || b3 || b4)
    {
//      // update bonus
//      bonus++;
//      bonus_time = 10 + gameBonus->bonusSpeed();
//      bonusTimer->start();
      res = true;
    }
    else    // else return
    {
      moveState = MS_RETURN;
    }
  }

  if (moveState == MS_RETURN)
  {
    // move back
    PlaceInfo &pi1 = DATA(targetRow,targetCol);
    PlaceInfo &pi2 = DATA(currentRow,currentCol);

    pi1.item = pi2.item;
    pi1.item->fall(targetCol-currentCol, targetRow-currentRow);

    pi2.item = currentItem;
    pi2.item->fall(currentCol-targetCol, currentRow-targetRow);
  }

  // finish
  moveState = MS_IDLE;
  currentItem = 0;
  currentCol = currentRow = -1;
  targetCol = targetRow = -1;

  return res;
}

////////////////////////////////////////////////////////////////////////////////

void GameScene::countTime()
{
  if (gameProfile->currentPlayer()->currentLevelPackInfo()->isPuzzle()) {
      // for puzzle mode level continues when time is off
      if (time > 0) time--;
  }
  else {
    if (time == 60 || time == 30 || time < 6)
      sndEngine->playSound(GameSound::sndBeep);

    if (--time <= 0) {
      levelFailed();
    }
  }
}

void GameScene::countBonusTime()
{
  if (--bonus_time <= 0)
  {
    bonus = 1;
    bonusTimer->stop();
    sndEngine->playSound(GameSound::sndBonus);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GameScene::levelWon()
{
  stopGame();
  sndEngine->playSound(GameSound::sndLevelWon);

  // set profile info from local
  LevelPackInfo *lpi = gameProfile->currentPlayer()->currentLevelPackInfo();

  // update stat
  //stat->level_score = score - lpi->score;
  lpi->score = score;

  lpi->longest_chain = priv->longest_chain;

  gameBonus->writeProfile(lpi);
  toolset->writeProfile(lpi);

  lpi->level++;
  gameProfile->saveProfile();
  menu->activate(false);

  showStatictics((level < max_level) ? STAT_LEVEL_WON : STAT_GAME_WON);
}

void GameScene::levelFailed()
{
  stopGame();
  //stat->level_targets_left = targetControl->targetsLeft();
  sndEngine->playSound(GameSound::sndLevelFail);

  showStatictics(STAT_LEVEL_FAIL);
}

void GameScene::pauseGame()
{
  if (!gameProfile->isGameStarted())
    return;
  if (gameBonus->isActive())
    return;

  menu->setMenuPage(MENU_PAUSE);
  menu->activate();

  restoreCursor();
  gameProfile->setGamePaused(true);

  disableInput();
  disableTimers();
}

void GameScene::continueGame()
{
  QApplication::setOverrideCursor(lastCursor);
  gameProfile->setGamePaused(false);

  enableInput();
  enableTimers();
}

void GameScene::stopGame()
{
  restoreCursor();
  gameProfile->setGameStarted(false);
  gameProfile->setGamePaused(false);

  disableInput();
  disableTimersButAdvance();
}

void GameScene::exitToMainMenu()
{
  stat_active = false;
  disableTimers();

  gameProfile->setGameStarted(false);
  gameProfile->setGamePaused(false);

  menu->setCurrentIndex(MENU_MAIN);
  menu->activate();
  menu->showNormal();
}

void GameScene::runNextLevel()
{
  stat_active = false;
  gameProfile->setGameStarted(true);
  gameProfile->setGamePaused(false);
  if (!initLevel(gameProfile->currentPlayer()->currentLevelPackInfo()->level))
      exitToMainMenu();
}

void GameScene::disableTimersButAdvance()
{
  timeTimer->stop();
  //advanceTimer->stop(); // this should not be stopped
  bonusTimer->stop();
  hintTimer->stop();

  update();
}

void GameScene::disableTimers()
{
  timeTimer->stop();
  advanceTimer->stop();
  bonusTimer->stop();
  hintTimer->stop();

  update();
}

void GameScene::enableTimers()
{
  timeTimer->start();
  advanceTimer->start();
  if (bonus_time > 0)
    bonusTimer->start();
  if (availFrom.x() != -1)
    hintTimer->start();

  update();
}


void GameScene::wait(int ms)
{
  QTime t;
  t.start();
  while (t.elapsed() < ms)
  {
    QApplication::processEvents();
  }
}

void GameScene::loadRandomBackground()
{
  QStringList ext; ext << "bg*.png" << "bg*.jpg";
  QStringList entryList = QDir(getSchemePath()).entryList(ext);
  if (entryList.count())
  {
    bgpicture = QPixmap(getSchemePath() + "/"
               + entryList.at(qrand() % entryList.count()))
                .scaled(WIDTH, HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //menu->setBackground(bgpicture);
  }
  else
    qDebug() << "Error: no background images loaded.";
}

void GameScene::enterLevel()
{
  level_start = false;

  if (!hintControl->execute())
  {
    timeTimer->start();
    enableInput();
  }
}
