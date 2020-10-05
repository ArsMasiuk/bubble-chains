TEMPLATE = app

QT += widgets gui xml opengl multimedia

CONFIG += resources_big

VERSION = 0.2.0.0
QMAKE_TARGET_COPYRIGHT = (c) 2010-2020 A.Masiuk, A.Korchenko
QMAKE_TARGET_PRODUCT = Bubble Chains
QMAKE_TARGET_DESCRIPTION = Free crossplatform 2D arcade-puzzle game
QMAKE_TARGET_COMPANY =


unix: {
    TARGET = ./bin/chains
    target.path = /usr/local/bin
    INSTALLS += target

    datas.path = /usr/share/games/chains
    datas.files = data
    INSTALLS += datas
	
    desktop.path = /usr/share/applications/
    desktop.files = setup.linux/chains.desktop
    INSTALLS += desktop
	
    icon.path = /usr/share/icons/hicolor/256x256/apps/
    icon.files = setup.linux/chains.png
    INSTALLS += icon

    icon2.path = /usr/share/pixmaps
    icon2.files = setup.linux/chains.png
    INSTALLS += icon2

    appdata.path = /usr/share/appdata/
    appdata.files = setup.linux/chains.appdata.xml
    INSTALLS += appdata
	
    LIBS += -lXrandr -lX11
    QT += x11extras
}

win32: {
    DESTDIR = $$OUT_PWD/bin
    TARGET = chains

    LIBS += -lUser32

    RC_ICONS = icon.ico

    #RC_FILE = res.rc
}

SOURCES += main.cpp \
    gamewidget.cpp \
    gamescene.cpp \
    gameitem.cpp \
    gamepaint.cpp \
    gamecontrol.cpp \
    baseitem.cpp \
    gamemenu.cpp \
    gametools.cpp \
    hammertool.cpp \
    bombtool.cpp \
    gamesound.cpp \
    menucontrol.cpp \
    gameprofile.cpp \
    thundertool.cpp \
    clocktool.cpp \
    gamestock.cpp \
    randomkilltool.cpp \
    mixertool.cpp \
    twintool.cpp \
    displaywrapper.cpp \
    gamebonus.cpp \
    gamestatics.cpp \
    gamestat.cpp \
    gamebackground.cpp \
    consttools.cpp \
    gamexml.cpp \
    scaler.cpp \
    gametargetcontrol.cpp \
    gametargets.cpp \
    progressbar.cpp \
    sceneprivate.cpp \
    gamepanel.cpp \
    gamepanelcontrol.cpp \
    gamedialogs.cpp \
    gamebutton.cpp \
    hintcontrol.cpp \
    gamehintcontrol.cpp \
    particlecontrol.cpp \
    helpform.cpp

HEADERS += gamewidget.h \
    gamescene.h \
    defines.h \
    gameitem.h \
    baseitem.h \
    gamemenu.h \
    gametools.h \
    hammertool.h \
    bombtool.h \
    gamesound.h \
    gameprofile.h \
    thundertool.h \
    clocktool.h \
    gamestock.h \
    randomkilltool.h \
    mixertool.h \
    twintool.h \
    displaywrapper.h \
    gamebonus.h \
    scene_if.h \
    gamebackground.h \
    consttools.h \
    scaler.h \
    sceneprivate.h \
    gametargetcontrol.h \
    gametargets.h \
    progressbar.h \
    gamepanel.h \
    gamepanelcontrol.h \
    gamedialogs.h \
    gamebutton.h \
    hintcontrol.h \
    gamehintcontrol.h \
    particlecontrol.h \
    helpform.h

RESOURCES += resources.qrc

FORMS += menu.ui \
    helpform.ui

TRANSLATIONS += ./data/lang/chains_ru.ts

OTHER_FILES += style/style.css

