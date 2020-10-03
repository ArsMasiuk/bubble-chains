#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QtGui>
#include "gamescene.h"
#include "ui_menu.h"

const int MENU_MAIN     = 0;
const int MENU_OPTIONS  = 1;
const int MENU_PAUSE    = 2;
const int MENU_GRAPHICS = 3;
const int MENU_AUDIO    = 4;
const int MENU_GAME     = 5;
const int MENU_PROFILE  = 6;
const int MENU_HALL     = 7;
const int MENU_CREDITS  = 8;
const int MENU_LEVELS   = 9;
const int MENU_HELP     = 10;
const int MENU_LANGUAGE = 11;

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    ConfirmDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

    int exec(const QString &qs);

public slots:
    void onNoClicked();
    void onYesClicked();

private:
    QLabel *text;
    int m_result;
};

////////////////////////////////////////////////////////////////////////////////
class MenuWidget : public QStackedWidget
{
    Q_OBJECT

public:
    MenuWidget(QWidget *parent = 0);

    void initGraphics();

    void showHelp();

    void setMenuPage(int index)   { setCurrentIndex(index); setFocus(); }
    void setBackground(QPixmap pix) { bgpicture = pix; }

    bool isActive()                 { return m_active; }
    void activate(bool en = true);

    void checkMenuMouseActions(const QPoint &pos);

    QString selectedLevelPack();

    void keyPressEvent(QKeyEvent *keyEvent);

    QRect buttonProfile, buttonLevelpack;

private:
    Ui::MenuWidget ui;

    QPixmap bgpicture;
    bool startAfterSelect, m_active, m_newplayer, m_renameplayer;
    QStringList lpacks;

    class HelpForm *helpForm;

//    void updateHallOfFame();

private slots:
    void on_bNew_clicked();
    void on_bContinue_clicked();
    void on_bOptions_clicked();
    void on_bHelp_clicked();
    void on_bCredits_clicked();
    void on_bExit_clicked();

    void on_bOptGraphics_clicked();
    void on_bOptSound_clicked();
    void on_bOptProfile_clicked();
    void on_bOptLevels_clicked();
    void on_bOptLanguage_clicked();
    void on_bOptBack_clicked();

    void on_bPauseGraphics_clicked();
    void on_bPauseSound_clicked();
    void on_bPauseHelp_clicked();
    void on_bPauseRestart_clicked();
    void on_bPauseAbandon_clicked();
    void on_bPauseBack_clicked();

    void on_bGraphicsOk_clicked();
    void on_bGraphicsBack_clicked();

    void on_bAudioBack_clicked();
    void on_cbVolumeMusic_valueChanged(int val);
    void on_cbMuteMusic_toggled(bool on);
    void on_cbVolumeSound_valueChanged(int val);
    void on_cbMuteSound_toggled(bool on);

    void on_rbArcade_toggled(bool);
    void on_rbPuzzle_toggled(bool);
    void on_sliDifficulty_valueChanged(int);
    void on_bGameStart_clicked();

//    void on_bProfileCancel_clicked();
//    void on_bProfileOk_clicked();
    void on_bProfileSelect_clicked();
    void on_bProfileRename_clicked();
    void on_bProfileDelete_clicked();
    void on_bProfileBack_clicked();
    void on_lwPlayersList_itemDoubleClicked(QListWidgetItem *item);

//    void on_bHallBack_clicked();
    void on_bHelpBack_clicked();
    void on_bCreditsBack_clicked();

    void on_bLangBack_clicked();
    void on_bLangOk_clicked();
    void on_lwLanguage_itemDoubleClicked(QListWidgetItem *item);

    void on_bLevelsOk_clicked();
    void on_bLevelsBack_clicked();
    void on_lwLevels_itemDoubleClicked(QListWidgetItem *item);

    void onCurrentChanged(int);

signals:
    void menuNew();
    void menuContinue();
    void menuExit();
    void menuPauseBack();
    void menuThemeChanged();
    void menuRestartLevel();
    void menuAbandonGame();
    void menuVideoModeChanged();
    void menuGameStart();
    void menuLevelPack();
};

#endif // GAMEMENU_H
