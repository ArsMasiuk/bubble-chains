#ifndef GAMESOUND_H
#define GAMESOUND_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QList>
#include <QSoundEffect>
#include <QSound>


class GameSound : public QObject
{
    Q_OBJECT

public:
    GameSound();
    ~GameSound();

    enum Sounds {
        sndItemDisappear = 0,
        sndSmallHammer = 1,
        sndUnblock = 2,
        sndHammer = 3,
        sndBigHammer = 4,
        sndBomb = 5,
        sndRow = 6,
        sndRandomKill = 7,
        sndMixer = 8,
        sndTwin = 9,
        sndClock = 10,
        sndBonus = 11,
        sndNewItem = 12,
        sndTarget = 13,
        sndLevelStart = 14,
        sndLevelFail = 15,
        sndLevelWon = 16,
        sndBeep = 17,
        sndBonusGot,
        sndTool
    };

    int loadSound(const QString &filename);
    void playSound(int index, int loops = 1);
    void stopSound(int index);
    void stopAllSounds();
    void setChannelVolume(int val, int ch = -1);

    void loadMusic(const QString &filename);
    void playMusic();
    void stopMusic();
    void setMusicVolume(int val);

    inline bool isMusicEnabled() const { return musicEnabled; }
    void enableMusic(bool on = true);

private slots:
    void checkPlayMusic();

signals:
    void musicFinished();

private:
    QList<QSoundEffect*> m_sounds;

    qreal music_vol = 1, channel_vol = 1;

    QSound *m_music;
    QTimer *myTimer;
    bool musicEnabled, musicPlaying;
};

extern GameSound* sndEngine;

#endif // GAMESOUND_H
