#include "hintcontrol.h"

#include "scaler.h"
#include "gamestock.h"

////////////////////////////////////////////////////////////////////////////////

HintPainter::HintPainter(const HintInfo &hint) : myBlock(hint.blocking)
  {
    myPixmap = QPixmap(DSIZE(hint.size));
    myPixmap.fill(Qt::white);

    myPos = hint.position;
    myOpacity = 0;

    QString style = QString("<head><style type=\"text/css\">\n"
                    ".head {\n"
                    "color: #ffffff;\n"
                    "font-size: %1pt;\n"
                    "font-weight: bold;\n"
                    "}\n"
                    "body {\n"
                    "background: qlineargradient("
                        "x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop: 0 #000000, stop: 1.0 #0079ff);\n"
                    "color: #ffffff;\n"
                    "font-size: %2pt;\n"
                    "border: 1px solid #00ddff;\n"
                    "}\n"
                    "</style></head>\n")
        .arg(gameStock->Font20.pointSize())
        .arg(gameStock->Font12.pointSize());

    QTextBrowser tb;
    tb.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tb.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tb.resize(myPixmap.size());
    tb.setHtml(style + "<body>" + hint.content + "</body>");

    tb.render(&myPixmap);
  }

void HintPainter::paint(QPainter &p)
  {
    int dy = myPos.y();

    if (myOpacity < 0.9)
    {
      myOpacity += 0.05;
//      dy += (0.9-myOpacity) * 30;
    }

    QRect rect(DPOINT(QPoint(myPos.x(),dy)), myPixmap.size());

    p.setOpacity(myOpacity);
    p.drawPixmap(rect, myPixmap);

    p.setPen(QPen(Qt::yellow, 2));
    p.setBrush(Qt::NoBrush);
    p.drawRect(rect);
  }

////////////////////////////////////////////////////////////////////////////////

HintControl::HintControl(QGraphicsScene *parent) : QObject(parent),
  block(false)
{
  scene = parent;
}

void HintControl::scheduleHint(const HintInfo &hint)
{
  // do not schedule inactive hints
  if (!hint.active)
    return;

  HintPainter *hw = new HintPainter(hint);
  hints.append(hw);
}

bool HintControl::execute()
{
  if (hints.isEmpty() && activeHints.isEmpty())
    return false;

  if (block)
    return true;

  for (int i = 0; i < hints.count(); i++)
  {
    HintPainter *hw = hints.takeFirst();
    activeHints.append(hw);
    if (hw->isBlocking())
    {
      block = true;
      return true;
    }
  }

//  hints.clear();

  return true;
}

void HintControl::paint(QPainter &p)
{
  for (int i = 0; i < activeHints.count(); i++)
  {
    HintPainter *hw = activeHints.at(i);
    hw->paint(p);
  }
}

bool HintControl::mouseClick(QPoint /*pos*/, int /*buttons*/)
{
  // by default, remove all the hints
  qDeleteAll(activeHints);
  activeHints.clear();

  block = false;

  return true;
}

void HintControl::init()
{
  qDeleteAll(activeHints);
  activeHints.clear();

  qDeleteAll(hints);
  hints.clear();
}
