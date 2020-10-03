#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QtGui>
#include <QtOpenGL>

#include "defines.h"
#include "gameitem.h"
#include "scene_if.h"

class BaseItem;
class MenuWidget;
class ConfirmDialog;
class GameTool;
class ToolSet;
class GameBonus;


class GameScene : public QGraphicsScene, virtual public IScene
{
  Q_OBJECT

  friend class GameWidget;

public:
    enum MoveState { MS_IDLE, MS_SELECTED, MS_MOVING, MS_RETURN };

    GameScene(QWidget *parent);
    virtual ~GameScene();

    void initGame();
    bool initLevel(int level);
    void initTheme();

    inline const QPoint cursorPosition() const { return cursorPos; }

    QPoint itemCoord(const QPoint &pos)
    {
      int c = pos.x() >= xoff ? (pos.x()-xoff) / X56 : -1;
      int r = pos.y() >= yoff ? (pos.y()-yoff) / Y56 : -1;
      return c >= 0 && r >= 0 ? QPoint(c,r) : QPoint(-1,-1);
    }

    inline bool isItemCoord(const QPoint &pos) { return pos != QPoint(-1,-1); }

    inline MenuWidget* gameMenu() const { return menu; }

    // internals
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent *event );
    void keyPressEvent ( QKeyEvent * keyEvent );

    // timers
    void disableTimersButAdvance();
    void disableTimers();
    void enableTimers();

    void wait(int ms);

    bool confirmExit();

public slots:
    void pauseGame();
    void continueGame();
    void stopGame();

protected slots:
    void nextCycle();

    void executeMovement();
    bool checkItemMovement();

    void hintAvailableMoves();

    void countTime();
    void countBonusTime();
    void bonusTimeLeft();

    void initProfile();
    void initGraphics();

    void on_menuNew();
    void on_menuContinue();
    void on_menuExit();
    void on_menuPauseBack();
    void on_menuRestartLevel();
    void on_menuAbandonGame();
    void on_menuThemeChanged();
    void on_menuGameStart();
    void on_menuLevelPack();


    void runNextLevel();
    void exitToMainMenu();

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &);
    void updateCounters(QPainter &p);
    void drawHint(QPainter &p);
    void drawLevelStartup(QPainter &p);
    void drawHUDonBackground();
    void drawConcretesOnBackground();

    enum STAT_CODE { STAT_LEVEL_WON, STAT_LEVEL_FAIL, STAT_GAME_WON };
    void showStatictics(STAT_CODE code);

    bool loadLevel(int index);
    bool loadScene(const QByteArray &ba);

    void selectActiveItems(int items);
    void initScenario(int level);

    bool fillEmptyFreeRows();
    GameItem* generateGameItem(int flags = 0, const QSet<int> &skip = QSet<int>());

    void refillItems(int deep);
    bool checkAvailableMoves();
    bool checkAvailItemsInRow(struct PlaceAvailInfo *pl, int row, int from, int to);
    bool checkAvailItemsInColumn(struct PlaceAvailInfo *pl, int col, int from, int to);

    bool checkRemoveItems();
    bool checkRemoveItemsInRow(int row, int from, int to, bool remove=true);
    bool checkRemoveItemsInColumn(int col, int from, int to, bool remove=true);
    void removeItemsColumn(int row, int col, int cnt);
    void removeItemsRow(int row, int col, int cnt);
    void removeScheduledItems();

    bool checkFallItems();
    void scheduleItemMove(int row1, int col1, int row2, int col2);

    void checkFallingTargets();

    void unsetCurrentItem();


    void checkMouseActions();
    void checkMouseClick();
    void checkMouseRelease();
    void checkMouseDrag();

    void levelWon();
    void levelFailed();

    void enterLevel();


    void advanceAndWait(int cycles);

    void advanceTempItems();

    void disableInput() { inputDisabled = true; }
    void enableInput() { inputDisabled = false; }

    void loadRandomBackground();

    volatile bool myLock;
    volatile bool paintState;
    volatile bool inputDisabled;

    GameItem *currentItem;
    int currentRow, currentCol;
    int targetRow, targetCol;
    QPoint lastClickPos, lastReleasePos, lastDragPos, cursorPos;
    MoveState moveState;

    QPoint availFrom, availTo;
    bool hintMove;
    qreal hintOpacity, hintDelta;

    QTimer *advanceTimer, *timeTimer, *bonusTimer, *hintTimer;
    int frameCount;

    MenuWidget *menu;
    ConfirmDialog *dconfirm;

    bool stat_active;

    bool level_start;
    int ls_x, ls_y;

    class ScenePrivate *priv;
};

#endif // GAMESCENE_H
