#ifndef GAMEPROFILE_H
#define GAMEPROFILE_H

#include <QtGui>
#include <QXmlStreamReader>

#define DIFF_LOW    1
#define DIFF_NORMAL 2
#define DIFF_HARD   3

#define LANG_DEFAULT "English"

struct ItemInfo
{
  ItemInfo() :
      totalCount(0), localCount(0)
  {
  }

  int totalCount, localCount;
};

struct LevelPackInfo
{
  LevelPackInfo(const QString &s) :
      levelpack(s),
      diff(DIFF_NORMAL), mode(0),
      score(0), localScore(0), level(1),
      toolScore(0),
      //time_bonus(0), speed_bonus(0), score_bonus(0), speed_mag_bonus(2),
      activeToolIndex(0),
      longest_chain(3)
  { }

  void init();

  bool isPuzzle() { return mode == 1; }

  QString levelpack;
  int diff, mode;

  // game progress
  int score, localScore, level, toolScore;
  //int time_bonus, speed_bonus, score_bonus, speed_mag_bonus;
  int activeToolIndex;

  int longest_chain;

  QList<ItemInfo> items;
};

struct PlayerInfo
{
    PlayerInfo(const QString &s) :
        name(s), theme("default"),
        currentlevelpack("default"),
        m_fullscreen(true),
        m_width(0), m_height(0),
        musicEnabled(false), soundVolume(64), musicVolume(64),
        x(0), y(0)
    {
      addLevelPack("default");
    }

    LevelPackInfo* currentLevelPackInfo();

    void addLevelPack(const QString &levelpack);
    void addLevelPack(LevelPackInfo* lpi);
    void setCurrentLevelPack(const QString &levelpack)
    { addLevelPack(levelpack); currentlevelpack = levelpack; }

    QString name, theme;
    QString currentlevelpack; // levelpack

    QStringList inactiveHints;

    // settings
    bool m_fullscreen;
    int m_width, m_height;

    bool musicEnabled;
    int soundVolume, musicVolume;
    int x, y;

    QList<LevelPackInfo*> levelpacks;
};

struct PlayerScore
{
  PlayerScore(const QString &s, int _score) :
      name(s), score(_score)  { }

  QString name;
  int score;
};


class GameProfile : public QObject
{
  Q_OBJECT

public:
    GameProfile(QObject *parent);

    PlayerInfo *currentPlayer();
    void setCurrentPlayer(const QString &name);
    void removePlayer(const QString &name);
    void renamePlayer(const QString &before, const QString &after);

    void saveProfile();
    void loadProfile();

    void saveXml();
    void loadXml();
    void readXmlUnknownElement(QXmlStreamReader &xsr);
    void readXmlPlayer(QXmlStreamReader &xsr);
    void readXmlInactiveHints(QXmlStreamReader &xsr, PlayerInfo *pl);
    void readXmlLevelPack(QXmlStreamReader &xsr, PlayerInfo *pl);
    void readXmlLevelItem(QXmlStreamReader &xsr, LevelPackInfo *lpi);

    void setPlayerNames(const QStringList &sl);
    QStringList playerNames();

    void setGameStarted(bool yes = true) { m_started = yes; }
    bool isGameStarted()                 { return m_started; }

    void setGamePaused(bool yes = true)  { m_paused = yes; }
    bool isGamePaused()                  { return m_paused; }

    void scanThemes();
    QStringList themesList()             { return m_themes; }

    void setCurrentTheme(const QString &_theme)  { currentPlayer()->theme = _theme; }
    QString currentTheme()               { return currentPlayer()->theme; }

    void setCurrentLevelPack(const QString &_lp)  { currentPlayer()->setCurrentLevelPack(_lp); }
    QString currentLevelPack()           { return currentPlayer()->currentlevelpack; }
    int levelPackCount(const QString &lp);
    QString levelPackName(const QString &lp);

    void setFullscreen(bool yes = true)  { currentPlayer()->m_fullscreen = yes; }
    bool isFullscreen()                  { return currentPlayer()->m_fullscreen; }

    void setAccelerated(bool yes = true)    { m_accel = yes; }
    bool isAccelerated()                    { return m_accel; }

    inline int screenWidth()            { return currentPlayer()->m_width; }
    inline int screenHeight()           { return currentPlayer()->m_height; }
    inline void setVideoMode(int w, int h)     { currentPlayer()->m_width = w; currentPlayer()->m_height = h; }

    inline void setWindowPos(int xx, int yy) { currentPlayer()->x = xx; currentPlayer()->y = yy; }

//    void addTopTenPlayer(PlayerScore ps);
//  QList<PlayerScore> topTenList()      { return m_topten; }

signals:
  void profileChanged();

private:
    void saveTopTen();
    void loadTopTen();

    int m_current;
    QList<PlayerInfo*> m_players;
//    QList<PlayerScore> m_topten;
    QStringList m_themes;
    QString userPath;

    bool m_started, m_paused, m_accel;
};

extern GameProfile* gameProfile;

#endif // GAMEPROFILE_H
