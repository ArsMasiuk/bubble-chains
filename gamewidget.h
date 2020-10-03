#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGraphicsView>
#include <QString>
#include <QPainter>


class GameScene;

class GameWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GameWidget(const QString &respath, QWidget *parent = nullptr);
    ~GameWidget();

    static const QString& getResourcePath() { return resourcePath; }

public slots:
    void setVideoMode();
    void playRandomMusic();


protected:
    void closeEvent(QCloseEvent *event);

private:
    void drawBackground ( QPainter * painter, const QRectF & rect );
    void keyPressEvent ( QKeyEvent * keyEvent );
    void focusOutEvent ( QFocusEvent * event );

    GameScene *scene;

    static QString resourcePath;
};

#endif // GAMEWIDGET_H
