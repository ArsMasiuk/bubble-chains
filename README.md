# Original Bubble Chains Game

[![Github All Releases](https://img.shields.io/github/downloads/ArsMasiuk/bubble-chains/total?style=for-the-badge)](https://github.com/ArsMasiuk/bubble-chains/releases/latest)
[![GitHub release](https://img.shields.io/github/v/release/ArsMasiuk/bubble-chains?include_prereleases&style=for-the-badge)](https://github.com/ArsMasiuk/bubble-chains/releases/latest)

Bubble Chains is a funny 2D game which runs on Linux, Windows and FreeBSD. 

You have to collect color bubbles on the field by combining them into chains of three or more bubbles. Collected chains disapper, making other bubbles falling down. The longer is a chain, the more score is added to the corresponding target at the bottom. Completed targets are then removed from the field.

The aim of Bubble Chains is to remove all of the targets on each level, and to do this before the time (indicated with the blue-colored bar at the right) runs out. Keep doing this until you have passed the last level and won the game. 

[![Watch gameplay video on youtube](https://img.youtube.com/vi/IW_ADc8k3qw/0.jpg)](https://www.youtube.com/watch?v=IW_ADc8k3qw)

# Changes compared to the original game

This repository contains original source codes and resources of the Bubble Chains game created in the year 2010, including necessary changes to be build with modern C++ compilers. 

Actual version (0.2) has been ported to most recent version of Qt5 framework (Qt 5.15 as for now) so it can be build using mainly latest versions of actual C++ compilers (Visual Studio 2019, GCC 9, clang 11 etc).

Dependency on SDL has been removed, therefore playback of MIDI music files is deprecated. Qt Multimedia Framework is used instead.

Original copyrights have been restored.

# Platforms supported

Since Bubble Chains is based on Qt5 framework, all the platforms supported by Qt should be able to build and launch the game on.

In particular:
- Microsoft Windows
- Linux
- FreeBSD

# Credits & Licenses 

Bubble Chains software is free and is distributed according to GPL-3 and compatible licenses.
Full text of GPL and more you can read here: http://www.fsf.org/licensing/licenses/gpl.html

Originally copyrighted by:	
- 2010-now (c) Arseniy Masiuk & Alexander Korchenko

Icon set used in the game (GPL):
- KDE Crystal Diamond (c) Everaldo Coelho (https://www.iconfinder.com/iconsets/KDE_Crystal)
  
Qt framework (https://www.qt.io) is used under open-source license (GPL).

# Disclaimer

Bubble Chains has been created using only free development software and free resources.
Its direct or indirect (i.e. derivative work) copies cannot be sold or used in any other commercial ways.

Thanks to the authors of the free software, graphic and sound resources used during creation of the game.

If I have occasionally forgotten to mention somebody's work used in the game, please contact me to resolve the issue.
