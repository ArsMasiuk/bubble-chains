#ifndef GAMETOOLS_H
#define GAMETOOLS_H

#include <QtGui>

class GameScene;
class ToolSet;
struct PlaceInfo;
struct BonusInfo;
struct PlayerInfo;
struct LevelPackInfo;


class GameTool : public QObject
{
  friend class ToolSet;

public:
  enum ToolAction { ToolOutOfRange, ToolInactive, ToolActive };
  enum ToolType { ManualTool, AutoTool, ConstTool };

  GameTool(int x, int y, int score);
  GameTool(int x, int y, int score, const QString &resource);
  virtual ~GameTool();

  virtual ToolType type() const { return ManualTool; }

  void setBase(ToolSet *set) { myToolset = set; }
  void setCurrent();

  inline const QPixmap& pixmap() const { return myPixmap; }
  inline void setPixmap(const QPixmap &pm) { myPixmap = pm; }

  inline BonusInfo* info() const { return bonusInfo; }

  inline int score() const { return myScore; }
  inline void setScore(int val) { myScore = val; }

  inline const QPoint& pos() const { return myPos; }

  inline bool inProgress() const { return myProgress; }

  inline bool isActive() const { return myActive; }
  inline void setActive(bool act = true) { myActive = act; }

  virtual QString hint() const;

  // internal
  virtual void advanceAndPaint(QPainter &p, int currentScore);

  void animateActivation();

  // defined for manual tools
  virtual bool checkItemClick(int /*row*/, int /*col*/) { return false; }
  virtual ToolAction checkItemState(int /*row*/, int /*col*/) { return ToolOutOfRange; }

  // defined for auto tools
  virtual bool checkToolClick() { return false; }

protected:
    QPixmap myPixmap;
    QPoint myPos;
    int myScore;
    ToolSet *myToolset;
    int myProgress;
    bool myActive;
    BonusInfo *bonusInfo;

    static int stage;
};


class ToolSet
{
public:
    ToolSet();
    ~ToolSet();

    inline GameTool* current() { return currentTool; }
    inline GameTool* tool(int idx) { return tools.at(idx); }

    void activateRandomTool();
    //inline GameTool* active() { return tools.at(myActiveToolIndex); }

//    int bonusClock() const;
//    int bonusScore() const;
//    int bonusTimer() const;
//    int bonusMag() const;

    void readProfile(LevelPackInfo *lpi);
    void writeProfile(LevelPackInfo *lpi);

    void initGraphics();

    void setScore(int score);
    void addScore(int score);

    void setLongestChain(int value);

    //void nextToolActivated();
    //int activeToolIndex(int score);
    //inline int nextToolIndex() const { return next_tool; }

    void updateTools(QPainter &p);

    inline int toolRow() const { return myOverRow; }
    inline int toolCol() const { return myOverCol; }

    bool checkMouseHover(const QPoint &pos);
    bool checkMouseActions(const QPoint &pos);
    bool checkItemClick(int row, int col);
    GameTool::ToolAction checkItemState(int row, int col);
    void checkMouseMoved(int row, int col);
    void progressCurrent();

protected:
    void toolAboutExecute();
    void toolExecuted();
    void updateScore();

    QList<GameTool*> tools;
    int toolScore;
    GameTool *currentTool;
    int myOverRow, myOverCol;

    struct ToolSetPrivate *priv;
};

extern ToolSet * toolset;

#endif // GAMETOOLS_H
