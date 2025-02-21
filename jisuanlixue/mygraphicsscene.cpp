#include "mygraphicsscene.h"
#include <QDebug>


void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) //C++ fabs,for(...:std::vector<>),vector有函数begin,end
{

    if(mainWindow->jjj==1){
        QPointF scenePos = event->scenePos();
        QPointF origin(-25, -40);
        QPointF transformedPos = scenePos - origin;


        for(const auto &v1 : mainWindow->vertices1)
        {
            if(fabs(v1.x*40-transformedPos.x())<5 && fabs(-v1.y*40-transformedPos.y())<5)           //40
                        {
QString myString = "点击坐标(%1,%2)(y向上为正) sigmax,sigmay,tau分别为%3,%4,%5 位移x,y方向分别为%6,%7";
QString formattedString = myString.arg(v1.x).arg(v1.y).arg(mainWindow->ST1[v1.id-1][0]).arg(mainWindow->ST1[v1.id-1][1]).arg(mainWindow->ST1[v1.id-1][2])
.arg(mainWindow->P[2*(v1.id-1)]).arg(mainWindow->P[2*(v1.id-1)+1]);
mainWindow->appendToTextBrowser(formattedString);
break;
                        }
        }
    }

    if(mainWindow->jjj==2){
        QPointF scenePos = event->scenePos();
        QPointF origin(-25, -40);
        QPointF transformedPos = scenePos - origin;


        for(const auto &v1 : mainWindow->verticesgaojie1)
        {
            if(fabs(v1.x*40-transformedPos.x())<5 && fabs(-v1.y*40-transformedPos.y())<5)           //40
                        {
QString myString = "点击坐标(%1,%2)(y向上为正) sigmax,sigmay,tau分别为%3,%4,%5 位移x,y方向分别为%6,%7";
QString formattedString = myString.arg(v1.x).arg(v1.y).arg(mainWindow->ST1[v1.id-1][0]).arg(mainWindow->ST1[v1.id-1][1]).arg(mainWindow->ST1[v1.id-1][2])
.arg(mainWindow->P[2*(v1.id-1)]).arg(mainWindow->P[2*(v1.id-1)+1]);
mainWindow->appendToTextBrowser(formattedString);
break;
                        }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}
