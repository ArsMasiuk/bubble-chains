#include "gamewidget.h"

#include "gamescene.h"
#include "gamesound.h"
#include "gameprofile.h"
#include "gamemenu.h"

#include "displaywrapper.h"

#include "scaler.h"

QString GameWidget::resourcePath;

GameWidget::GameWidget(const QString &respath, QWidget *parent)
    : QGraphicsView(parent)
{
  resourcePath = respath;

  qsrand(QTime::currentTime().msec());

  setWindowTitle("Bubble Chains");

  // create sound engine
  sndEngine = new GameSound();
  connect(sndEngine, SIGNAL(musicFinished()), this, SLOT(playRandomMusic()));

  // create and load profile
  gameProfile = new GameProfile(this);
  gameProfile->loadProfile();

  // play music if enabled
  playRandomMusic();


  // use acceleration
  if (gameProfile->isAccelerated())
    setViewport(new QGLWidget());

  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // create and setup scene
  scene = new GameScene(this);
  setScene(scene);

  // setup window
  connect(scene->gameMenu(), SIGNAL(menuVideoModeChanged()),
          this, SLOT(setVideoMode()));

  setVideoMode();
}

GameWidget::~GameWidget()
{
  gameProfile->setWindowPos(x(),y());
  gameProfile->saveProfile();

  delete sndEngine;
  delete gameProfile;

  QApplication::quit();
}

void GameWidget::closeEvent(QCloseEvent *event)
{
    if (!scene->confirmExit())
        event->ignore();
}

void GameWidget::drawBackground ( QPainter * painter, const QRectF & rect )
{
  painter->fillRect(rect, Qt::black);

  scene->drawBackground(painter, rect);
}

void GameWidget::keyPressEvent ( QKeyEvent * keyEvent )
{
  scene->keyPressEvent(keyEvent);
}

void GameWidget::focusOutEvent ( QFocusEvent * event )
{
  QWidget::focusOutEvent(event);

  scene->pauseGame();
}

void GameWidget::setVideoMode()
{
  // setup scaler
  scaler->rescale(gameProfile->screenWidth(), gameProfile->screenHeight());

  // restore video mode (to avoid any issues)
  DisplayWrapper::restoreVideoMode();
  showNormal();
  QApplication::processEvents();

  if (gameProfile->isFullscreen())
  {
    // set full screen mode
    if (!DisplayWrapper::switchVideoMode(WIDTH,HEIGHT,0,true))
    {
        qDebug() << "Unable to change resolution.";
    }

    showFullScreen();
  }
  else
  {
    PlayerInfo *pl = gameProfile->currentPlayer();
    move(pl->x, pl->y);
  }

  // resize window
  setFixedSize(WIDTH,HEIGHT);
  setSceneRect(1, 1, WIDTH-2, HEIGHT-2);

  // update profile mode
  gameProfile->setVideoMode(WIDTH, HEIGHT);

  // invoke graphics update
  scene->initGraphics();
}

void GameWidget::playRandomMusic()
{
  QStringList ext; ext << "*.mp3" << "*.wav" << "*.mod" << "*.mid";
  QString path(resourcePath + "/music");
  QStringList entryList = QDir(path).entryList(ext);
//  qDebug() << entryList;
  if (entryList.count())
  {
    int r = qrand() % entryList.count();
//    qDebug() << path + "/" + entryList.at(r);
    sndEngine->loadMusic(path + "/" + entryList.at(r));
    sndEngine->playMusic();
  }
}
