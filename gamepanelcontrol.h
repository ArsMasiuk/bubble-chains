#ifndef GAMEPANELCONTROL_H
#define GAMEPANELCONTROL_H

#include <QtGui>

class GamePanel;

class GamePanelControl : public QObject
{
  Q_OBJECT
public:
    GamePanelControl(QObject *parent = 0);

    void addPanel(GamePanel *panel);

    void activatePanel(GamePanel *panel, bool activate);

    void paint(QPainter &p);

    bool mouseMove(QPoint pos, int buttons);
    bool mouseClick(QPoint pos, int buttons);

signals:
    void blocked();
    void unblocked();

protected:
    QList<GamePanel*> panels, activePanels, activeBlockingPanels, activeNonBlockingPanels;
};

#endif // GAMEPANELCONTROL_H
