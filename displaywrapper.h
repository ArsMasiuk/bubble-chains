#ifndef DISPLAYWRAPPER_H
#define DISPLAYWRAPPER_H

#include <QtGui>

#ifdef Q_OS_WIN32
#include <Windows.h>
#else
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <QX11Info>
#endif

struct VideoModeInfo
{
  int width, height;
  int bpp;
};

class DisplayWrapper
{
public:
  DisplayWrapper();
  ~DisplayWrapper();

  static void init(bool filter=true, int minWidth=800, int minHeight=600)
  { dwrapper.dw_init(filter,minWidth,minHeight); }

  static bool switchVideoMode(int w, int h, int bpp, bool fs)
  { return dwrapper.dw_switchMode(w,h,bpp,fs); }

  static void restoreVideoMode()
  { dwrapper.dw_restoreMode(); }

  static const QStringList& listModes() { return dwrapper.modeNames; }

  static VideoModeInfo currentVideoMode() { return dwrapper.dw_currentMode(); }

private:
  bool dw_switchMode(int w, int h, int bpp, bool fs);
  void dw_restoreMode();
  void dw_init(bool filter, int minWidth, int minHeight);
  VideoModeInfo dw_currentMode();

  static DisplayWrapper dwrapper;

  int base_width, base_height, base_bpp;

  QStringList modeNames;
  QList<VideoModeInfo> modes;
};

#endif // DISPLAYWRAPPER_H
