#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QtGui>

class GamePanelControl;

class GamePanel : public QObject
{
  Q_OBJECT
public:
    GamePanel(GamePanelControl *panelControl, const QRect &rect);
    GamePanel(GamePanel *parentPanel, const QRect &rect);
    virtual ~GamePanel() {}

    void setPosition(QPoint pos);
    inline const QRect& rect() const { return myRect; }

    void setBlocking() { myBlock = true; }
    inline bool isBlocking() const { return myBlock; }

    void setInvisible() { myHidden = true; }
    inline bool isInvisible() const { return myHidden; }

    void setInsensitive() { myInsensitive = true; }
    inline bool isInsensitive() const { return myInsensitive; }

    void setBackBorderPaint(bool set) { myBackBorder = set; }
    void setHoverPaint(bool set) { myPaintHover = set; }

    void setOpacity(double opacity) { myOpacity = opacity; }
    void setHoverOpacity(double opacity) { myHoverOpacity = opacity; }

    void setPen(const QPen &pen) { myPen = pen; }
    void setHoverPen(const QPen &pen) { myHoverPen = pen; }

    void setBackground(QBrush bg) { myBrush = bg; }
    void setHoverBackground(QBrush bg) { myHoverBrush = bg; }

    void setId(int id) { myId = id; }

    void show();
    void close();

    void paint(QPainter &p);

    bool mouseMove(QPoint pos, int buttons);
    bool mouseClick(QPoint pos, int buttons);

    // internal use only
    void setHover(bool set);

protected:
    virtual void init();

    void drawHCentered(QPainter &p, int y, const QString &text);

    virtual void paintContent(QPainter &p);
    virtual bool mouseMoveContent(QPoint pos, int buttons);
    virtual bool mouseClickContent(QPoint pos, int buttons);

    virtual void subcontrolClicked(int id);

    GamePanelControl *myControl;
    QRect myRect;
    GamePanel *myParent;
    bool myBlock, myHidden, myInsensitive;

    double myOpacity, myHoverOpacity;
    QPen myPen, myHoverPen;
    QBrush myBrush, myHoverBrush;

    bool myBackBorder, myPaintHover;

    bool myHover;

    int myId;

    QList<GamePanel*> subPanels;
};

#endif // GAMEPANEL_H
