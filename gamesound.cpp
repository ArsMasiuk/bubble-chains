#include "gamesound.h"
#include "gamewidget.h"

#include <QDebug>
#include <QAudioDeviceInfo>

GameSound* sndEngine = nullptr;

// divider coefficient for volume range (0..128 GUI = 0..1 engine)
const qreal VOLUME_COEFF = 128.0;


GameSound::GameSound()
{
  // load sounds
  QString sndpath = GameWidget::getResourcePath() + "sounds/";

  loadSound(sndpath + "disappear.wav");
  loadSound(sndpath + "smallhammer.wav");
  loadSound(sndpath + "unblock.wav");
  loadSound(sndpath + "hammer.wav");
  loadSound(sndpath + "bighammer.wav");
  loadSound(sndpath + "bomb.wav");
  loadSound(sndpath + "row.wav");
  loadSound(sndpath + "randomkill.wav");
  loadSound(sndpath + "mixer.wav");
  loadSound(sndpath + "twin.wav");
  loadSound(sndpath + "clock.wav");
  loadSound(sndpath + "bonusend.wav");
  loadSound(sndpath + "newitem.wav");
  loadSound(sndpath + "target.wav");
  loadSound(sndpath + "levelstart.wav");
  loadSound(sndpath + "levelfail.wav");
  loadSound(sndpath + "levelwon.wav");
  loadSound(sndpath + "beep.wav");
  loadSound(sndpath + "bonus.wav");
  loadSound(sndpath + "newtool.wav");

  // music
 // music = 0;
  musicEnabled = false;
  musicPlaying = false;

  myTimer = new QTimer(this);
  myTimer->setInterval(1000);
  connect(myTimer, SIGNAL(timeout()), this, SLOT(checkPlayMusic()));
  myTimer->stop();

  // setup volume
//  setChannelVolume(MIX_MAX_VOLUME);
//  setMusicVolume(MIX_MAX_VOLUME/4);
}

GameSound::~GameSound()
{
  stopMusic();
  stopAllSounds();

  qDeleteAll(m_sounds);
  m_sounds.clear();

//  Mix_FreeMusic(music);
}

int GameSound::loadSound(const QString &filename)
{
    QSoundEffect *sound = new QSoundEffect;
    sound->setSource(QUrl::fromLocalFile(filename));

//    while (sound->status() == QSoundEffect::Loading);

//    if (!sound->isLoaded())
//    {
//        delete sound;
//        qDebug() << QUrl::fromLocalFile(filename) << ": sound not loaded";
//        return -1;
//    }

    m_sounds.append(sound);

    return m_sounds.size()-1;
}

void GameSound::playSound(int index, int loops)
{
    if (index >= 0 && index < m_sounds.count())
    {
        QSoundEffect *sound = m_sounds[index];
        sound->setLoopCount(loops);
        sound->setVolume(channel_vol);
        sound->play();
    }
}

void GameSound::stopSound(int index)
{
    if (index >= 0 && index < m_sounds.count())
    {
        m_sounds[index]->stop();
    }
}

void GameSound::stopAllSounds()
{
    for (int i = 0; i <  m_sounds.count(); i++)
        m_sounds.at(i)->stop();
}

void GameSound::setChannelVolume(int val, int /*ch*/)
{
    channel_vol = (qreal)val / VOLUME_COEFF;
}


void GameSound::loadMusic(const QString &filename)
{
//  if (music)
//    Mix_FreeMusic(music);

//  music = Mix_LoadMUS(filename.toLocal8Bit().constData());

//  if(music == NULL) {
//    qDebug() << filename << ": music not loaded: " << Mix_GetError();
//    //fprintf(stderr, "Unable to load music file: %s\n", Mix_GetError());
//  }

}

void GameSound::playMusic()
{
  musicPlaying = true;

  if (!musicEnabled)
    return;

//  int channel = Mix_PlayMusic(music, 0);
//  if(channel == -1)
//  {
//    qDebug() << "Unable to play music file: " << Mix_GetError();
//    //fprintf(stderr, "Unable to play music file: %s\n", Mix_GetError());
//  }

//  Mix_VolumeMusic(music_vol);
  myTimer->start();
}

void GameSound::stopMusic()
{
//  Mix_HaltMusic();
  musicPlaying = false;
  myTimer->stop();
}

void GameSound::enableMusic(bool on)
{
  musicEnabled = on;
  if (musicEnabled)
  {
    if (musicPlaying)
      playMusic();
  }
  else
  {
    //Mix_HaltMusic();
    myTimer->stop();
    // do NOT set musicPlaying to false!
  }
}

void GameSound::checkPlayMusic()
{
  // just rewind
//  if (!Mix_PlayingMusic())
//    //playMusic();
//    emit musicFinished();
}

void GameSound::setMusicVolume(int val)
{
    music_vol = (qreal)val / VOLUME_COEFF;
}
