# -------------------------------------------------
# Project created by QtCreator 2011-04-06T16:44:27
# -------------------------------------------------
QT += core \
    gui \
    phonon
TARGET = semko
TEMPLATE = app
SOURCES += main.cpp \
    mainwgt.cpp \
    semkoitem.cpp \
    SlidingStackedWidget.cpp
HEADERS += mainwgt.h \
    semkoitem.h \
    SlidingStackedWidget.h

TRANSLATIONS = tr/semko_ru.ts

FORMS += mainwgt.ui
RESOURCES += semko.qrc

soundFiles.sources = sounds\*.mp3
soundFiles.path = sounds
DEPLOYMENT += soundFiles

ICON = img/semko.svg
