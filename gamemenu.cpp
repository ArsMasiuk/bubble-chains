#include "gamemenu.h"
#include "gameprofile.h"
#include "gamewidget.h"
#include "gamestock.h"

#include "scaler.h"
#include "displaywrapper.h"

#include "helpform.h"
#include "version.h"

#include <QDebug>

MenuWidget::MenuWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    ui.setupUi(this);
    setWindowOpacity(0.7);

    ui.twHall->setColumnWidth(0, 220);
    ui.twHall->horizontalHeader()->setStretchLastSection(true);
    ui.twHall->setRowCount(10);
    for (int i = 0; i < 10; i++) {
        ui.twHall->setItem(i, 0, new QTableWidgetItem(""));
        ui.twHall->setItem(i, 1, new QTableWidgetItem(""));
    }

    // set version
    ui.labelVersion->setText(tr("Version %1").arg(GAME_VERSION_STR));

    // theme temp hide
    ui.groupBox->hide();

    // load help file
    QSettings settings("xlabsoft","chains");
    QString lang = settings.value("Language", "").toString();
    QString helpfile = GameWidget::getResourcePath() +
                       "help/index";
    if (!lang.isEmpty())
      helpfile += "_" + lang;
    helpfile += ".htm";

    QString helpstring = "";

    //qDebug() << helpfile;

    QFile fhelp(helpfile);
    if (fhelp.open(QIODevice::ReadOnly)) {
        QTextStream ts(&fhelp);
        helpstring = ts.readAll().simplified();
        fhelp.close();
    }

    helpForm = new HelpForm(helpstring, parent);
    helpForm->hide();


    connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));

    setCurrentIndex(MENU_MAIN);
    startAfterSelect = false;
    setMouseTracking(true);

    m_active = false;
    m_newplayer = m_renameplayer = false;

    // load video modes
    ui.lwVideoMode->addItems(DisplayWrapper::listModes());

    // init dynamic stuff
    initGraphics();
}

void MenuWidget::initGraphics()
{
    onCurrentChanged(0);

    int w = DX(200);
    buttonProfile = QRect(x()-w+DX(10),DY(380-30), w,DY(30));
    buttonLevelpack = QRect(x()-w+DX(10),DY(380+60-30), w,DY(30));
}

void MenuWidget::activate(bool en)
{
    m_active = en;
    if (en)
        showNormal();
    else
        hide();
}

/*
void MenuWidget::updateHallOfFame()
{
    QList<PlayerScore> topten = gameProfile->topTenList();

    for (int i = 0; i < 10; i++) {
        QString name = QString("%1. ").arg(i + 1, 2, 10, QChar(' '));
        int score = 0;
        if (i < topten.count()) {
            name += topten.at(i).name;
            score = topten.at(i).score;
        }
        QString s_score = QString("%1").arg(score, 7, 10, QChar('0'));
        if (score == 0)
            s_score = "-------";

        QTableWidgetItem *twi = ui.twHall->item(i, 0);
        if (twi)
            twi->setText(name);

        twi = ui.twHall->item(i, 1);
        if (twi) {
            twi->setText(s_score);
            twi->setTextAlignment(Qt::AlignCenter);
        }
    }
}
*/

void MenuWidget::onCurrentChanged(int)
{
    int dy;

    if (gameProfile->isGameStarted())
      dy = DY(150);
    else
      dy = DY(250);

    if (HEIGHT < 700)
      dy = DY(100);

    move((WIDTH-width())/2, dy);
}

QString MenuWidget::selectedLevelPack()
{
    QString qs;
    int row = ui.lwLevels->currentRow();
    if (row >= 0 && row < lpacks.count())
        qs = lpacks.at(row).section('.',0,0);
    else
        qs = gameProfile->currentLevelPack();
    return qs;
}

void MenuWidget::checkMenuMouseActions(const QPoint &pos)
{
    if (isActive() && !gameProfile->isGameStarted()) {
          if (buttonProfile.contains(pos))
              on_bOptProfile_clicked();
          else if (buttonLevelpack.contains(pos))
              on_bOptLevels_clicked();
      }
}


////////////////////////////////////////////////////////////////////////////////
ConfirmDialog::ConfirmDialog(QWidget *parent, Qt::WindowFlags /*f*/):
    QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    QVBoxLayout *vbl = new QVBoxLayout();

    vbl->addSpacerItem(new QSpacerItem(1, 20));
    text = new QLabel("");
    text->setAlignment(Qt::AlignCenter);
    vbl->addWidget(text);
    vbl->addSpacerItem(new QSpacerItem(1, 20));

    QHBoxLayout *hbl = new QHBoxLayout();

    QPushButton *bNo = new QPushButton(tr("No"));
    bNo->setFixedHeight(48);
    connect(bNo, SIGNAL(clicked()), this, SLOT(onNoClicked()));
    hbl->addWidget(bNo);
    hbl->addSpacerItem(new QSpacerItem(40, 1));

    QPushButton *bYes = new QPushButton(tr("Yes"));
    bYes->setFixedHeight(48);
    connect(bYes, SIGNAL(clicked()), this, SLOT(onYesClicked()));
    hbl->addWidget(bYes);

    vbl->addLayout(hbl);
    setLayout(vbl);

    setWindowOpacity(0.9);
}

int ConfirmDialog::exec(const QString &qs)
{
    setFixedSize(DX(400), DY(200));
    move((WIDTH-DX(400))/2, (HEIGHT-DY(200))/2);
    raise();
    QApplication::processEvents();

    m_result = 0;
    text->setFont(gameStock->Font16);
    text->setText(qs);

    QDialog::exec();

    return m_result;
}

void ConfirmDialog::onNoClicked()
{
    m_result = 0;
    close();
}

void ConfirmDialog::onYesClicked()
{
    m_result = 1;
    close();
}
