TEMPLATE = app

QT += widgets gui xml opengl

CONFIG += resources_big

unix: {
    TARGET = ./bin/chains
    target.path = /usr/bin/
    INSTALLS += target
    datas.path = /usr/share/chains
    datas.files = data
    INSTALLS += datas
    LIBS += -lXrandr -lX11
}

win32: {
    TARGET = ../bin/chains
#    INCLUDEPATH += winlibs/SDL-1.2.13/include/SDL \
#        winlibs/SDL_mixer-1.2.8
#    LIBS += -Lwinlibs/SDL-1.2.13/lib \
#        -Lwinlibs/SDL_mixer-1.2.8/lib \
#        -Lwinlibs/libs \
#        -lSDL.dll
    RC_FILE = res.rc
}

#LIBS += -lSDL -lX11 \
#    -lSDL_mixer

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
