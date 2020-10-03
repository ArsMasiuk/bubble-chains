#include "scaler.h"

#include <QDesktopWidget>

Scaler *scaler = 0;

void Scaler::init()
{
  scaler = new Scaler();
}

Scaler::Scaler()
{
  rescale(1024,768);
}

void Scaler::rescale(int w, int h)
{
  // read actual screen size
  if (w == 0 || h == 0)
  {
    QDesktopWidget dw;
    w = dw.width();
    h = dw.height();
  }

  win_w = w;  cw = (float)win_w/1024.0;
  win_h = h;  ch = (float)win_h/768.0;

  m_x56 = (int)(cw*56);  m_y56 = (int)(ch*56);
  m_x48 = (int)(cw*48);  m_y48 = (int)(ch*48);
  m_x128 = (int)(cw*128);  m_y128 = (int)(ch*128);

  m_x100 = (int)(cw*100);
  m_x900 = (int)(cw*900);
  m_x910 = (int)(cw*910);
  m_y20 = (int)(ch*20);
}
