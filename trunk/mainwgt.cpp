#include "mainwgt.h"
#include "ui_mainwgt.h"
#include <QLCDNumber>
#include "semkoitem.h"
#include <QDesktopServices>
#include <QDebug>
#include <QInputDialog>
#include <QFile>
#include "SlidingStackedWidget.h"
#include <QDir>

static int nextLevelCount, placeTimerInterval, semkoInterval = 30;
static const int lives = 10, placeTimerIntervalDecrement = 50,placeTimerIntervalMin = 200,
semkoIntervalMin = 10, nextLevelIncrement = 30;

MainWgt::MainWgt(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainWgt)
{
  ui->setupUi(this);

  resultsList = ui->lwResults;

  pMenu = ui->pageMenu;
  pGame = ui->pageGame;
  pResults = ui->pageResults;

  stacked = new SlidingStackedWidget(this);
  stacked->setSpeed(500);
  stacked->addWidget(pMenu);
  stacked->addWidget(pGame);
  stacked->addWidget(pResults);
  stacked->slideInIdx(stacked->indexOf(pMenu));
  layout()->addWidget(stacked);
  ui->stackedWidget->hide();
  ui->stackedWidget->deleteLater();

  scene = new QGraphicsScene();
  ui->gw->setScene(scene);

  ui->gw->setRenderHint(QPainter::Antialiasing);
  ui->gw->setBackgroundBrush(QPixmap(":/img/block_wall.png"));
  ui->gw->setCacheMode(QGraphicsView::CacheBackground);
  ui->gw->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

  placeSemkoTimer = new QTimer();
  connect(placeSemkoTimer,SIGNAL(timeout ()), this, SLOT(placeSemko()));

  loadResults();

  musicOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
  musicOutput->setVolume(0.5);
  notifyOutput = new Phonon::AudioOutput(Phonon::NotificationCategory, this);
  notifyOutput->setVolume(0.7);

  moMainTheme = new Phonon::MediaObject(this);
  connect(moMainTheme,SIGNAL(finished()),moMainTheme,SLOT(play()));
  moMainTheme->setCurrentSource(Phonon::MediaSource(QApplication::applicationDirPath() + "/sounds/mainTheme.mp3"));
  Phonon::createPath(moMainTheme, musicOutput);

  moGameOver  = new Phonon::MediaObject(this);
  moGameOver->setCurrentSource(Phonon::MediaSource(QApplication::applicationDirPath() + "/sounds/gameOver.mp3"));
  Phonon::createPath(moGameOver, notifyOutput);
}

bool MainWgt::isSound()
{
  return ui->cbSound->isChecked();
}

MainWgt::~MainWgt()
{
  delete ui;
}

void MainWgt::loadResults()
{
  QFile resultsFile(QDesktopServices::storageLocation(QDesktopServices::DataLocation)+"/semko.txt");
  if (!resultsFile.open(QIODevice::ReadOnly | QIODevice::Text)){
    return;
  }

  while (!resultsFile.atEnd()) {
    QStringList line = QString(resultsFile.readLine()).split(": ");
    if (line.count() == 2){
      results.insert(line.at(0).toInt(), line.at(1).trimmed());
    }
  }
  resultsFile.close();
  updateResultsList();
}

void MainWgt::saveResults()
{
  QDir dataDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
  if (!dataDir.exists()){
    dataDir.mkpath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
  }
  QFile resultsFile(QDesktopServices::storageLocation(QDesktopServices::DataLocation)+"/semko.txt");
  if (!resultsFile.open(QIODevice::WriteOnly | QIODevice::Text)){
    return;
  }

  QTextStream out(&resultsFile);

  QMapIterator<int, QString> i(results);
  while (i.hasNext()) {
    i.next();
    out << i.key() << ": " << i.value() << "\n";
  }
  out.flush();
  resultsFile.close();
}

void MainWgt::updateResultsList()
{
  resultsList->clear();
  QMapIterator<int, QString> i(results);
  while (i.hasNext()) {
    i.next();
    QString key = "         " + QString::number(i.key());
    resultsList->addItem(key.right(9) + ": " + i.value());
  }
  resultsList->sortItems(Qt::DescendingOrder);

}

void MainWgt::startGame()
{
  if (isSound()){
    moMainTheme->play();
  }
  placeSemkoTimer->stop();
  placeTimerInterval = 1000;
  placeSemkoTimer->setInterval(placeTimerInterval);
  nextLevelCount = nextLevelIncrement;
  semkoInterval = 30;

  scene->setSceneRect(0, 0, 10, 10);
  ui->lcdSluzgano->display(0);
  ui->lcdUnluzged->display ( lives );
  placeSemkoTimer->start();
}

void MainWgt::placeSemko()
{
  SemkoItem *semko = new SemkoItem(scene, semkoInterval);
  connect(semko, SIGNAL(luzg()),this,SLOT(luzg()));
  connect(semko, SIGNAL(unluzged()),this,SLOT(unluzged()));
  connect(this, SIGNAL(nextLevel()),semko,SLOT(nextLevel()));
  connect(this, SIGNAL(gameOver(bool)),semko,SLOT(gameOver(bool)));
  scene->addItem(semko);
}

void MainWgt::luzg()
{
  int c = ui->lcdSluzgano->intValue() + 1;
  if ( c >= nextLevelCount ){
    nextLevelCount += nextLevelIncrement;
    semkoInterval -= 3;
    if (semkoInterval <= semkoIntervalMin){
      semkoInterval = semkoIntervalMin;
    }
    int livesAdd = ui->lcdUnluzged->intValue() + 3;
    ui->lcdUnluzged->display ( livesAdd );
    placeTimerInterval = placeTimerInterval - placeTimerIntervalDecrement;
    if (placeTimerInterval <= placeTimerIntervalMin){
      placeTimerInterval = placeTimerIntervalMin;
    }
    placeSemkoTimer->stop();
    placeSemkoTimer->setInterval(placeTimerInterval);
    placeSemkoTimer->start();
    emit nextLevel();
  }
  ui->lcdSluzgano->display ( c );
}

void MainWgt::unluzged()
{
  int c = ui->lcdUnluzged->intValue() - 1;
  if (c < 0){
    return;
  }
  ui->lcdUnluzged->display ( c );
  if (c <= 0){
    c = 0;
    emit gameOver(false);
    slotGameOver(false);
  }
}

void MainWgt::slotGameOver(bool isWinner)
{
  placeSemkoTimer->stop();
  QApplication::processEvents();

  if (isSound()){
    moMainTheme->stop();
    moGameOver->play();
  }

  QString name = QInputDialog::getText(this,tr("Game over!"), tr("Enter your name"));
  if (name.isEmpty()){
    on_pbBackFromGame_clicked();
    return;
  }

  results.insert(ui->lcdSluzgano->intValue(), name);

  saveResults();
  updateResultsList();
  on_pbResults_clicked();
}

void MainWgt::on_pbQuit_clicked()
{
  qApp->quit();
}

void MainWgt::on_pbResults_clicked()
{
  stacked->slideInIdx(stacked->indexOf(pResults));
}

void MainWgt::on_pbStartGame_clicked()
{
  stacked->slideInIdx(stacked->indexOf(pGame));
  startGame();
}

void MainWgt::on_pbBackFromGame_clicked()
{
  if (isSound()){
    moMainTheme->stop();
  }
  emit gameOver(false);
  placeSemkoTimer->stop();
  stacked->slideInIdx(stacked->indexOf(pMenu));
}

void MainWgt::on_pbBackFromResults_clicked()
{
  stacked->slideInIdx(stacked->indexOf(pMenu));
}
