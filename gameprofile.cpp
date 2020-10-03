#include "gameprofile.h"
#include "gamewidget.h"
#include "gamesound.h"

GameProfile* gameProfile = 0;

//--------------------------------
void LevelPackInfo::init()
{
  score = localScore = 0;
  level = 1;

  //time_bonus = speed_bonus = score_bonus = 0;
  //speed_mag_bonus = 2;

  //next_tool = 2;
  activeToolIndex = 0;

  longest_chain = 3;
  toolScore = 0;

  for (int i = 0; i < items.count(); i++)
    items[i] = ItemInfo();
}


//--------------------------------
LevelPackInfo *PlayerInfo::currentLevelPackInfo()
{
    for (int i = 0; i < levelpacks.count(); i++)
        if (levelpacks.at(i)->levelpack == currentlevelpack) {
            return levelpacks.at(i);
        }
    return new LevelPackInfo("");
}

void PlayerInfo::addLevelPack(const QString &levelpack)
{
    bool found = false;
    for (int i = 0; i < levelpacks.count(); i++)
        if (levelpacks.at(i)->levelpack == levelpack) {
            found = true;
            break;
        }
    if (!found)
        levelpacks.append(new LevelPackInfo(levelpack));
}

void PlayerInfo::addLevelPack(LevelPackInfo* lpi)
{
    bool found = false;
    for (int i = 0; i < levelpacks.count(); i++)
        if (levelpacks.at(i)->levelpack == lpi->levelpack) {
            levelpacks.replace(i, lpi);
            found = true;
            break;
        }
    if (!found)
        levelpacks.append(lpi);
}

//--------------------------------
GameProfile::GameProfile(QObject *parent) : QObject(parent),
  m_accel(false)
{
    userPath = QDir::homePath();
#ifdef Q_OS_WIN32
  userPath += "/Application Data/XlabSoft/chains/";
#else
  userPath += "/.xlabsoft/chains/";
#endif

    QDir userdir(userPath);
    if (!userdir.exists())
        userdir.mkpath(userPath);

    m_started = false;
    m_paused = false;

    scanThemes();
}

PlayerInfo *GameProfile::currentPlayer()
{
    if (m_current >= 0 && m_current < m_players.count())
        return m_players.at(m_current);
    else
        return new PlayerInfo("");
}

void GameProfile::setCurrentPlayer(const QString &name)
{
    if (name.isEmpty()) return;

    int index = -1;
    for (int i = 0; i < m_players.count(); i++)
        if (m_players.at(i)->name == name) {
            index = i;
            break;
        }
    if (index == -1) {
        m_players.append(new PlayerInfo(name));
        m_current = m_players.count() - 1;
    }
    else
        m_current = index;

    emit profileChanged();
}

void GameProfile::removePlayer(const QString &name)
{
    int index = -1;
    for (int i = 0; i < m_players.count(); i++)
        if (m_players.at(i)->name == name) {
            index = i;
            break;
        }
    if (index >= 0) {
        m_players.removeAt(index);
        if (m_current >= index)
            m_current--;
        while (m_current >= m_players.count())
            m_current--;
    }
}

void GameProfile::renamePlayer(const QString &before, const QString &after)
{
    int index = -1;
    for (int i = 0; i < m_players.count(); i++)
        if (m_players.at(i)->name == before) {
            index = i;
            break;
        }
    if (index >= 0)
        m_players.at(index)->name = after;
}

void GameProfile::saveProfile()
{
    saveXml();

//  saveTopTen();
}

void GameProfile::loadProfile()
{
    loadXml();

    // if no profile found, create default player
    if (m_players.count() == 0)
        setCurrentPlayer(tr("Player"));

    if (m_current >= 0 && m_current < m_players.count()) {
        sndEngine->setChannelVolume(currentPlayer()->soundVolume);
        sndEngine->setMusicVolume(currentPlayer()->musicVolume);
        sndEngine->enableMusic(currentPlayer()->musicEnabled);
    }

//    loadTopTen();
}

void GameProfile::setPlayerNames(const QStringList &sl)
{
    m_players.clear();
    for (int i = 0; i < sl.count(); i++)
        setCurrentPlayer(sl.at(i));
}

QStringList GameProfile::playerNames()
{
    QStringList sl;
    for (int i = 0; i < m_players.count(); i++) {
        sl.append(m_players.at(i)->name);
    }
    return sl;
}

void GameProfile::scanThemes()
{
    m_themes.clear();
    QString path = GameWidget::getResourcePath() + "schemes/";

    QDir dir(path);
    QFileInfoList lst = dir.entryInfoList(QStringList("*"), QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for (int i = 0; i < lst.count(); i++) {
        QString name = lst.at(i).canonicalFilePath();
        if (!name.isEmpty() && lst.at(i).isDir())
//            m_themes.append(name.section(path, 1, 1));
            m_themes.append(name.mid(name.lastIndexOf('/') + 1));
    }
}

int GameProfile::levelPackCount(const QString &lp)
{
    QString fname = GameWidget::getResourcePath() + "levels/" + lp + ".lpk";
    int levels = 0;
    QFile f(fname);
    if (f.open(QIODevice::ReadOnly)) {
        QDataStream ds(&f);

        // signature
        QString sig;
        ds >> sig;

        if (sig == "LPK2") {
            // packname, author and comment
            QString packname, author, comment;
            ds >> packname >> author >> comment;
        }

        if (sig == "LPK1" || sig == "LPK2") {
            // read levels count
            ds >> levels;
        }
        f.close();
    }
    return levels;
}

QString GameProfile::levelPackName(const QString &lp)
{
    QString fname = GameWidget::getResourcePath() + "levels/" + lp + ".lpk";
    QString packname = lp;
    QFile f(fname);
    if (f.open(QIODevice::ReadOnly)) {
        QDataStream ds(&f);

        // signature
        QString sig;
        ds >> sig;

        if (sig == "LPK2") {
            // packname
            ds >> packname;
        }

        f.close();
    }
    return packname;
}

/*
void GameProfile::addTopTenPlayer(PlayerScore ps)
{
    bool ins = false;
    for (int i = 0; i < m_topten.count(); i++) {
        if (ps.score > m_topten.at(i).score) {
            m_topten.insert(i, ps);
            ins = true;
            break;
        }
    }
    if (!ins)
        m_topten.append(ps);
}

void GameProfile::saveTopTen()
{
    QFile f(userPath + "hall.dat");
    if (f.open(QIODevice::WriteOnly)) {
        QDataStream ds(&f);
        // format signature
        ds << QString("HAL1");

        // count
        int cnt = qMin(m_topten.count(), 10);
        ds << cnt;

        for (int i = 0; i < cnt; i++) {
            ds << m_topten.at(i).name;
            ds << m_topten.at(i).score;
        }
    }
}

void GameProfile::loadTopTen()
{
    QFile f(userPath + "hall.dat");
    if (f.open(QIODevice::ReadOnly)) {
        QDataStream ds(&f);

        QString sig;
        // format signature
        ds >> sig;
        if (sig == "HAL1") {
            int cnt;
            // count
            ds >> cnt;

            for (int i = 0; i < cnt; i++) {
                QString name;
                int score;
                ds >> name;
                ds >> score;
                m_topten.append(PlayerScore(name, score));
            }
        }
    }
}
*/
