#include "semkoitem.h"
#include <math.h>
#include <QGraphicsView>
#include <QDebug>
#include <QTimerEvent>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
static int ySpeed = 3;
static const int ySpeedIncrement = 1, ySpeedMax = 10;

static qreal normalizeAngle(qreal angle)
{
  while (angle < 0)
    angle += TwoPi;
  while (angle > TwoPi)
    angle -= TwoPi;
  return angle;
}

SemkoItem::SemkoItem(QGraphicsScene *scene, int interval, QObject *parent) : QObject(parent),
QGraphicsPixmapItem(), angle(0), m_scene(scene), timerInterval(interval),
state(UnLuzged)
{
  normal = QPixmap(":/img/semko_normal.png");
  pressed = QPixmap(":/img/semko_pressed.png");
  setOffset(-normal.width()/2,-normal.height()/2);
  setPixmap(normal);
  timer = startTimer(timerInterval);
  rotationTimer = startTimer(40);

  int x = fmod(qrand(), scene->views().at(0)->width()-30);
  if (x < 30){
    x += 30;
  }

  setX(x);
#ifdef Q_OS_SYMBIAN
  setY(-350);
#else
  setY(-300);
#endif

  setRotation(qrand() % (360 * 16));
}

void SemkoItem::timerEvent ( QTimerEvent * event )
{
  if (event->timerId() == timer){
      setY(y() + ySpeed);
      if (y() > m_scene->views().at(0)->height()+50){
          if (state == UnLuzged) {
              emit unluzged();
            }
          disconnect();
          deleteLater();
        }
    }

  if (event->timerId() == rotationTimer){

      if (qrand() % 2)
        angle += (qrand() % 2) / 2.0;
      else
        angle -= (qrand() % 2) / 2.0;

      qreal dx = ::sin(angle) * 10;

      setRotation(rotation() + dx);
    }
}

void SemkoItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  setOffset(-pressed.width()/2,-pressed.height()/2);
  setPixmap(pressed);
  state = Luzged;
  emit luzg();
}

void SemkoItem::nextLevel()
{
  killTimer(timer);
  timerInterval -= 5;
  if (timerInterval <= 5){
      timerInterval = 5;
    }
  ySpeed += ySpeedIncrement;
  if(ySpeed >= ySpeedMax){
    ySpeed = ySpeedMax;
  }
  timer = startTimer(timerInterval);
}

void SemkoItem::gameOver(bool isWinner)
{
  deleteLater();
}
