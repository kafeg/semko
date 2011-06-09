#include <QtGui/QApplication>
#include "mainwgt.h"
#include <QTime>
#include <QTranslator>
#include <QLibraryInfo>
#include <QTextCodec>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QTextCodec* codec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForCStrings(codec);
  QTextCodec::setCodecForLocale(codec);
  QTextCodec::setCodecForTr(codec);

  QTranslator qtTranslator;
  qtTranslator.load("qt_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  a.installTranslator(&qtTranslator);

  QTranslator myappTranslator;
  myappTranslator.load(":/tr/semko_" + QLocale::system().name());
  a.installTranslator(&myappTranslator);

  qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

  QApplication::setApplicationName("Semko");
  QApplication::setApplicationVersion("0.1");
  QApplication::setOrganizationName("Kafeg");
  QApplication::setOrganizationDomain("kafeg.org");

  MainWgt w;
#ifdef Q_OS_SYMBIAN
  w.showFullScreen();
#else
  w.show();
#endif

  return a.exec();
}
