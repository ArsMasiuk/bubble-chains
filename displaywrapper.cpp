#include "displaywrapper.h"

DisplayWrapper DisplayWrapper::dwrapper;

#ifdef Q_OS_WIN32

DisplayWrapper::DisplayWrapper()
{
}

DisplayWrapper::~DisplayWrapper()
{
}

void DisplayWrapper::dw_init(bool filter, int minWidth, int minHeight)
{
  base_height = base_width = 0;
  base_bpp = 0;

  DEVMODE devmode;
  DWORD modenum = 0;
  VideoModeInfo mode;

  while (EnumDisplaySettings(NULL, modenum, &devmode))
  {
    if (devmode.dmPelsWidth >= minWidth && devmode.dmPelsHeight >= minHeight)
    {
      modeNames.append(QString("%1 x %2")
                       .arg(devmode.dmPelsWidth)
                       .arg(devmode.dmPelsHeight)
                       );

      mode.width = devmode.dmPelsWidth;
      mode.height = devmode.dmPelsHeight;
      mode.bpp = devmode.dmBitsPerPel;
      modes.append(mode);
    }
    modenum++;
  }

  if (filter)
    modeNames.removeDuplicates();
}

bool DisplayWrapper::dw_switchMode(int w, int h, int bpp, bool fs)
{
  DEVMODE originalMode;
  originalMode.dmSize = sizeof(originalMode);

  EnumDisplaySettings(0,
            ENUM_CURRENT_SETTINGS,
            &originalMode);

  DEVMODE newMode = originalMode;

  bool downgrade = false;

  // Changing the settings
  newMode.dmPelsWidth = (uint)w;
  newMode.dmPelsHeight = (uint)h;
  if (bpp > 0)
    newMode.dmBitsPerPel = (uint)bpp;
  else {
    newMode.dmBitsPerPel = 32;
    downgrade = true;
  }

  DWORD flags = 0;
  if (fs) flags |= CDS_FULLSCREEN;
  long res = ChangeDisplaySettings(&newMode, flags);
  if (res == DISP_CHANGE_SUCCESSFUL) return true;

  if (downgrade)
  {
    // 24
    newMode.dmBitsPerPel = 24;
    res = ChangeDisplaySettings(&newMode, flags);
    if (res == DISP_CHANGE_SUCCESSFUL) return true;

    // 16
    newMode.dmBitsPerPel = 16;
    res = ChangeDisplaySettings(&newMode, flags);
    if (res == DISP_CHANGE_SUCCESSFUL) return true;

    // 8
    newMode.dmBitsPerPel = 8;
    res = ChangeDisplaySettings(&newMode, flags);
    if (res == DISP_CHANGE_SUCCESSFUL) return true;
  }

  return false;
}

void DisplayWrapper::dw_restoreMode()
{
  ChangeDisplaySettings(0,0);
}

VideoModeInfo DisplayWrapper::dw_currentMode()
{
  DEVMODE devmode;
  EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
  VideoModeInfo mode;
  mode.width = devmode.dmPelsWidth;
  mode.height = devmode.dmPelsHeight;
  mode.bpp = devmode.dmBitsPerPel;
  return mode;
}

#else

DisplayWrapper::DisplayWrapper()
{
  base_height = base_width = 0;
  base_bpp = 0;
}

DisplayWrapper::~DisplayWrapper()
{
}

void DisplayWrapper::dw_init(bool filter, int minWidth, int minHeight)
{
  VideoModeInfo mode;

  int num_sizes;
  Rotation original_rotation;

  Display *dpy = XOpenDisplay(NULL);
  Window root = RootWindow(dpy, 0);
  XRRScreenSize *xrrs = XRRSizes(dpy, 0, &num_sizes);
  //
  //     GET CURRENT RESOLUTION AND FREQUENCY
  //
  XRRScreenConfiguration *conf = XRRGetScreenInfo(dpy, root);
  short original_rate = XRRConfigCurrentRate(conf);
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);

  XCloseDisplay(dpy);

  for (int i = 0; i < num_sizes; i++)
  {
    int width = xrrs[i].width;
    int height = xrrs[i].height;

    if (width >= minWidth && height >= minHeight)
    {
      modeNames.append(QString("%1 x %2")
                       .arg(width)
                       .arg(height)
                       );

      mode.width = width;
      mode.height = height;
      mode.bpp = 32;  // #### How to know BPP from XRandr ???
      modes.append(mode);
    }
  }

  if (filter)
    modeNames.removeDuplicates();

}

bool DisplayWrapper::dw_switchMode(int w, int h, int bpp, bool fs)
{
  if (!base_height || !base_width)
  {
    XRRScreenConfiguration * info = XRRGetScreenInfo (QX11Info::display(), QX11Info::appRootWindow());
    if (!info)
      return false;

    int numSizes;
    XRRScreenSize * sizes = XRRConfigSizes(info, &numSizes);
    if (!numSizes)
      return false;

    base_width = sizes->width;
    base_height = sizes->height;

    XRRFreeScreenConfigInfo(info);
  }

  QStringList args; args << "--size" << QString("%1x%2").arg(w).arg(h);
  QProcess::execute("xrandr", args);

  return true;
}

void DisplayWrapper::dw_restoreMode()
{
  if (base_height && base_width)
    dw_switchMode(base_width, base_height, base_bpp, true);
}

#endif
