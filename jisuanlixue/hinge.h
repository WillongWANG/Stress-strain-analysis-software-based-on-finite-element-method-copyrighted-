#ifndef HINGE_H
#define HINGE_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QToolTip>


class hinge : public QObject,public QGraphicsPathItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    hinge(qreal x, qreal y, int i, QGraphicsItem *parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

signals:
    void arrowClicked();


};

#endif // HINGE_H
