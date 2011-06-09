#ifndef SEMKOITEM_H
#define SEMKOITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class SemkoItem : public QObject, public QGraphicsPixmapItem
{
Q_OBJECT
public:
    explicit SemkoItem(QGraphicsScene *scene, int interval, QObject *parent = 0);

  enum SemkoState {
  UnLuzged = 0,
    Luzged

  };

signals:
    void luzg();
    void unluzged();

public slots:
    void nextLevel();
    void gameOver(bool isWinner);

private:
    void timerEvent ( QTimerEvent * event );
    QGraphicsScene *m_scene;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    qreal angle;
    QPixmap normal, pressed;
    int timerInterval, timer, rotationTimer, state;

};

#endif // SEMKOITEM_H
