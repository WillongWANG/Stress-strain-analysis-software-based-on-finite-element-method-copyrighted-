#include "hinge.h"
#include <QGraphicsScene>
#include <QToolTip>
#include <QGraphicsSceneMouseEvent>
#include <QCoreApplication>


hinge::hinge(qreal x, qreal y, int i, QGraphicsItem *parent)
    : QObject(), QGraphicsPathItem(parent)
{
    setAcceptHoverEvents(true);

    QPainterPath path;
    qreal r = 8;

    //滑动
    // 右端面
    if(i==1){
    QPointF center(x+r, y);
    path.addEllipse(center, r, r);
    QPointF center1(x+5*r, y);
    path.addEllipse(center1, r, r);
    path.moveTo(x+2*r, y);
    path.lineTo(x+4*r, y);
    setPath(path);}

    //左端面
    if(i==0){
        QPointF center(x-r, y);
        path.addEllipse(center, r, r);
        QPointF center1(x-5*r, y);
        path.addEllipse(center1, r, r);
        path.moveTo(x-2*r, y);
        path.lineTo(x-4*r, y);
    setPath(path);}

    //上端面
    if(i==2){
        QPointF center(x, y-r);
        path.addEllipse(center, r, r);
        QPointF center1(x, y-5*r);
        path.addEllipse(center1, r, r);
        path.moveTo(x, y-2*r);
        path.lineTo(x, y-4*r);
    setPath(path);}

    //下端面
    if(i==3){
        QPointF center(x, y+r);
        path.addEllipse(center, r, r);
        QPointF center1(x, y+5*r);
        path.addEllipse(center1, r, r);
        path.moveTo(x, y+2*r);
        path.lineTo(x, y+4*r);
    setPath(path);}


    //固定
    // 右端面
    if(i==5){
    QPointF center(x+r, y);
    path.addEllipse(center, r, r);
    path.moveTo(x+2*r, y);
    path.lineTo(x+4*r, y-r);
    path.lineTo(x+4*r, y+r);
    path.lineTo(x+2*r, y);
    setPath(path);}

    //左端面
    if(i==4){
        QPointF center(x-r, y);
        path.addEllipse(center, r, r);
        path.moveTo(x-2*r, y);
        path.lineTo(x-4*r, y-r);
        path.lineTo(x-4*r, y+r);
        path.lineTo(x-2*r, y);
    setPath(path);}

    //上端面
    if(i==6){
        QPointF center(x, y-r);
        path.addEllipse(center, r, r);
        path.moveTo(x, y-2*r);
        path.lineTo(x-r, y-4*r);
        path.lineTo(x+r, y-4*r);
        path.lineTo(x, y-2*r);
    setPath(path);}

    //下端面
    if(i==7){
        QPointF center(x, y+r);
        path.addEllipse(center, r, r);
        path.moveTo(x, y+2*r);
        path.lineTo(x+r, y+4*r);
        path.lineTo(x-r, y+4*r);
        path.lineTo(x, y+2*r);
    setPath(path);}
}

void hinge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
         emit arrowClicked();
    }
}

void hinge::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    QToolTip::showText(event->screenPos(), QCoreApplication::translate("CustomArrowItem", "点击删除"));
}

void hinge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    QToolTip::hideText();
}
