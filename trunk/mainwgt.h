#ifndef MAINWGT_H
#define MAINWGT_H

#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>
#include <QMap>
#include <QListWidget>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <Phonon/MediaSource>

namespace Ui {
  class MainWgt;
}

class SlidingStackedWidget;

class MainWgt : public QWidget
{
  Q_OBJECT
public:
  explicit MainWgt(QWidget *parent = 0);
  ~MainWgt();

public slots:
  void startGame();

private:
  Ui::MainWgt *ui;
  QTimer *placeSemkoTimer;
  QGraphicsScene *scene;
  QMap<int, QString> results;
  //pages
  QWidget *pMenu, *pGame, *pResults;
  SlidingStackedWidget *stacked;
  void loadResults();
  void saveResults();
  void updateResultsList();
  QListWidget *resultsList;

  bool isSound();
  Phonon::MediaObject *moMainTheme, *moGameOver;
  Phonon::AudioOutput *musicOutput, *notifyOutput;

private slots:
  void placeSemko();
  void luzg();
  void unluzged();
  void slotGameOver(bool isWinner);

  void on_pbQuit_clicked();
  void on_pbResults_clicked();
  void on_pbStartGame_clicked();
  void on_pbBackFromGame_clicked();
  void on_pbBackFromResults_clicked();

signals:
  void nextLevel();
  void gameOver(bool isWinner);
};

#endif // MAINWGT_H
