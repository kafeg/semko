# -------------------------------------------------
# Project created by QtCreator 2011-04-06T16:44:27
# -------------------------------------------------
QT += core \
    gui \
    multimedia \
    widgets #\
    #phonon
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

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
