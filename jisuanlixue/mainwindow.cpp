#include <QDebug>
#include <QMessageBox>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPainterPath>
#include <QPainter>
#include <QGraphicsPathItem>
#include <QFont>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "coordinateitem.h"
#include "customrectitem.h"
#include "customrectitem_1.h"
#include "customellipseitem.h"
#include "customarrowitem.h"
#include "hinge.h"
#include <gmsh.h>
#include <QList>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <tuple>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <set>
#include <map>
#include <Eigen/IterativeLinearSolvers>
#include <QMouseEvent>
#include <QToolTip>
#include "mygraphicsscene.h"


struct Vertex1 {
    int id;
    double x;
    double y;
    double li;
};
QPointF origin(-25, -40);
int MNE,MNN,MNN1,NBW,NBW1,ss=0;double *SE;int bb=0;QList<QGraphicsPathItem*> pathi;
std::map<int, std::set<int>> graph; //C++ map,set;map/set/vector.clear()


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

       this->setWindowTitle("基于有限单元法的简易二维平面应力计算软件");

       scene = new MyGraphicsScene(this);
       ui->graphicsView->setScene(scene);

       // 设置场景矩形以包含负坐标区域
       scene->setSceneRect(-300, -300, 600, 600);

       // 设置原点和轴长度
       qreal axisLength = 30;

       CoordinateItem *coordinateItem = new CoordinateItem(origin, axisLength);

       scene->addItem(coordinateItem);

       QColor backgroundColor = ui->stackedWidget->palette().color(QPalette::Background);
       ui->tabWidget->setStyleSheet(QString("QTabWidget::pane { background-color: %1; } "
                                            "QWidget { background-color: %1; }").arg(backgroundColor.name()));
       ui->tabWidget->show();

       ui->zuoduandian_2->setStyleSheet("QLineEdit { background-color: white; }");
       ui->zuoduandian_2->show();
       ui->youduandian_2->setStyleSheet("QLineEdit { background-color: white; }");
       ui->youduandian_2->show();
       ui->li_2->setStyleSheet("QLineEdit { background-color: white; }");
       ui->li_2->show();
       ui->shangduandian_3->setStyleSheet("QLineEdit { background-color: white; }");
       ui->shangduandian_3->show();
       ui->xiaduandian_3->setStyleSheet("QLineEdit { background-color: white; }");
       ui->xiaduandian_3->show();
       ui->li_6->setStyleSheet("QLineEdit { background-color: white; }");
       ui->li_6->show();

       ui->tabWidget_2->setStyleSheet(QString("QTabWidget::pane { background-color: %1; } "
                                            "QWidget { background-color: %1; }").arg(backgroundColor.name()));
       ui->tabWidget_2->show();

       ui->x_2->setStyleSheet("QLineEdit { background-color: white; }");
       ui->x_2->show();
       ui->y_2->setStyleSheet("QLineEdit { background-color: white; }");
       ui->y_2->show();
       ui->banjin->setStyleSheet("QLineEdit { background-color: white; }");
       ui->banjin->show();
       ui->x_5->setStyleSheet("QLineEdit { background-color: white; }");
       ui->x_5->show();
       ui->y_5->setStyleSheet("QLineEdit { background-color: white; }");
       ui->y_5->show();
       ui->chang_3->setStyleSheet("QLineEdit { background-color: white; }");
       ui->chang_3->show();
       ui->kuan_3->setStyleSheet("QLineEdit { background-color: white; }");
       ui->kuan_3->show();

       ui->progressBar->setValue(0);

       buttonGroup = new QButtonGroup(this);
       buttonGroup->addButton(ui->radioButton, 1); // "1" 是此按钮的标识符，您可以使用任意唯一的整数
       buttonGroup->addButton(ui->radioButton_2, 2);
       buttonGroup->addButton(ui->radioButton_3, 3);

  }


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendToTextBrowser(const QString &text)
{
    ui->textBrowser->append(text);
}

void MainWindow::removeHinges()
{
    for (hinge *hinge : hingeList) {
        scene->removeItem(hinge);
        delete hinge;
    }
    hingeList.clear();
    yueshuList.clear();
    userPoints.erase(std::remove_if(userPoints.begin(), userPoints.end(),[](const Point &point) {return point.a == "yueshu";}),userPoints.end()); //C++ remove_if,lambda函数表达式
    QString myString = "删除铰链约束";                                                                                                              //vector/map.erase
    ui->textBrowser->append(myString);

}

void MainWindow::createHinges(double x,double y,double a,int b)    //a,长或宽;
{

    qreal spacing = 20;
    int numHinges = fabs(x-y)*40/spacing;          /*40!!!*/

    if(b==0){
    for (int i = 0; i <= numHinges; ++i) {
        qreal xPos = origin.x()-a/2*40 ;
        qreal yPos = origin.y()+x*40+ i * spacing;

        hinge *hingeItem = new hinge(xPos, yPos,b);
        connect(hingeItem, &hinge::arrowClicked, this, &MainWindow::removeHinges);
        scene->addItem(hingeItem);
        hingeList.append(hingeItem);
    }}
    if(b==1){
    for (int i = 0; i <= numHinges; ++i) {
        qreal xPos = origin.x()+a/2*40 ;
        qreal yPos = origin.y()+x*40+ i * spacing;

        hinge *hingeItem = new hinge(xPos, yPos,b);
        connect(hingeItem, &hinge::arrowClicked, this, &MainWindow::removeHinges);
        scene->addItem(hingeItem);
        hingeList.append(hingeItem);
    }}
    if(b==2){
    for (int i = 0; i <= numHinges; ++i) {
        qreal xPos = origin.x()+x*40+ i * spacing ;
        qreal yPos = origin.y()-a/2*40;

        hinge *hingeItem = new hinge(xPos, yPos,b);
        connect(hingeItem, &hinge::arrowClicked, this, &MainWindow::removeHinges);
        scene->addItem(hingeItem);
        hingeList.append(hingeItem);
    }}
    if(b==3){
    for (int i = 0; i <= numHinges; ++i) {
        qreal xPos = origin.x()+x*40+ i * spacing ;
        qreal yPos = origin.y()+a/2*40;

        hinge *hingeItem = new hinge(xPos, yPos,b);
        connect(hingeItem, &hinge::arrowClicked, this, &MainWindow::removeHinges);
        scene->addItem(hingeItem);
        hingeList.append(hingeItem);
    }}

    if(b==4){
    for (int i = 0; i <= numHinges; ++i) {
        qreal xPos = origin.x()-a/2*40 ;
        qreal yPos = origin.y()+x*40+ i * spacing;

        hinge *hingeItem = new hinge(xPos, yPos,b);
        connect(hingeItem, &hinge::arrowClicked, this, &MainWindow::removeHinges);
        scene->addItem(hingeItem);
        hingeList.append(hingeItem);
    }}
    if(b==5){
    for (int i = 0; i <= numHinges; ++i) {
        qreal xPos = origin.x()+a/2*40 ;
        qreal yPos = origin.y()+x*40+ i * spacing;

        hinge *hingeItem = new hinge(xPos, yPos,b);
        connect(hingeItem, &hinge::arrowClicked, this, &MainWindow::removeHinges);
        scene->addItem(hingeItem);
        hingeList.append(hingeItem);
    }}
    if(b==6){
    for (int i = 0; i <= numHinges; ++i) {
        qreal xPos = origin.x()+x*40+ i * spacing ;
        qreal yPos = origin.y()-a/2*40;

        hinge *hingeItem = new hinge(xPos, yPos,b);
        connect(hingeItem, &hinge::arrowClicked, this, &MainWindow::removeHinges);
        scene->addItem(hingeItem);
        hingeList.append(hingeItem);
    }}
    if(b==7){
    for (int i = 0; i <= numHinges; ++i) {
        qreal xPos = origin.x()+x*40+ i * spacing ;
        qreal yPos = origin.y()+a/2*40;

        hinge *hingeItem = new hinge(xPos, yPos,b);
        connect(hingeItem, &hinge::arrowClicked, this, &MainWindow::removeHinges);
        scene->addItem(hingeItem);
        hingeList.append(hingeItem);
    }}
}

void MainWindow::removeAllArrows()
{
    for (CustomArrowItem *arrow : arrowsList) {
        scene->removeItem(arrow);
        delete arrow;
    }
    arrowsList.clear();
    shouliList.clear();
    userPoints.erase(std::remove_if(userPoints.begin(), userPoints.end(),[](const Point &point) {return point.a == "li";}),userPoints.end());
    QString myString = "删除力";
    ui->textBrowser->append(myString);
}

void MainWindow::createArrows(double duandian,double duandian1,double li_2,double kuan_2,int b)
{
    // 设置箭头的数量和间距
    qreal arrowSpacing = 20;
    int numArrows = (duandian1-duandian)*40/arrowSpacing;          /*40!!!*/
    int a=(li_2<0);
    qreal length = fabs(li_2);

    if(b==0){
    // 创建箭头
    for (int i = 0; i <= numArrows; ++i) {
        qreal xPos = origin.x()+duandian*40 + i * arrowSpacing;
        qreal yPos = origin.y()-kuan_2*40/2;

        CustomArrowItem *arrowItem = new CustomArrowItem(xPos, yPos, length,a,b);
        connect(arrowItem, &CustomArrowItem::arrowClicked, this, &MainWindow::removeAllArrows);
        scene->addItem(arrowItem);
        arrowsList.append(arrowItem);
    }}
    if(b==1){
    // 创建箭头
    for (int i = 0; i <= numArrows; ++i) {
        qreal xPos = origin.x()+duandian*40 + i * arrowSpacing;
        qreal yPos = origin.y()+kuan_2*40/2;

        CustomArrowItem *arrowItem = new CustomArrowItem(xPos, yPos, length,a,b);
        connect(arrowItem, &CustomArrowItem::arrowClicked, this, &MainWindow::removeAllArrows);
        scene->addItem(arrowItem);
        arrowsList.append(arrowItem);
    }}
    if(b==2){
    // 创建箭头
    for (int i = 0; i <= numArrows; ++i) {
        qreal xPos = origin.x()-chang_2*40/2;
        qreal yPos = origin.y()+duandian*40 + i * arrowSpacing;

        CustomArrowItem *arrowItem = new CustomArrowItem(xPos, yPos, length,a,b);
        connect(arrowItem, &CustomArrowItem::arrowClicked, this, &MainWindow::removeAllArrows);
        scene->addItem(arrowItem);
        arrowsList.append(arrowItem);
    }}
    if(b==3){
    // 创建箭头
    for (int i = 0; i <= numArrows; ++i) {
        qreal xPos = origin.x()+chang_2*40/2;
        qreal yPos = origin.y()+duandian*40 + i * arrowSpacing;

        CustomArrowItem *arrowItem = new CustomArrowItem(xPos, yPos, length,a,b);
        connect(arrowItem, &CustomArrowItem::arrowClicked, this, &MainWindow::removeAllArrows);
        scene->addItem(arrowItem);
        arrowsList.append(arrowItem);
    }}

}


void MainWindow::on_jihe_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_cailiao_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_shouli_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_huadong_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_guding_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

bool compareYAscending(const Point &a, const Point &b)
{
    return a.y < b.y;
}

bool compareYDescending(const Point &a, const Point &b)
{
    return a.y > b.y;
}

bool compareXAscending(const Point &a, const Point &b)
{
    return a.x < b.x;
}

bool compareXDescending(const Point &a, const Point &b)
{
    return a.x > b.x;
}

std::vector<int> BFS(int startNode) { //C++ queue有push,pop,empty,front,back,size;set有insert,find,end,begin,size,clear(可以set1=set2);vector有push_back,size,clear(可以vector1=vector2)
    std::queue<int> q;
    std::set<int> visited;
    std::vector<int> result;

    q.push(startNode);
    visited.insert(startNode);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        result.push_back(node);

        for (int adjacentNode : graph[node]) {
            if (visited.find(adjacentNode) == visited.end()) {
                q.push(adjacentNode);
                visited.insert(adjacentNode);
            }
        }
    }

    return result;
}


void MainWindow::on_shencheng_triggered()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;} //qt的tr不懂
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_pushButton_14_clicked()
{
    for(const auto &y : yuankongList) //C++ for(...:QList<>)
    {// 创建一个红色的点
    int radius=3;
    QGraphicsEllipseItem* point = new QGraphicsEllipseItem(origin.x()+(y.x_2-y.banjin)*40 - radius, origin.y()+(y.y_2)*40 - radius, 2*radius, 2*radius);   //40
    point->setBrush(QBrush(Qt::red));

    QGraphicsEllipseItem* point1 = new QGraphicsEllipseItem(origin.x()+(y.x_2+y.banjin)*40 - radius, origin.y()+(y.y_2)*40 - radius, 2*radius, 2*radius);   //40
    point1->setBrush(QBrush(Qt::red));

    QGraphicsEllipseItem* point2 = new QGraphicsEllipseItem(origin.x()+(y.x_2)*40 - radius, origin.y()+(y.y_2+y.banjin)*40 - radius, 2*radius, 2*radius);   //40
    point2->setBrush(QBrush(Qt::red));

    QGraphicsEllipseItem* point3 = new QGraphicsEllipseItem(origin.x()+(y.x_2)*40 - radius, origin.y()+(y.y_2-y.banjin)*40 - radius, 2*radius, 2*radius);   //40
    point3->setBrush(QBrush(Qt::red));

    // 添加到场景中
    scene->addItem(point);
    scene->addItem(point1);
    scene->addItem(point2);
    scene->addItem(point3);
    }

    bool ok1=false,ok2=false;
    double value = ui->lineEdit->text().toDouble(&ok1);
    double value1 = ui->lineEdit_2->text().toDouble(&ok2);
    if (ok1&&ok2) {
          if(value>chang_2/2||value<-chang_2/2||value1>kuan_2/2||value1<-kuan_2/2||(value>-chang_2/2&&value<chang_2/2&&value1<kuan_2/2&&value1>-kuan_2/2))
          {QMessageBox::warning(this, tr("错误"), tr("非法位置！"));return;}

          lineEdit=value;lineEdit_2=value1;
          userPoints.append({lineEdit,-lineEdit_2,"jian"});

          QString myString = "监控点(%1,%2)";
          QString formattedString = myString.arg(value).arg(value1);
          ui->textBrowser->append(formattedString);

          // 创建一个红色的点
          int radius=3;
          QGraphicsEllipseItem* point = new QGraphicsEllipseItem(origin.x()+value*40 - radius, origin.y()+value1*40 - radius, 2*radius, 2*radius);   //40
          point->setBrush(QBrush(Qt::red));

          // 添加到场景中
          scene->addItem(point);
    }else
    {QMessageBox::warning(this, tr("错误"), tr("输入!"));}

}

int ff=0;

void MainWindow::on_pushButton_15_clicked()
{
    bool ok1=false,ok2=false;
    double value = ui->lineEdit_3->text().toDouble(&ok1);
    double value1 = ui->lineEdit_4->text().toDouble(&ok2);
    if (ok1&&ok2&&value>0&&value1>0)
    {
        lineEdit_3=value;lineEdit_4=value1;
        QString myString = "边、洞大小分别为%1、%2";
        QString formattedString = myString.arg(lineEdit_3).arg(lineEdit_4);
        ui->textBrowser->append(formattedString);

        ff=1;
    }
    else if(ok1==false||ok2==false){QMessageBox::warning(this, tr("错误"), tr("输入!"));}
    else{QMessageBox::warning(this, tr("错误"), tr("输入大于0的数!"));}
}

void MainWindow::on_pushButton_16_clicked()
{
    if(ff!=1) {QMessageBox::warning(this, tr("错误"), tr("定义网格大小！"));return;}

    if(ss==1) {QMessageBox::warning(this, tr("错误"), tr("已生成网格！"));return;}


    gmsh::initialize();
    gmsh::option::setNumber("General.Terminal", 1);
    gmsh::model::add("RectangleWithHoles");        //把GraphicsView中的图形划入gmsh，即原图位置不变(不考虑缩放*40)，y坐标反向（所有在GraphicsView和gmsh相同位置的两点y坐标异号，x坐标同）

    // 添加矩形角点和用户给定的点
    QList<Point> points;
    double boundartLc = lineEdit_3, holeLc = lineEdit_4;   //0.6 0.4

    // 添加矩形边缘，确保包含用户给定的点
    // 对于每条边，先找到落在该边上的用户给定的点，然后按顺序连接点
         QList<Point> zuomian;
         QList<Point> youmian;
         QList<Point> shangmian;
         QList<Point> xiamian;
        for (const auto &userPoint : userPoints) {
            if(userPoint.a=="li")
            {if (userPoint.x==-chang_2/2) {
                zuomian.append({userPoint.x,userPoint.y,"li"});
            }
            if (userPoint.x==chang_2/2) {
                youmian.append({userPoint.x,userPoint.y,"li"});
            }
            if (userPoint.y==-kuan_2/2) {
                xiamian.append({userPoint.x,userPoint.y,"li"});
            }
            if (userPoint.y==kuan_2/2) {
                shangmian.append({userPoint.x,userPoint.y,"li"});
            }}
            if(userPoint.a=="yueshu")
            {if (userPoint.x==-chang_2/2) {
                zuomian.append({userPoint.x,userPoint.y,"yueshu"});
            }
            if (userPoint.x==chang_2/2) {
                youmian.append({userPoint.x,userPoint.y,"yueshu"});
            }
            if (userPoint.y==-kuan_2/2) {
                xiamian.append({userPoint.x,userPoint.y,"yueshu"});
            }
            if (userPoint.y==kuan_2/2) {
                shangmian.append({userPoint.x,userPoint.y,"yueshu"});
            }}
            if(userPoint.a=="jian")
            {if (userPoint.x==-chang_2/2) {
                zuomian.append({userPoint.x,userPoint.y,"jian"});
            }
            if (userPoint.x==chang_2/2) {
                youmian.append({userPoint.x,userPoint.y,"jian"});
            }
            if (userPoint.y==-kuan_2/2) {
                xiamian.append({userPoint.x,userPoint.y,"jian"});
            }
            if (userPoint.y==kuan_2/2) {
                shangmian.append({userPoint.x,userPoint.y,"jian"});
            }}
        }

       std::sort(zuomian.begin(), zuomian.end(), compareYDescending); //C++ std::sort,Qt QList.end()返回的是一个指向QList容器末尾之后一个位置的迭代器并不指向实际的最后一个元素
       std::sort(shangmian.begin(), shangmian.end(), compareXDescending);
       std::sort(youmian.begin(), youmian.end(), compareYAscending);
       std::sort(xiamian.begin(), xiamian.end(), compareXAscending);

       points.append({-chang_2/2,-kuan_2/2,"a"});
       for (const auto &p : xiamian) {points.append(p);}
       points.append({chang_2/2,-kuan_2/2,"a"});
       for (const auto &p : youmian) {points.append(p);}
       points.append({chang_2/2,kuan_2/2,"a"});
       for (const auto &p : shangmian) {points.append(p);}
       points.append({-chang_2/2,kuan_2/2,"a"});
       for (const auto &p : zuomian) {points.append(p);}

       QList<Point> points1;
       points1.append({-chang_2/2,-kuan_2/2,"a"});
       for(const auto &p :points)
       {
           int tag=0;
           for(auto &p1 :points1){if((p.x==p1.x)&&(p.y==p1.y))tag=1;}
           if(tag==0) points1.append(p);
       }

        std::vector<int> loop, curveLoop, planeSurface;

    for (const auto &p :points1) {
       loop.push_back(gmsh::model::geo::addPoint(p.x, p.y, 0.0,boundartLc));
    }

    for(size_t i = 0; i < loop.size(); i++)
      {
        curveLoop.push_back(gmsh::model::geo::addLine(loop[i], i == loop.size()-1 ? loop[0] : loop[i+1]));
      }

    planeSurface.push_back(gmsh::model::geo::addCurveLoop(curveLoop,1));

    loop.clear();

    int t=1;

    QList<juxinkong> juxinkong1;
        for(const auto &p :juxinkongList)
        {
            int tag=0;
            for(auto &p1 :juxinkong1){if((p.x_5==p1.x_5)&&(p.y_5==p1.y_5)&&(p.chang_3==p1.chang_3)&&(p.kuan_3==p1.kuan_3))tag=1;}
            if(tag==0) juxinkong1.append(p);
        }
        juxinkongList = juxinkong1; //Qt QList<>=
        QList<yuankong> yuankong1;
            for(const auto &p :yuankongList)
            {
                int tag=0;
                for(auto &p1 :yuankong1){if((p.x_2==p1.x_2)&&(p.y_2==p1.y_2)&&(p.banjin==p1.banjin))tag=1;}
                if(tag==0) yuankong1.append(p);
            }
            yuankongList = yuankong1;

        // 添加矩形孔
        if (!juxinkongList.isEmpty()) {
        for (const auto &item : juxinkongList) {
            curveLoop.clear();
            double x = item.x_5 - item.chang_3 / 2;
            double y = -item.y_5 - item.kuan_3 / 2;
            double chang = item.chang_3;
            double kuan = item.kuan_3;

            int p1 = gmsh::model::geo::addPoint(x, y, 0,holeLc);
            int p2 = gmsh::model::geo::addPoint(x+chang, y, 0,holeLc);
            int p3 = gmsh::model::geo::addPoint(x + chang, y + kuan, 0,holeLc);
            int p4 = gmsh::model::geo::addPoint(x, y+ kuan, 0,holeLc);

            curveLoop.push_back(gmsh::model::geo::addLine(p1, p2));
            curveLoop.push_back(gmsh::model::geo::addLine(p2, p3));
            curveLoop.push_back(gmsh::model::geo::addLine(p3, p4));
            curveLoop.push_back(gmsh::model::geo::addLine(p4, p1));

            if (!curveLoop.empty()) {
                int curveLoopTag=gmsh::model::geo::addCurveLoop(curveLoop,++t);
                planeSurface.push_back(curveLoopTag);
            }
        }
    }

    // 添加圆孔
   if (!yuankongList.isEmpty()) {
        for (const auto &item : yuankongList) {
            curveLoop.clear();
            double x = item.x_2;
            double y = -item.y_2;
            double r = item.banjin;

            int centerTag = gmsh::model::geo::addPoint(x, y, 0,holeLc);
            int p1 = gmsh::model::geo::addPoint(x + r, y, 0,holeLc);
            int p2 = gmsh::model::geo::addPoint(x - r, y, 0,holeLc);
            int p11 = gmsh::model::geo::addPoint(x, y+r, 0,holeLc);
            int p22 = gmsh::model::geo::addPoint(x, y-r, 0,holeLc);

            curveLoop.push_back(gmsh::model::geo::addCircleArc(p1, centerTag, p11));
            curveLoop.push_back(gmsh::model::geo::addCircleArc(p11, centerTag, p2));
            curveLoop.push_back(gmsh::model::geo::addCircleArc(p2, centerTag, p22));
            curveLoop.push_back(gmsh::model::geo::addCircleArc(p22, centerTag, p1));

            if (!curveLoop.empty()) {
                planeSurface.push_back(gmsh::model::geo::addCurveLoop(curveLoop,++t));
            }
        }
    }


     gmsh::model::geo::addPlaneSurface(planeSurface);

     // 同步模型
     gmsh::model::geo::synchronize();

     // 生成2D网格
     gmsh::model::mesh::generate(2);


    // 获取生成的网格数据
    std::vector<int> elemTypes;
    std::vector<std::vector<std::size_t>> elemTags, nodeTags; //C++ std::size_t,std::abs
    gmsh::model::mesh::getElements(elemTypes, elemTags, nodeTags,-1,-1);

    std::vector<std::size_t> allNodeTags;
    std::vector<double> coord, parametricCoord;
    gmsh::model::mesh::getNodes(allNodeTags, coord, parametricCoord,-1,-1,false,true);

    MNE=nodeTags[1].size()/3-1;

    if(coord.size()/3==allNodeTags.size())  //不是MNN
    std::cout<<" equal ";

    int max=0,gaojieid=nodeTags[1].size();

    // 将所有三角形网格绘制到QGraphicsView中
    for (size_t a = 0; a < nodeTags[1].size(); a += 3) {
        int node1 = nodeTags[1][a]-1;
        int node2 = nodeTags[1][a+1]-1;
        int node3 = nodeTags[1][a+2]-1;

        if((node1+1)>=(nodeTags[1].size()+1)||(node2+1)>=(nodeTags[1].size()+1)||(node3+1)>=(nodeTags[1].size()+1)) qDebug()<<"wrong";

        int diff1 = std::abs(node1 - node2);
        int diff2 = std::abs(node1 - node3);
        int diff3 = std::abs(node2 - node3);

        int maxDiff = std::max({diff1, diff2, diff3}); //C++ std::max/min
        if(maxDiff>max) max=maxDiff;

        // 计算有向面积
            double signed_area = 0.5 * ((coord[node2 * 3] - coord[node1 * 3]) * (coord[node3 * 3 + 1] - coord[node1 * 3 + 1]) -
                                         (coord[node3 * 3] - coord[node1 * 3]) * (coord[node2 * 3 + 1] - coord[node1 * 3 + 1]));

            // 根据有向面积调整顶点顺序，必须逆时针
            if (signed_area < 0) {
                std::swap(node2, node3); //C++ std::swap?vector[]
            }

        QPainterPath path;
        path.moveTo(origin.x()+coord[node1 * 3]*40, origin.y()-coord[node1 * 3 + 1]*40);          //40
        path.lineTo(origin.x()+coord[node2 * 3]*40, origin.y()-coord[node2 * 3 + 1]*40);
        path.lineTo(origin.x()+coord[node3 * 3]*40, origin.y()-coord[node3 * 3 + 1]*40);
        path.lineTo(origin.x()+coord[node1 * 3]*40, origin.y()-coord[node1 * 3 + 1]*40);


        vertices.push_back({node1+1,coord[node1 * 3],coord[node1 * 3 + 1]});  //NE=(int)b(为实际编号-1);i=NNE(NE,1)=vertices[b(NE)*3].id;j=NNE(NE,2)=vertices[b*3+1].id;k=NNE(NE,3)=vertices[b*3+2].id
        vertices.push_back({node2+1,coord[node2 * 3],coord[node2 * 3 + 1]});  //CN(i,1)=vertices[b*3].x;CN(i,2)=vertices[b*3].y;CN(j,1)=vertices[b*3+1].x;CN(j,2)=vertices[b*3+1].y
        vertices.push_back({node3+1,coord[node3 * 3],coord[node3 * 3 + 1]});  //CN(k,1)=vertices[b*3+2].x;CN(k,2)=vertices[b*3+2].y
                                                                              //此处及以上坐标均为gmsh坐标系下,计算力也以此为坐标

        verticesgaojie.push_back({node1+1,coord[node1 * 3],coord[node1 * 3 + 1]});
        std::vector<Vertex> verticesgaotemp=verticesgaojie;bool f=false;int vv;
        for(const auto &v :verticesgaotemp)
        {if(((coord[node1 * 3]+coord[node2 * 3])/2.)==v.x&&((coord[node1 * 3 + 1]+coord[node2 * 3 + 1])/2.)==v.y) {f=true;vv=v.id;break;}}
         if(!f){gaojieid++;verticesgaojie.push_back({gaojieid,(coord[node1 * 3]+coord[node2 * 3])/2.,(coord[node1 * 3 + 1]+coord[node2 * 3 + 1])/2.});}
         else{verticesgaojie.push_back({vv,(coord[node1 * 3]+coord[node2 * 3])/2.,(coord[node1 * 3 + 1]+coord[node2 * 3 + 1])/2.});}
        verticesgaojie.push_back({node2+1,coord[node2 * 3],coord[node2 * 3 + 1]});
        verticesgaotemp=verticesgaojie;f=false;
        for(const auto &v :verticesgaotemp)
        {if(((coord[node2 * 3]+coord[node3 * 3])/2.)==v.x&&((coord[node2 * 3 + 1]+coord[node3 * 3 + 1])/2.)==v.y) {f=true;vv=v.id;break;}}
        if(!f){gaojieid++;verticesgaojie.push_back({gaojieid,(coord[node2 * 3]+coord[node3 * 3])/2.,(coord[node2 * 3 + 1]+coord[node3 * 3 + 1])/2.});}
        else{verticesgaojie.push_back({vv,(coord[node2 * 3]+coord[node3 * 3])/2.,(coord[node2 * 3 + 1]+coord[node3 * 3 + 1])/2.});}
        verticesgaojie.push_back({node3+1,coord[node3 * 3],coord[node3 * 3 + 1]});
        verticesgaotemp=verticesgaojie;f=false;
        for(const auto &v :verticesgaotemp)
        {if(((coord[node1 * 3]+coord[node3 * 3])/2.)==v.x&&((coord[node1 * 3 + 1]+coord[node3 * 3 + 1])/2.)==v.y) {f=true;vv=v.id;break;}}
        if(!f){gaojieid++;verticesgaojie.push_back({gaojieid,(coord[node1 * 3]+coord[node3 * 3])/2.,(coord[node1 * 3 + 1]+coord[node3 * 3 + 1])/2.});}
        else{verticesgaojie.push_back({vv,(coord[node1 * 3]+coord[node3 * 3])/2.,(coord[node1 * 3 + 1]+coord[node3 * 3 + 1])/2.});}

        QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
        scene->addItem(pathItem);
    }

    if(2*vertices.size()!=verticesgaojie.size())  qDebug()<<"wrong";
    if((6*MNE+6)!=verticesgaojie.size())  qDebug()<<"wrong";

    NBW=(max+1)*2;
    qDebug()<<" nbw0 "<<NBW;
    int j=0;

    //网格三角形从最左上的顶点开始编号
    for (size_t i = 0; i < vertices.size(); i += 3) {
        if(vertices[i].x==-chang_2/2&&vertices[i].y==kuan_2/2&&j==0)
        {std::swap(vertices[i], vertices[0]);
            std::swap(vertices[i+1], vertices[1]);
            std::swap(vertices[i+2], vertices[2]);
        j=1;}
        if(vertices[i+1].x==-chang_2/2&&vertices[i+1].y==kuan_2/2&&j==0)
        {std::swap(vertices[i+1], vertices[i]);
            std::swap(vertices[i+2], vertices[i+1]);
            std::swap(vertices[i], vertices[0]);
            std::swap(vertices[i+1], vertices[1]);
            std::swap(vertices[i+2], vertices[2]);
        j=1;}
        if(vertices[i+2].x==-chang_2/2&&vertices[i+2].y==kuan_2/2&&j==0)
        {std::swap(vertices[i+2], vertices[i]);
            std::swap(vertices[i+2], vertices[i+1]);
            std::swap(vertices[i], vertices[0]);
            std::swap(vertices[i+1], vertices[1]);
            std::swap(vertices[i+2], vertices[2]);
        j=1;}
    }

    for(const auto &v :vertices)
    {
        int tag=0;
        for(auto &v1 :vertices1){if(v.id==v1.id)tag=1;}
        if(tag==0) vertices1.push_back(v);
    }
    MNN=vertices1.size();qDebug()<<coord.size()/3<<" gg "<<MNN;    //不是MNN

    for (size_t i = 1; i < vertices1.size(); i++) {
        if(vertices1[i-1].id==vertices1[i].id) qDebug()<<"wrong";
    }

    for (size_t i = 0; i < vertices.size(); i += 3) {
        int node1 = vertices[i].id;
        int node2 = vertices[i+1].id;
        int node3 = vertices[i+2].id;

        graph[node1].insert(node2);
        graph[node1].insert(node3);
        graph[node2].insert(node1);
        graph[node2].insert(node3);
        graph[node3].insert(node1);
        graph[node3].insert(node2);
    }
    std::vector<int> order = BFS(vertices[0].id);

    std::sort(vertices1.begin(), vertices1.end(), [&](const Vertex &a, const Vertex &b) { //C++ std::sort std::find
        int orderA = std::find(order.begin(), order.end(), a.id) - order.begin(); //find返回的iterator类型是order.begin()的类型,该类型即vector的迭代器类型(随机访问迭代器)可以做减法
        int orderB = std::find(order.begin(), order.end(), b.id) - order.begin();
        return orderA < orderB;
    });

    int i=1;
    for(auto &v1 :vertices1){v1.id=i;i++;}

    if(i-1==vertices1.size()&&vertices1[0].x==-chang_2/2&&vertices1[0].y==kuan_2/2) qDebug()<<" chongpai ";

    for(auto &v :vertices)
    {
        int i=0;
        for(const auto &v1 :vertices1){if((v.x==v1.x)&&(v.y==v1.y)){v.id=v1.id;i++;}}
        if(i!=1) qDebug()<<" wrong ";
    }

    max=0;
    for (size_t a = 0; a < vertices.size(); a += 3) {
        int node1 = vertices[a].id;
        int node2 = vertices[a+1].id;
        int node3 = vertices[a+2].id;
        int diff1 = std::abs(node1 - node2);
        int diff2 = std::abs(node1 - node3);
        int diff3 = std::abs(node2 - node3);

        int maxDiff = std::max({diff1, diff2, diff3});
        if(maxDiff>max) max=maxDiff;}
    NBW=(max+1)*2;
    qDebug()<<" nbw "<<NBW;


    //高阶单元同理
    j=0;
    for (size_t i = 0; i < verticesgaojie.size(); i += 6) {
        if(verticesgaojie[i].x==-chang_2/2&&verticesgaojie[i].y==kuan_2/2&&j==0)
        {   std::swap(verticesgaojie[i], verticesgaojie[0]);
            std::swap(verticesgaojie[i+1], verticesgaojie[1]);
            std::swap(verticesgaojie[i+2], verticesgaojie[2]);
            std::swap(verticesgaojie[i+3], verticesgaojie[3]);
            std::swap(verticesgaojie[i+4], verticesgaojie[4]);
            std::swap(verticesgaojie[i+5], verticesgaojie[5]);
        j=1;}
        if(verticesgaojie[i+2].x==-chang_2/2&&verticesgaojie[i+2].y==kuan_2/2&&j==0)
        {std::swap(verticesgaojie[i+2], verticesgaojie[i]);
         std::swap(verticesgaojie[i+3], verticesgaojie[i+1]);
         std::swap(verticesgaojie[i+4], verticesgaojie[i+2]);
         std::swap(verticesgaojie[i+5], verticesgaojie[i+3]);
            std::swap(verticesgaojie[i], verticesgaojie[0]);
            std::swap(verticesgaojie[i+1], verticesgaojie[1]);
            std::swap(verticesgaojie[i+2], verticesgaojie[2]);
            std::swap(verticesgaojie[i+3], verticesgaojie[3]);
            std::swap(verticesgaojie[i+4], verticesgaojie[4]);
            std::swap(verticesgaojie[i+5], verticesgaojie[5]);
        j=1;}
        if(verticesgaojie[i+4].x==-chang_2/2&&verticesgaojie[i+4].y==kuan_2/2&&j==0)
        {std::swap(verticesgaojie[i+4], verticesgaojie[i]);
         std::swap(verticesgaojie[i+5], verticesgaojie[i+1]);
         std::swap(verticesgaojie[i+4], verticesgaojie[i+2]);
         std::swap(verticesgaojie[i+3], verticesgaojie[i+5]);
               std::swap(verticesgaojie[i], verticesgaojie[0]);
               std::swap(verticesgaojie[i+1], verticesgaojie[1]);
               std::swap(verticesgaojie[i+2], verticesgaojie[2]);
               std::swap(verticesgaojie[i+3], verticesgaojie[3]);
               std::swap(verticesgaojie[i+4], verticesgaojie[4]);
               std::swap(verticesgaojie[i+5], verticesgaojie[5]);
        j=1;}
    }
    for(const auto &v :verticesgaojie)
    {
        int tag=0;
        for(auto &v1 :verticesgaojie1){if(v.id==v1.id)tag=1;}
        if(tag==0) verticesgaojie1.push_back(v);
    }

    for (size_t i = 1; i < verticesgaojie1.size(); i++) {
        if(verticesgaojie1[i-1].id==verticesgaojie1[i].id) qDebug()<<"wrong";
    }
    MNN1=verticesgaojie1.size();

    graph.clear();
    for (size_t a = 0; a < verticesgaojie.size(); a += 6) {                           //六节点半带宽
        int node[6];
        for(int i=0;i<6;i++)
        {
            node[i]=verticesgaojie[a+i].id;
        }

        for(int i=0;i<6;i++)
        {
            for(int ii=0;ii<6;ii++)
            {if(ii==i) continue;
             graph[node[i]].insert(node[ii]);}
        }
    }
    std::vector<int> order1 = BFS(verticesgaojie[0].id);

    std::sort(verticesgaojie1.begin(), verticesgaojie1.end(), [&](const Vertex &a, const Vertex &b) {
        int orderA = std::find(order1.begin(), order1.end(), a.id) - order1.begin();
        int orderB = std::find(order1.begin(), order1.end(), b.id) - order1.begin();
        return orderA < orderB;
    });

    int i1=1;
    for(auto &v1 :verticesgaojie1){v1.id=i1;i1++;}

    if(i1-1==verticesgaojie1.size()&&verticesgaojie1[0].x==-chang_2/2&&verticesgaojie1[0].y==kuan_2/2) qDebug()<<" chongpai1";

    for(auto &v :verticesgaojie)
    {
        int i=0;
        for(const auto &v1 :verticesgaojie1){if((v.x==v1.x)&&(v.y==v1.y)){v.id=v1.id;i++;}}
        if(i!=1) qDebug()<<" wrong ";
    }

    max=0;
    for (size_t a = 0; a < verticesgaojie.size(); a += 6) {
        int node[6];
        for(int i=0;i<6;i++)
        {
            node[i]=verticesgaojie[a+i].id;
        }

        for(int i=0;i<6;i++)
        {
            for(int ii=0;ii<6;ii++)
            {if(ii==i) continue;
             int diff=std::abs(node[i] - node[ii]);
             if(diff>max) max=diff;
            }
        }
}
    NBW1=(max+1)*2;
    qDebug()<<" nbw1 "<<NBW1;


    double tolerance = 1e-6; // 设置一个容差值，用于比较点的坐标
    for (const auto &userPoint : userPoints) {
        bool found = false;

        for (size_t i = 0; i < coord.size() / 3; ++i) {
            double x = coord[i * 3];
            double y = coord[i * 3 + 1];

            // 如果顶点在边界上，并且接近用户给定的点，则设置 found 为 true
            if (y==kuan_2/2 && fabs(x - userPoint.x) < tolerance ) {
                found = true;
            }
            if (y==-kuan_2/2 && fabs(x - userPoint.x) < tolerance ) {
                found = true;
            }
            if (x==-chang_2/2 && fabs(y - userPoint.y) < tolerance ) {
                found = true;
            }
            if (x==chang_2/2 && fabs(y - userPoint.y) < tolerance ) {
                found = true;
            }
        }
        if (found) {
            qDebug() << "A mesh vertex was found close to user point (" << userPoint.x << ", " << userPoint.y << ")";
        } else {
            qDebug() << "No mesh vertex was found close to user point (" << userPoint.x << ", " << userPoint.y << ")";
        }

    }


        // 保存网格到文件
       gmsh::write("RectangleWithHoles.msh");

    gmsh::finalize();


    ss=1;
}


void MainWindow::ESM(int NE,double EK[][6],double S[][6])
{
   double Xji=vertices[NE*3+1].x-vertices[NE*3].x,Xik=vertices[NE*3].x-vertices[NE*3+2].x,Xkj=vertices[NE*3+2].x-vertices[NE*3+1].x,A=0.5*(vertices[NE*3+2].y*Xji+vertices[NE*3+1].y*Xik+vertices[NE*3].y*Xkj);
   double B[3][6]={{0}},D[3][3]={{0}};
   B[0][0]=vertices[NE*3+1].y-vertices[NE*3+2].y;B[0][2]=vertices[NE*3+2].y-vertices[NE*3].y;B[0][4]=vertices[NE*3].y-vertices[NE*3+1].y;
   B[1][1]=Xkj;B[1][3]=Xik;B[1][5]=Xji;
   B[2][0]=B[1][1];B[2][1]=B[0][0];B[2][2]=B[1][3];B[2][3]=B[0][2];B[2][4]=B[1][5];B[2][5]=B[0][4];
   for(int m=0;m<3;m++)
     for(int n=0;n<6;n++) B[m][n]=0.5*B[m][n]/A;
   double DU=tanxinmoliang_2/(1-bosongbi_2*bosongbi_2);D[0][0]=DU;D[0][1]=DU*bosongbi_2;D[1][0]=D[0][1];D[1][1]=DU;D[2][2]=0.5*DU*(1-bosongbi_2);
   for(int m=0;m<3;m++)
       for(int n=0;n<6;n++)
       {S[m][n]=0;
       for(int k=0;k<3;k++) S[m][n]=S[m][n]+D[m][k]*B[k][n];}
   for(int i=0;i<6;i++)
       for(int j=0;j<6;j++)
       {
           EK[i][j]=0;
           for(int k=0;k<3;k++) EK[i][j]=EK[i][j]+B[k][i]*S[k][j]*A;
       }
}

void MainWindow::ESMgaojie(int NE,double EK[][12])     //注意verticesgaojie中每六个存的点的顺序与六节点单元的六点逆时针顺序不同
{
   double Xji=verticesgaojie[NE*6+2].x-verticesgaojie[NE*6].x,Xik=verticesgaojie[NE*6].x-verticesgaojie[NE*6+4].x,Xkj=verticesgaojie[NE*6+4].x-verticesgaojie[NE*6+2].x,A=0.5*(verticesgaojie[NE*6+4].y*Xji+verticesgaojie[NE*6+2].y*Xik+verticesgaojie[NE*6].y*Xkj);
   double ai=verticesgaojie[NE*6+2].x*verticesgaojie[NE*6+4].y-verticesgaojie[NE*6+4].x*verticesgaojie[NE*6+2].y,bi=verticesgaojie[NE*6+2].y-verticesgaojie[NE*6+4].y,ci=verticesgaojie[NE*6+4].x-verticesgaojie[NE*6+2].x,
          aj=verticesgaojie[NE*6+4].x*verticesgaojie[NE*6].y-verticesgaojie[NE*6+4].y*verticesgaojie[NE*6].x,bj=verticesgaojie[NE*6+4].y-verticesgaojie[NE*6].y,cj=verticesgaojie[NE*6].x-verticesgaojie[NE*6+4].x,
          ak=verticesgaojie[NE*6].x*verticesgaojie[NE*6+2].y-verticesgaojie[NE*6+2].x*verticesgaojie[NE*6].y,bk=verticesgaojie[NE*6].y-verticesgaojie[NE*6+2].y,ck=verticesgaojie[NE*6+2].x-verticesgaojie[NE*6].x;
   double Ai[2][2]={{0}},Aj[2][2]={{0}},Ak[2][2]={{0}},Bi[2][2]={{0}},Bj[2][2]={{0}},Bk[2][2]={{0}},Gij[2][2]={{0}},Gik[2][2]={{0}},Gjk[2][2]={{0}},Dij[2][2]={{0}},Dik[2][2]={{0}},Djk[2][2]={{0}};
   Ai[0][0]=6*bi*bi+3*(1-bosongbi_2)*ci*ci;Ai[0][1]=3*(1+bosongbi_2)*bi*ci;Ai[1][0]=Ai[0][1];Ai[1][1]=6*ci*ci+3*(1-bosongbi_2)*bi*bi;
   Aj[0][0]=6*bj*bj+3*(1-bosongbi_2)*cj*cj;Aj[0][1]=3*(1+bosongbi_2)*bj*cj;Aj[1][0]=Aj[0][1];Aj[1][1]=6*cj*cj+3*(1-bosongbi_2)*bj*bj;
   Ak[0][0]=6*bk*bk+3*(1-bosongbi_2)*ck*ck;Ak[0][1]=3*(1+bosongbi_2)*bk*ck;Ak[1][0]=Ak[0][1];Ak[1][1]=6*ck*ck+3*(1-bosongbi_2)*bk*bk;
   Bi[0][0]=16*(bi*bi-bj*bk)+8*(1-bosongbi_2)*(ci*ci-cj*ck);Bi[0][1]=4*(1+bosongbi_2)*(bi*ci+bj*cj+bk*ck);Bi[1][0]=Bi[0][1];Bi[1][1]=16*(ci*ci-cj*ck)+8*(1-bosongbi_2)*(bi*bi-bj*bk);
   Bj[0][0]=16*(bj*bj-bi*bk)+8*(1-bosongbi_2)*(cj*cj-ci*ck);Bj[0][1]=Bi[0][1];Bj[1][0]=Bi[0][1];Bj[1][1]=16*(cj*cj-ci*ck)+8*(1-bosongbi_2)*(bj*bj-bi*bk);
   Bk[0][0]=16*(bk*bk-bi*bj)+8*(1-bosongbi_2)*(ck*ck-ci*cj);Bk[0][1]=Bi[0][1];Bk[1][0]=Bi[0][1];Bk[1][1]=16*(ck*ck-ci*cj)+8*(1-bosongbi_2)*(bk*bk-bi*bj);
   Gij[0][0]=-2*bi*bj-(1-bosongbi_2)*ci*cj;Gij[0][1]=-2*bosongbi_2*bi*cj-(1-bosongbi_2)*ci*bj;Gij[1][0]=-2*bosongbi_2*ci*bj-(1-bosongbi_2)*bi*cj;Gij[1][1]=-2*ci*cj-(1-bosongbi_2)*bi*bj;
   Gik[0][0]=-2*bi*bk-(1-bosongbi_2)*ci*ck;Gik[0][1]=-2*bosongbi_2*bi*ck-(1-bosongbi_2)*ci*bk;Gik[1][0]=-2*bosongbi_2*ci*bk-(1-bosongbi_2)*bi*ck;Gik[1][1]=-2*ci*ck-(1-bosongbi_2)*bi*bk;
   Gjk[0][0]=-2*bk*bj-(1-bosongbi_2)*ck*cj;Gjk[0][1]=-2*bosongbi_2*bj*ck-(1-bosongbi_2)*cj*bk;Gjk[1][0]=-2*bosongbi_2*cj*bk-(1-bosongbi_2)*bj*ck;Gjk[1][1]=-2*cj*ck-(1-bosongbi_2)*bk*bj;
   Dij[0][0]=16*bi*bj+8*(1-bosongbi_2)*ci*cj;Dij[0][1]=4*(1+bosongbi_2)*(ci*bj+bi*cj);Dij[1][0]=Dij[0][1];Dij[1][1]=16*ci*cj+8*(1-bosongbi_2)*bi*bj;
   Dik[0][0]=16*bi*bk+8*(1-bosongbi_2)*ci*ck;Dik[0][1]=4*(1+bosongbi_2)*(ci*bk+bi*ck);Dik[1][0]=Dik[0][1];Dik[1][1]=16*ci*ck+8*(1-bosongbi_2)*bi*bk;
   Djk[0][0]=16*bj*bk+8*(1-bosongbi_2)*cj*ck;Djk[0][1]=4*(1+bosongbi_2)*(cj*bk+bj*ck);Djk[1][0]=Djk[0][1];Djk[1][1]=16*cj*ck+8*(1-bosongbi_2)*bj*bk;

   for(int i=0;i<2;i++)
       for(int j=0;j<2;j++)
       {EK[i][j]=Ai[i][j];}
   for(int i=0;i<2;i++)
       for(int j=2;j<4;j++)
       {EK[i][j]=Gij[i][j-2];}
   for(int i=0;i<2;i++)
       for(int j=4;j<6;j++)
       {EK[i][j]=Gik[i][j-4];}
   for(int i=0;i<2;i++)
       for(int j=6;j<8;j++)
       {EK[i][j]=0;}
   for(int i=0;i<2;i++)
       for(int j=8;j<10;j++)
       {EK[i][j]=-4*Gik[i][j-8];}
   for(int i=0;i<2;i++)
       for(int j=10;j<12;j++)
       {EK[i][j]=-4*Gij[i][j-10];}
   for(int i=2;i<4;i++)
       for(int j=2;j<4;j++)
       {EK[i][j]=Aj[i-2][j-2];}
   for(int i=2;i<4;i++)
       for(int j=4;j<6;j++)
       {EK[i][j]=Gjk[i-2][j-4];}
   for(int i=2;i<4;i++)
       for(int j=6;j<8;j++)
       {EK[i][j]=-4*Gjk[i-2][j-6];}
   for(int i=2;i<4;i++)
       for(int j=8;j<10;j++)
       {EK[i][j]=0;}
   for(int i=2;i<4;i++)
       for(int j=10;j<12;j++)
       {EK[i][j]=EK[i-2][j];}EK[2][11]=EK[1][10];EK[3][10]=EK[0][11];
   for(int i=4;i<6;i++)
       for(int j=4;j<6;j++)
       {EK[i][j]=Ak[i-4][j-4];}
   for(int i=4;i<6;i++)
       for(int j=6;j<8;j++)
       {EK[i][j]=EK[i-2][j];}EK[5][6]=EK[2][7];EK[4][7]=EK[3][6];
   for(int i=4;i<6;i++)
       for(int j=8;j<10;j++)
       {EK[i][j]=EK[i-4][j];}EK[4][9]=EK[1][8];EK[5][8]=EK[0][9];
   for(int i=4;i<6;i++)
       for(int j=10;j<12;j++)
       {EK[i][j]=0;}
   for(int i=6;i<8;i++)
       for(int j=6;j<8;j++)
       {EK[i][j]=Bi[i-6][j-6];}
   for(int i=6;i<8;i++)
       for(int j=8;j<10;j++)
       {EK[i][j]=Dij[i-6][j-8];}
   for(int i=6;i<8;i++)
       for(int j=10;j<12;j++)
       {EK[i][j]=Dik[i-6][j-10];}
   for(int i=8;i<10;i++)
       for(int j=8;j<10;j++)
       {EK[i][j]=Bj[i-8][j-8];}
   for(int i=8;i<10;i++)
       for(int j=10;j<12;j++)
       {EK[i][j]=Djk[i-8][j-10];}
   for(int i=10;i<12;i++)
       for(int j=10;j<12;j++)
       {EK[i][j]=Bk[i-10][j-10];}

   for(int i=1;i<12;i++)
       for(int j=0;j<i;j++)
           EK[i][j]=EK[j][i];

   for(int i=0;i<12;i++)
       for(int j=0;j<12;j++)
       {
           EK[i][j]=EK[i][j]*tanxinmoliang_2/(24*A*(1-bosongbi_2*bosongbi_2));
       }
}

bool compareYAscend(const Vertex1 &a, const Vertex1 &b)
{
    return a.y <= b.y;
}

bool compareYDescend(const Vertex1 &a, const Vertex1 &b)
{
    return a.y >= b.y;
}

bool compareXAscend(const Vertex1 &a, const Vertex1 &b)
{
    return a.x <= b.x;
}

bool compareXDescend(const Vertex1 &a, const Vertex1 &b)
{
    return a.x >= b.x;
}

void MainWindow::Load(double P[])
{   
for(int a=0;a<MNN*2;a++)
    P[a]=0;

QList<Vertex1> junzuo;
QList<Vertex1> junyou;
QList<Vertex1> junshang;
QList<Vertex1> junxia;
for(const auto &v:vertices1)
{
    for (const auto &s:shouliList)
{
    if(v.x==s.duandian)
    if(s.a=="jizhongli")
    {
        if(s.b=="shang"&&v.y==kuan_2/2)
            P[(v.id-1)*2+1]=P[(v.id-1)*2+1]-s.li;
        if(s.b=="xia"&&v.y==-kuan_2/2)
            P[(v.id-1)*2+1]=P[(v.id-1)*2+1]+s.li;
    }
    if(v.y==-s.duandian)
        if(s.a=="jizhongli")
    {
        if(s.b=="zuo"&&v.x==-chang_2/2)
            P[(v.id-1)*2]=P[(v.id-1)*2]-s.li;
        if(s.b=="you"&&v.x==chang_2/2)
            P[(v.id-1)*2]=P[(v.id-1)*2]+s.li;
    }
    if(v.x<=s.duandian1&&v.x>=s.duandian)
    {if(s.a=="junbuli"&&s.b=="shang"&&v.y==kuan_2/2)
            junshang.append({v.id,v.x,v.y,s.li});
        else if(s.a=="junbuli"&&s.b=="xia"&&v.y==-kuan_2/2)
            junxia.append({v.id,v.x,v.y,s.li});}
    if(v.y<=-s.duandian&&v.y>=-s.duandian1)
    {if(s.a=="junbuli"&&s.b=="zuo"&&v.x==-chang_2/2)
            junzuo.append({v.id,v.x,v.y,s.li});
        else if(s.a=="junbuli"&&s.b=="you"&&v.x==chang_2/2)
            junyou.append({v.id,v.x,v.y,s.li});}
}
}

std::sort(junzuo.begin(), junzuo.end(), compareYDescend);
std::sort(junshang.begin(), junshang.end(), compareXDescend);
std::sort(junyou.begin(), junyou.end(), compareYAscend);
std::sort(junxia.begin(), junxia.end(), compareXAscend);

for(int i=0;i<junzuo.size()-1;i++)
{
    P[(junzuo[i].id-1)*2]=P[(junzuo[i].id-1)*2]-junzuo[i].li*0.5*fabs(junzuo[i].y-junzuo[i+1].y);
    P[(junzuo[i+1].id-1)*2]=P[(junzuo[i+1].id-1)*2]-junzuo[i].li*0.5*fabs(junzuo[i].y-junzuo[i+1].y);
}
for(int i=0;i<junyou.size()-1;i++)
{
    P[(junyou[i].id-1)*2]=P[(junyou[i].id-1)*2]+junyou[i].li*0.5*fabs(junyou[i].y-junyou[i+1].y);
    P[(junyou[i+1].id-1)*2]=P[(junyou[i+1].id-1)*2]+junyou[i].li*0.5*fabs(junyou[i].y-junyou[i+1].y);
}
for(int i=0;i<junshang.size()-1;i++)
{
    P[(junshang[i].id-1)*2+1]=P[(junshang[i].id-1)*2+1]-junshang[i].li*0.5*fabs(junshang[i].x-junshang[i+1].x);
    P[(junshang[i+1].id-1)*2+1]=P[(junshang[i+1].id-1)*2+1]-junshang[i].li*0.5*fabs(junshang[i].x-junshang[i+1].x);
}
for(int i=0;i<junxia.size()-1;i++)
{
    P[(junxia[i].id-1)*2+1]=P[(junxia[i].id-1)*2+1]+junxia[i].li*0.5*fabs(junxia[i].x-junxia[i+1].x);
    P[(junxia[i+1].id-1)*2+1]=P[(junxia[i+1].id-1)*2+1]+junxia[i].li*0.5*fabs(junxia[i].x-junxia[i+1].x);
}
}

void MainWindow::Loadgaojie(double P[])
{
    for(int a=0;a<MNN1*2;a++)
        P[a]=0;

    QList<Vertex1> junzuo;
    QList<Vertex1> junyou;
    QList<Vertex1> junshang;
    QList<Vertex1> junxia;
    for(const auto &v:verticesgaojie1)
    {
        for (const auto &s:shouliList)
        {
            if(v.x==s.duandian)
            if(s.a=="jizhongli")
            {
                if(s.b=="shang"&&v.y==kuan_2/2)
                    P[(v.id-1)*2+1]=P[(v.id-1)*2+1]-s.li;
                if(s.b=="xia"&&v.y==-kuan_2/2)
                    P[(v.id-1)*2+1]=P[(v.id-1)*2+1]+s.li;
            }
            if(v.y==-s.duandian)
                if(s.a=="jizhongli")
            {
                if(s.b=="zuo"&&v.x==-chang_2/2)
                    P[(v.id-1)*2]=P[(v.id-1)*2]-s.li;
                if(s.b=="you"&&v.x==chang_2/2)
                    P[(v.id-1)*2]=P[(v.id-1)*2]+s.li;
            }
            if(v.x<=s.duandian1&&v.x>=s.duandian)
            {if(s.a=="junbuli"&&s.b=="shang"&&v.y==kuan_2/2)
                    junshang.append({v.id,v.x,v.y,s.li});
                else if(s.a=="junbuli"&&s.b=="xia"&&v.y==-kuan_2/2)
                    junxia.append({v.id,v.x,v.y,s.li});}
            if(v.y<=-s.duandian&&v.y>=-s.duandian1)
            {if(s.a=="junbuli"&&s.b=="zuo"&&v.x==-chang_2/2)
                    junzuo.append({v.id,v.x,v.y,s.li});
                else if(s.a=="junbuli"&&s.b=="you"&&v.x==chang_2/2)
                    junyou.append({v.id,v.x,v.y,s.li});}
        }
        }

    std::sort(junzuo.begin(), junzuo.end(), compareYDescend);
    std::sort(junshang.begin(), junshang.end(), compareXDescend);
    std::sort(junyou.begin(), junyou.end(), compareYAscend);
    std::sort(junxia.begin(), junxia.end(), compareXAscend);

    for(int i=0;i<junzuo.size()-1;i++)
    {
        P[(junzuo[i].id-1)*2]=P[(junzuo[i].id-1)*2]-junzuo[i].li*0.5*fabs(junzuo[i].y-junzuo[i+1].y);
        P[(junzuo[i+1].id-1)*2]=P[(junzuo[i+1].id-1)*2]-junzuo[i].li*0.5*fabs(junzuo[i].y-junzuo[i+1].y);
    }
    for(int i=0;i<junyou.size()-1;i++)
    {
        P[(junyou[i].id-1)*2]=P[(junyou[i].id-1)*2]+junyou[i].li*0.5*fabs(junyou[i].y-junyou[i+1].y);
        P[(junyou[i+1].id-1)*2]=P[(junyou[i+1].id-1)*2]+junyou[i].li*0.5*fabs(junyou[i].y-junyou[i+1].y);
    }
    for(int i=0;i<junshang.size()-1;i++)
    {
        P[(junshang[i].id-1)*2+1]=P[(junshang[i].id-1)*2+1]-junshang[i].li*0.5*fabs(junshang[i].x-junshang[i+1].x);
        P[(junshang[i+1].id-1)*2+1]=P[(junshang[i+1].id-1)*2+1]-junshang[i].li*0.5*fabs(junshang[i].x-junshang[i+1].x);
    }
    for(int i=0;i<junxia.size()-1;i++)
    {
        P[(junxia[i].id-1)*2+1]=P[(junxia[i].id-1)*2+1]+junxia[i].li*0.5*fabs(junxia[i].x-junxia[i+1].x);
        P[(junxia[i+1].id-1)*2+1]=P[(junxia[i+1].id-1)*2+1]+junxia[i].li*0.5*fabs(junxia[i].x-junxia[i+1].x);
    }
}

void MainWindow::GSM(double** GK)
{
    for(int i=0;i<2*MNN;i++)
        for(int j=0;j<NBW;j++)
            GK[i][j]=0;
    for(int NE=0;NE<=MNE;NE++)
    {
        double EK[6][6],S[3][6];
        ESM(NE,EK,S);
        for(int IR=1;IR<=3;IR++)
        {for(int I2=0;I2<=1;I2++)
            {int NRE=(IR-1)*2+I2;
             int NRG=2*(vertices[NE*3+IR-1].id-1)+I2;
             for(int JC=1;JC<=3;JC++)
             {
                 for(int J2=0;J2<=1;J2++)
                 {
                     int NCE=2*(JC-1)+J2;
                     int NCG=2*(vertices[NE*3+JC-1].id-1)+J2;
                     int ND=NCG+1-NRG;
                     if (ND<=0) continue;
                     GK[NRG][ND-1]=GK[NRG][ND-1]+EK[NRE][NCE];
                 }
             }
            }
        }
        int a=30*NE/MNE;
        ui->progressBar->setValue(a);
    }
}

void MainWindow::GSMgaojie(double** GK)
{
    for(int i=0;i<2*MNN1;i++)
        for(int j=0;j<2*MNN1;j++)
            GK[i][j]=0;
    int m=0;

    for(int NE=0;NE<=MNE;NE++)
    {
        double EK[12][12];
        ESMgaojie(NE,EK);
        int ii=0,jj=0;
        {       jj=0;
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE].id-1)]+=EK[ii+1][jj];
                jj=2;
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii+1][jj];
                jj=4;
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii+1][jj];
                jj=6;
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii+1][jj];
                jj=8;
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii+1][jj];
                jj=10;
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE].id-1)][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii+1][jj];
        }
        ii=2;
        {       jj=0;
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE].id-1)]+=EK[ii+1][jj];
                jj=2;
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii+1][jj];
                jj=4;
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii+1][jj];
                jj=6;
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii+1][jj];
                jj=8;
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii+1][jj];
                jj=10;
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+2].id-1)][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+2].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii+1][jj];
        }
        ii=4;
        {       jj=0;
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE].id-1)]+=EK[ii+1][jj];
                jj=2;
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii+1][jj];
                jj=4;
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii+1][jj];
                jj=6;
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii+1][jj];
                jj=8;
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii+1][jj];
                jj=10;
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+4].id-1)][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+4].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii+1][jj];
        }
        ii=6;
        {       jj=0;
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE].id-1)]+=EK[ii+1][jj];
                jj=2;
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii+1][jj];
                jj=4;
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii+1][jj];
                jj=6;
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii+1][jj];
                jj=8;
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii+1][jj];
                jj=10;
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+3].id-1)][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+3].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii+1][jj];
        }
        ii=8;
        {       jj=0;
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE].id-1)]+=EK[ii+1][jj];
                jj=2;
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii+1][jj];
                jj=4;
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii+1][jj];
                jj=6;
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii+1][jj];
                jj=8;
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii+1][jj];
                jj=10;
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+5].id-1)][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+5].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii+1][jj];
        }
        ii=10;
        {       jj=0;
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE].id-1)]+=EK[ii+1][jj];
                jj=2;
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+2].id-1)]+=EK[ii+1][jj];
                jj=4;
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+4].id-1)]+=EK[ii+1][jj];
                jj=6;
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+3].id-1)]+=EK[ii+1][jj];
                jj=8;
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+5].id-1)]+=EK[ii+1][jj];
                jj=10;
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii][jj];
                GK[2*(verticesgaojie[6*NE+1].id-1)][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)+1]+=EK[ii+1][jj+1];
                GK[2*(verticesgaojie[6*NE+1].id-1)+1][2*(verticesgaojie[6*NE+1].id-1)]+=EK[ii+1][jj];
        }

        int a=30*NE/MNE;
        ui->progressBar_2->setValue(a);
    }

    int ij=0;
    for(int i=0;i<2*MNN1;i++)
    {for(int j=i;j<2*MNN1;j++)
        {
            if(GK[i][j]!=GK[j][i]) qDebug()<<"GKwrong";
            if(GK[i][i]!=0 && GK[i][j]!=0)
                 ij=j;}
            if((ij-i)+1>m) m=(ij-i)+1;
        }
    qDebug()<<m<<" NBW "<<NBW1;

}

void AGM(double** GK,int NBW,QList<int> yue,double P[])
{
    for(const auto &y:yue)
    {
        GK[y][0]=1;
        for(int j=1;j<NBW;j++)
            GK[y][j]=0;
        int Ju=std::min(y,NBW-1);
        for(int J=1;J<=Ju;J++) GK[y-J][J]=0;
        P[y]=0;
    }
}

void AGMgaojie(double** GK,QList<int> yue,double P[])
{
    for(const auto &y:yue)
    {
        for(int j=0;j<2*MNN1;j++)
            GK[y][j]=0;
        for(int j=0;j<2*MNN1;j++)
            GK[j][y]=0;
        GK[y][y]=1;
        P[y]=0;
    }

}

void MainWindow::Solver(double** GK,double P[],int NBW,int MNN)
{
    Eigen::SparseMatrix<double> GK1(MNN*2,MNN*2);//默认全0
    Eigen::VectorXd v(MNN*2),x(MNN*2);
    int a=0;
    std::vector<Eigen::Triplet<double>> tripletList;

    for(int i=0;i<2*MNN;i++)
    {
        v(i)=P[i];
    }

    for(int i = 0; i <= 2 * MNN - NBW; i++)
    {
        int w = 0;
        for(int j = a; j < a + NBW; j++)
        {
            tripletList.push_back(Eigen::Triplet<double>(i, j, GK[i][w]));
            w++;
        }
        a++;
    }

a=1;
    for(int i=2*MNN-NBW+1;i<2*MNN;i++)
    {    int w=0;
        for(int j=2*MNN-(NBW-a);j<=2*MNN-1;j++)
        {
            tripletList.push_back(Eigen::Triplet<double>(i, j, GK[i][w]));w++;
        }
        a++;
    }
    ui->progressBar->setValue(35);

    GK1.setFromTriplets(tripletList.begin(), tripletList.end());

    Eigen::SparseMatrix<double> colMajorGK1Transpose = GK1.transpose().eval();
    Eigen::SparseMatrix<double> symmetric_matrix = GK1 + colMajorGK1Transpose;
    // 对角线元素被加了两次，需要减去一次
    symmetric_matrix.diagonal() *= 0.5;

    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
//    solver.setMaxIterations(5000);
//    solver.setTolerance(1e-6);
    x=solver.compute(symmetric_matrix).solve(v);


    for (int i = 0; i < 2*MNN; ++i) {
        P[i] = x(i);
    }

    ui->progressBar->setValue(75);

    for(int a=0;a<MNN*2;a++) qDebug()<<" a "<<P[a];
}

void MainWindow:: Solvergaojie(double** GK,double P[])
{
    Eigen::SparseMatrix<double> GK1(MNN1*2,MNN1*2);//默认全0
    Eigen::VectorXd v(MNN1*2),x(MNN1*2);
    int a=0;std::vector<Eigen::Triplet<double>> tripletList;

    for(int i=0;i<2*MNN1;i++)
    {
        v(i)=P[i];
    }

    for(int i = 0; i <= 2 * MNN1 - NBW1; i++)
    {
        for(int j = i; j < i + NBW1; j++)
        {
            tripletList.push_back(Eigen::Triplet<double>(i, j, GK[i][j]));
        }
    }

    a=1;
    for(int i=2*MNN1-NBW1+1;i<2*MNN1;i++)
    {
        for(int j=2*MNN1-(NBW1-a);j<=2*MNN1-1;j++)
        {
            tripletList.push_back(Eigen::Triplet<double>(i, j, GK[i][j]));
        }
        a++;
    }
    ui->progressBar_2->setValue(35);

    GK1.setFromTriplets(tripletList.begin(), tripletList.end());

    Eigen::SparseMatrix<double> colMajorGK1Transpose = GK1.transpose().eval();
    Eigen::SparseMatrix<double> symmetric_matrix = GK1 + colMajorGK1Transpose;
    // 对角线元素被加了两次，需要减去一次
    symmetric_matrix.diagonal() *= 0.5;

    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
//    solver.setMaxIterations(5000);
//    solver.setTolerance(1e-6);
    x=solver.compute(symmetric_matrix).solve(v);


    for (int i = 0; i < 2*MNN1; ++i) {
        P[i] = x(i);
    }

    ui->progressBar_2->setValue(75);

    for(int a=0;a<MNN1*2;a++) qDebug()<<" a "<<P[a];
}

void MainWindow::Stress(double P[],double ST[][3],double SE[])   //SE[3*(MNE+1)] ,NE三角形的三应力sigmax,sigmay,tau分别为SE[3*NE],SE[3*NE+1],SE[3*NE+2]
{                                                                //ST[MNN][3],a编号节点的三应力sigmax,sigmay,tau分别为ST[a-1][0,1,2]   
    int NT[MNN];double DE[6];
    for(int NN=1;NN<=MNN;NN++)
    {
        NT[NN-1]=0;
        for(int J=1;J<=3;J++)
        {
            ST[NN-1][J-1]=0;
        }
    }
    for(int NE=0;NE<=MNE;NE++)
    {
        double EK[6][6],S[3][6];
        ESM(NE,EK,S);
        for(int I=1;I<=3;I++)
            for(int J=1;J<=2;J++)
            {
                int NRE=2*(I-1)+J;
                int NRG=2*(vertices[NE*3+I-1].id-1)+J;
                DE[NRE-1]=P[NRG-1];
            }
        int i=vertices[NE*3].id;
        int j=vertices[NE*3+1].id;
        int k=vertices[NE*3+2].id;
        for(int M=1;M<=3;M++)
        {
            SE[3*NE+M-1]=0;
            int IR=vertices[NE*3+M-1].id;
            NT[IR-1]=NT[IR-1]+1;
            for(int N=1;N<=6;N++)
            {
                SE[3*NE+M-1]=SE[3*NE+M-1]+S[M-1][N-1]*DE[N-1];
            }
            ST[i-1][M-1]=ST[i-1][M-1]+SE[3*NE+M-1];
            ST[j-1][M-1]=ST[j-1][M-1]+SE[3*NE+M-1];
            ST[k-1][M-1]=ST[k-1][M-1]+SE[3*NE+M-1];
        }
        int a=75+20*NE/MNE;
        ui->progressBar->setValue(a);
    }
    for(int I=1;I<=MNN;I++)
        for(int J=1;J<=3;J++)
            ST[I-1][J-1]=ST[I-1][J-1]/NT[I-1];
}

void MainWindow::Stressgaojie(double P[],double ST[][3],double SE[])
{
    int NT[MNN1];double DE[12],S[3][12];
    for(int NN=1;NN<=MNN1;NN++)
    {
        NT[NN-1]=0;
        for(int J=1;J<=3;J++)
        {
            ST[NN-1][J-1]=0;
        }
    }

    for(int NE=0;NE<=MNE;NE++)
    {
        SE[3*NE]=0;SE[3*NE+1]=0;SE[3*NE+2]=0;
        double Xji=verticesgaojie[NE*6+2].x-verticesgaojie[NE*6].x,Xik=verticesgaojie[NE*6].x-verticesgaojie[NE*6+4].x,Xkj=verticesgaojie[NE*6+4].x-verticesgaojie[NE*6+2].x,A=0.5*(verticesgaojie[NE*6+4].y*Xji+verticesgaojie[NE*6+2].y*Xik+verticesgaojie[NE*6].y*Xkj);
        double ai=verticesgaojie[NE*6+2].x*verticesgaojie[NE*6+4].y-verticesgaojie[NE*6+4].x*verticesgaojie[NE*6+2].y,bi=verticesgaojie[NE*6+2].y-verticesgaojie[NE*6+4].y,ci=verticesgaojie[NE*6+4].x-verticesgaojie[NE*6+2].x,
               aj=verticesgaojie[NE*6+4].x*verticesgaojie[NE*6].y-verticesgaojie[NE*6+4].y*verticesgaojie[NE*6].x,bj=verticesgaojie[NE*6+4].y-verticesgaojie[NE*6].y,cj=verticesgaojie[NE*6].x-verticesgaojie[NE*6+4].x,
               ak=verticesgaojie[NE*6].x*verticesgaojie[NE*6+2].y-verticesgaojie[NE*6+2].x*verticesgaojie[NE*6].y,bk=verticesgaojie[NE*6].y-verticesgaojie[NE*6+2].y,ck=verticesgaojie[NE*6+2].x-verticesgaojie[NE*6].x;
        double EK[12][12];
        ESMgaojie(NE,EK);
        int i=0;
        {
            DE[i]=P[2*(verticesgaojie[NE*6].id-1)];
            DE[i+1]=P[2*(verticesgaojie[NE*6].id-1)+1];
            NT[verticesgaojie[NE*6].id-1]++;
        }
        i=2;
        {
            DE[i]=P[2*(verticesgaojie[NE*6+2].id-1)];
            DE[i+1]=P[2*(verticesgaojie[NE*6+2].id-1)+1];
            NT[verticesgaojie[NE*6+2].id-1]++;
        }
        i=4;
        {
            DE[i]=P[2*(verticesgaojie[NE*6+4].id-1)];
            DE[i+1]=P[2*(verticesgaojie[NE*6+4].id-1)+1];
            NT[verticesgaojie[NE*6+4].id-1]++;
        }
        i=6;
        {
            DE[i]=P[2*(verticesgaojie[NE*6+3].id-1)];
            DE[i+1]=P[2*(verticesgaojie[NE*6+3].id-1)+1];
            NT[verticesgaojie[NE*6+3].id-1]++;
        }
        i=8;
        {
            DE[i]=P[2*(verticesgaojie[NE*6+5].id-1)];
            DE[i+1]=P[2*(verticesgaojie[NE*6+5].id-1)+1];
            NT[verticesgaojie[NE*6+5].id-1]++;
        }
        i=10;
        {
            DE[i]=P[2*(verticesgaojie[NE*6+1].id-1)];
            DE[i+1]=P[2*(verticesgaojie[NE*6+1].id-1)+1];
            NT[verticesgaojie[NE*6+1].id-1]++;
        }

        //Li,j,k中的x和y为该单元的重心坐标
        int x=(verticesgaojie[NE*6].x+verticesgaojie[NE*6+2].x+verticesgaojie[NE*6+4].x)/3,y=(verticesgaojie[NE*6].y+verticesgaojie[NE*6+2].y+verticesgaojie[NE*6+4].y)/3;
        int Li=(ai+bi*x+ci*y)/(2*A),Lj=(aj+bj*x+cj*y)/(2*A),Lk=(ak+bk*x+ck*y)/(2*A);
        S[0][0]=2*bi;S[0][1]=2*ci*bosongbi_2;S[1][0]=2*bi*bosongbi_2;S[1][1]=2*ci;S[2][0]=(1-bosongbi_2)*ci;S[2][1]=(1-bosongbi_2)*bi;
        S[0][2]=2*bj;S[0][3]=2*cj*bosongbi_2;S[1][2]=2*bj*bosongbi_2;S[1][3]=2*cj;S[2][2]=(1-bosongbi_2)*cj;S[2][3]=(1-bosongbi_2)*bj;
        S[0][4]=2*bk;S[0][5]=2*ck*bosongbi_2;S[1][4]=2*bk*bosongbi_2;S[1][5]=2*ck;S[2][4]=(1-bosongbi_2)*ck;S[2][5]=(1-bosongbi_2)*bk;
        for(int i=0;i<3;i++)
        {for(int k=0;k<2;k++)
                S[i][k]=S[i][k]*tanxinmoliang_2*(4*Li-1)/(4*A*(1-bosongbi_2*bosongbi_2));
            for(int k=2;k<4;k++)
                S[i][k]=S[i][k]*tanxinmoliang_2*(4*Lj-1)/(4*A*(1-bosongbi_2*bosongbi_2));
            for(int k=4;k<6;k++)
                S[i][k]=S[i][k]*tanxinmoliang_2*(4*Lk-1)/(4*A*(1-bosongbi_2*bosongbi_2));
        }
        S[0][6]=8*(bj*Lk+bk*Lj);S[0][7]=8*bosongbi_2*(cj*Lk+ck*Lj);S[1][6]=8*bosongbi_2*(bj*Lk+bk*Lj);S[1][7]=8*(cj*Lk+ck*Lj);S[2][6]=4*(1-bosongbi_2)*(cj*Lk+ck*Lj);S[2][7]=4*(1-bosongbi_2)*(bj*Lk+bk*Lj);
        S[0][8]=8*(bi*Lk+bk*Li);S[0][9]=8*bosongbi_2*(ci*Lk+ck*Li);S[1][8]=8*bosongbi_2*(bi*Lk+bk*Li);S[1][9]=8*(ci*Lk+ck*Li);S[2][8]=4*(1-bosongbi_2)*(ci*Lk+ck*Li);S[2][9]=4*(1-bosongbi_2)*(bi*Lk+bk*Li);
        S[0][10]=8*(bj*Li+bi*Lj);S[0][11]=8*bosongbi_2*(cj*Li+ci*Lj);S[1][10]=8*bosongbi_2*(bj*Li+bi*Lj);S[1][11]=8*(cj*Li+ci*Lj);S[2][10]=4*(1-bosongbi_2)*(cj*Li+ci*Lj);S[2][11]=4*(1-bosongbi_2)*(bj*Li+bi*Lj);
        for(int i=0;i<3;i++)
        {for(int k=6;k<12;k++)
                S[i][k]=S[i][k]*tanxinmoliang_2/(4*A*(1-bosongbi_2*bosongbi_2));
        }

        for(int i=0;i<3;i++)
        {int a=0;
            for(int j=0;j<12;j++)
            {
               a+=S[i][j]*DE[j];
            }
            SE[3*NE+i]=a;
        }

        for(int j=0;j<3;j++)
        {
        ST[verticesgaojie[NE*6].id-1][j]+=SE[3*NE+j];
        ST[verticesgaojie[NE*6+1].id-1][j]+=SE[3*NE+j];
        ST[verticesgaojie[NE*6+2].id-1][j]+=SE[3*NE+j];
        ST[verticesgaojie[NE*6+3].id-1][j]+=SE[3*NE+j];
        ST[verticesgaojie[NE*6+4].id-1][j]+=SE[3*NE+j];
        ST[verticesgaojie[NE*6+5].id-1][j]+=SE[3*NE+j];
        }

        int a=75+20*NE/MNE;
        ui->progressBar_2->setValue(a);
    }

    for(int I=1;I<=MNN1;I++)
        for(int J=1;J<=3;J++)
            ST[I-1][J-1]=ST[I-1][J-1]/NT[I-1];

}

void MainWindow::on_jisuan_2_triggered()
{
if(tanxinmoliang_2==INFINITY||bosongbi_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义材料！"));return;}
if(ss==0){QMessageBox::warning(this, tr("错误"), tr("先生成网格！"));return;}
if(jjj==1||jjj==2) {QMessageBox::warning(this, tr("错误"), tr("已计算！"));return;}
ui->stackedWidget->setCurrentIndex(7);

ui->progressBar->setValue(0);
std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now(); //C++ std::chrono::high_resolution_clock

QList<int> yue;
for(const auto &v:vertices1)
{
    for (const auto &y:yueshuList)
    {
        if(v.x<=y.duandian1&&v.x>=y.duandian&&v.y==kuan_2/2&&y.b=="shang")
        {if(y.a=="guding") {yue.append(2*(v.id-1)+1);yue.append(2*(v.id-1));}
            if(y.a=="huadong")yue.append(2*(v.id-1)+1);}
        if(v.x<=y.duandian1&&v.x>=y.duandian&&v.y==-kuan_2/2&&y.b=="xia")
        {if(y.a=="guding") {yue.append(2*(v.id-1)+1);yue.append(2*(v.id-1));}
            if(y.a=="huadong")yue.append(2*(v.id-1)+1);}
        if(v.y>=-y.duandian1&&v.y<=-y.duandian&&v.x==-chang_2/2&&y.b=="zuo")
        {if(y.a=="guding") {yue.append(2*(v.id-1)+1);yue.append(2*(v.id-1));}
            if(y.a=="huadong")yue.append(2*(v.id-1));}
        if(v.y>=-y.duandian1&&v.y<=-y.duandian&&v.x==chang_2/2&&y.b=="you")
        {if(y.a=="guding") {yue.append(2*(v.id-1)+1);yue.append(2*(v.id-1));}
            if(y.a=="huadong")yue.append(2*(v.id-1));}
    }
}

int rows = 2*MNN;
int cols = NBW;

P= new double[2 * MNN];
SE = new double[3 * (MNE + 1)];

// 使用指针动态分配二维数组
double** GK = new double*[rows];
for (int i = 0; i < rows; ++i) {
    GK[i] = new double[cols];
}
double ST[MNN][3];
GSM(GK);
Load(P);
AGM(GK,NBW,yue,P);
Solver(GK,P,NBW,MNN);

for (int i = 0; i < rows; ++i) {
      delete[] GK[i]; //C++ delete[];delete 一指针
  }
  delete[] GK;
Stress(P,ST,SE);
ui->progressBar->setValue(100);

    // 记录结束时间
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    // 计算经过的时间
    std::chrono::duration<double> elapsed = end - start; //C++ std::chrono::duration(.count())
    QString elapsedTimeStr = QString::number(elapsed.count(), 'f', 1);

    ui->lcdNumber->display(elapsedTimeStr);

    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    ST1.resize(MNN, std::vector<double>(3)); //C++ vector.resize
    for (int i = 0; i < MNN; ++i) {
        for (int j = 0; j < 3; ++j) {
            ST1[i][j] = ST[i][j];
        }
    }

        jjj=1;
        ui->textBrowser->append("***点击网格顶点可查看相应应力位移***\n");
}

void MainWindow::on_jisuan_4_triggered()
{
if(tanxinmoliang_2==INFINITY||bosongbi_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义材料！"));return;}
if(ss==0){QMessageBox::warning(this, tr("错误"), tr("先生成网格！"));return;}
if(jjj==2||jjj==1) {QMessageBox::warning(this, tr("错误"), tr("已计算！"));return;}
ui->stackedWidget->setCurrentIndex(9);

ui->progressBar_2->setValue(0);
std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

QList<int> yue;
for(const auto &v:verticesgaojie1)
{
    for (const auto &y:yueshuList)
    {
        if(v.x<=y.duandian1&&v.x>=y.duandian&&v.y==kuan_2/2&&y.b=="shang")
        {if(y.a=="guding") {yue.append(2*(v.id-1)+1);yue.append(2*(v.id-1));}
            if(y.a=="huadong")yue.append(2*(v.id-1)+1);}
        if(v.x<=y.duandian1&&v.x>=y.duandian&&v.y==-kuan_2/2&&y.b=="xia")
        {if(y.a=="guding") {yue.append(2*(v.id-1)+1);yue.append(2*(v.id-1));}
            if(y.a=="huadong")yue.append(2*(v.id-1)+1);}
        if(v.y>=-y.duandian1&&v.y<=-y.duandian&&v.x==-chang_2/2&&y.b=="zuo")
        {if(y.a=="guding") {yue.append(2*(v.id-1)+1);yue.append(2*(v.id-1));}
            if(y.a=="huadong")yue.append(2*(v.id-1));}
        if(v.y>=-y.duandian1&&v.y<=-y.duandian&&v.x==chang_2/2&&y.b=="you")
        {if(y.a=="guding") {yue.append(2*(v.id-1)+1);yue.append(2*(v.id-1));}
            if(y.a=="huadong")yue.append(2*(v.id-1));}
    }
}

int rows = 2*MNN1;
int cols = NBW1;
P= new double[2 * MNN1];

SE = new double[3 * (MNE + 1)];

// 使用指针动态分配二维数组
double** GK = new double*[rows];
for (int i = 0; i < rows; ++i) {
    GK[i] = new double[rows];
}
double ST[MNN1][3];
GSMgaojie(GK);
Loadgaojie(P);
AGMgaojie(GK,yue,P);
Solvergaojie(GK,P);

for (int i = 0; i < rows; ++i) {
      delete[] GK[i];
  }
  delete[] GK;
Stressgaojie(P,ST,SE);

ui->progressBar_2->setValue(100);

    // 记录结束时间
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    // 计算经过的时间
    std::chrono::duration<double> elapsed = end - start;
    QString elapsedTimeStr = QString::number(elapsed.count(), 'f', 1);

    ui->lcdNumber_2->display(elapsedTimeStr);

    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    ST1.resize(MNN1, std::vector<double>(3));
    for (int i = 0; i < MNN1; ++i) {
        for (int j = 0; j < 3; ++j) {
            ST1[i][j] = ST[i][j];
        }
    }

        jjj=2;
        ui->textBrowser->append("***点击网格顶点可查看相应应力位移***\n");
}

void MainWindow::on_pushButton_6_clicked()
{
    if(bb==0)
    {
    for(size_t a = 0; a < vertices.size()/3; a++)
    {
        QPainterPath path;
        path.moveTo(origin.x()+vertices[3*a].x*40+P[2*(vertices[3*a].id-1)], origin.y()-vertices[3*a].y*40-P[2*(vertices[3*a].id-1)+1]);  //40
        path.lineTo(origin.x()+vertices[3*a+1].x*40+P[2*(vertices[3*a+1].id-1)], origin.y()-vertices[3*a+1].y*40-P[2*(vertices[3*a+1].id-1)+1]);
        path.lineTo(origin.x()+vertices[3*a+2].x*40+P[2*(vertices[3*a+2].id-1)], origin.y()-vertices[3*a+2].y*40-P[2*(vertices[3*a+2].id-1)+1]);
        path.lineTo(origin.x()+vertices[3*a].x*40+P[2*(vertices[3*a].id-1)], origin.y()-vertices[3*a].y*40-P[2*(vertices[3*a].id-1)+1]);

        QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
        scene->addItem(pathItem);
        pathi.append(pathItem);
    }
    bb=1;return;
    }

   if(bb==1)
    {
        for (QGraphicsPathItem* q: pathi) {
            scene->removeItem(q);
            delete q;
        }
        pathi.clear();
        bb=0;
    }
}

void MainWindow::on_pushButton_17_clicked()
{
    if(bb==0)
    {
    for(size_t a = 0; a < verticesgaojie.size()/6; a++)
    {
        QPainterPath path;
        path.moveTo(origin.x()+verticesgaojie[6*a].x*40+P[2*(verticesgaojie[6*a].id-1)], origin.y()-verticesgaojie[6*a].y*40-P[2*(verticesgaojie[6*a].id-1)+1]);  //40
        path.lineTo(origin.x()+verticesgaojie[6*a+1].x*40+P[2*(verticesgaojie[6*a+1].id-1)], origin.y()-verticesgaojie[6*a+1].y*40-P[2*(verticesgaojie[6*a+1].id-1)+1]);
        path.lineTo(origin.x()+verticesgaojie[6*a+2].x*40+P[2*(verticesgaojie[6*a+2].id-1)], origin.y()-verticesgaojie[6*a+2].y*40-P[2*(verticesgaojie[6*a+2].id-1)+1]);
        path.lineTo(origin.x()+verticesgaojie[6*a+3].x*40+P[2*(verticesgaojie[6*a+3].id-1)], origin.y()-verticesgaojie[6*a+3].y*40-P[2*(verticesgaojie[6*a+3].id-1)+1]);
        path.lineTo(origin.x()+verticesgaojie[6*a+4].x*40+P[2*(verticesgaojie[6*a+4].id-1)], origin.y()-verticesgaojie[6*a+4].y*40-P[2*(verticesgaojie[6*a+4].id-1)+1]);
        path.lineTo(origin.x()+verticesgaojie[6*a+5].x*40+P[2*(verticesgaojie[6*a+5].id-1)], origin.y()-verticesgaojie[6*a+5].y*40-P[2*(verticesgaojie[6*a+5].id-1)+1]);
        path.lineTo(origin.x()+verticesgaojie[6*a].x*40+P[2*(verticesgaojie[6*a].id-1)], origin.y()-verticesgaojie[6*a].y*40-P[2*(verticesgaojie[6*a].id-1)+1]);

        QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
        scene->addItem(pathItem);
        pathi.append(pathItem);
    }
    bb=1;return;
    }

   if(bb==1)
    {
        for (QGraphicsPathItem* q: pathi) {
            scene->removeItem(q);
            delete q;
        }
        pathi.clear();
        bb=0;
    }
}

std::vector<QColor> colorVector;

// 创建颜色图例
QList<QGraphicsItem*>  MainWindow:: createColorLegend(QGraphicsScene* scene, double minStress, double maxStress)
{
    QList<QGraphicsItem*> legendItems;
    double legendWidth = 18.1;
    int legendHeight = 40*kuan_2;
    qreal leftMargin = origin.x()+(legendWidth/2+1)*40;
    qreal topMargin = origin.y()-legendHeight/2;


    QLinearGradient gradient(0, 0, 0, legendHeight);

    // 设置颜色块
    for (int i = 0; i <20; ++i) {
        double positionStart = i / 20.0;
        double positionEnd = (i + 1) / 20.0;
        int hue=static_cast<int> (280-280*pow(1-positionStart,7)); //C++ static_cast<>
        gradient.setColorAt(positionStart, QColor::fromHsv(hue, 255, 255));
        gradient.setColorAt(positionEnd, QColor::fromHsv(hue, 255, 255));
        colorVector.push_back(QColor::fromHsv(280*positionStart, 255, 255));
    }

    // 创建渐变矩形
    QGraphicsRectItem *gradientRect = new QGraphicsRectItem(leftMargin, topMargin, legendWidth, legendHeight);
    gradientRect->setBrush(QBrush(gradient));
    scene->addItem(gradientRect);
    legendItems.append(gradientRect);

    // 添加标签 
    for (int i = 0; i <= 20; ++i) {
        double value = minStress + (maxStress - minStress)*pow(i/20.0,4.25);
        QGraphicsTextItem *label = new QGraphicsTextItem(QString::number(value));
        qreal y = topMargin + legendHeight - i * (legendHeight / 20.0)- legendHeight/50;
        label->setPos(leftMargin + legendWidth + 5, y);
        scene->addItem(label);
        legendItems.append(label);
    }


   return legendItems;
}

double sigmaxmax=0,sigmaymax=0,taumax=0,sigmaxmin=0,sigmaymin=0,taumin=0;int a[20]={0},hue[20]={0};

QColor stressToColor(double stress, double minStress, double maxStress)
{

    for(int i=0;i<20;i++)
    {
        double m=minStress+(maxStress - minStress)*pow(i/20.0,4.25);
        if(stress>=m&&stress<=minStress+(maxStress - minStress)*pow((i+1)/20.0,4.25))
        {a[i]++;hue[i]=colorVector[colorVector.size()-i-1].hue();return colorVector[colorVector.size()-i-1];}
    }

}

void MainWindow::on_keshihua_2_triggered()
{
if(jjj==0){QMessageBox::warning(this, tr("错误"), tr("先计算！"));return;}

for (QGraphicsPathItem* q: pathi) {
    scene->removeItem(q);
    delete q;
}
pathi.clear();

ui->stackedWidget->setCurrentIndex(8);
// 为每个三角形上色并添加到场景中

sigmaxmin=fabs(SE[0]),sigmaymin=fabs(SE[1]),taumin=fabs(SE[2]);

for(int i=0;i<3*(MNE+1);i+=3)
{
    if(fabs(SE[i])>sigmaxmax) sigmaxmax=fabs(SE[i]);
    if(fabs(SE[i])<sigmaxmin) sigmaxmin=fabs(SE[i]);
    if(fabs(SE[i+1])>sigmaymax) sigmaymax=fabs(SE[i+1]);
    if(fabs(SE[i+1])<sigmaymin) sigmaymin=fabs(SE[i+1]);
    if(fabs(SE[i+2])>taumax) taumax=fabs(SE[i+2]);
    if(fabs(SE[i+2])<taumin) taumin=fabs(SE[i+2]);
}

}

int xx=0,yy=0,zz=0;

QList<QGraphicsPathItem *> pp;QList<QGraphicsItem*> legendItems;

void MainWindow::on_pushButton_13_clicked()
{
for (QGraphicsPathItem* p: pp) {
    scene->removeItem(p);
    delete p;
}
pp.clear();
for (QGraphicsItem* item : legendItems) {
    scene->removeItem(item);
    delete item;
}
legendItems.clear();

if(jjj==1)
{
    if (ui->radioButton->isChecked())
    {
        if(xx==0)
        {
            legendItems = createColorLegend(scene, sigmaxmin, sigmaxmax);

            for(size_t a = 0; a < vertices.size()/3; a++)
            {
                QPainterPath path;
                path.moveTo(origin.x()+vertices[3*a].x*40, origin.y()-vertices[3*a].y*40);       //40
                path.lineTo(origin.x()+vertices[3*a+1].x*40, origin.y()-vertices[3*a+1].y*40);
                path.lineTo(origin.x()+vertices[3*a+2].x*40, origin.y()-vertices[3*a+2].y*40);
                path.lineTo(origin.x()+vertices[3*a].x*40, origin.y()-vertices[3*a].y*40);

                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                QColor color = stressToColor(fabs(SE[3*a]), sigmaxmin, sigmaxmax);
                pathItem->setBrush(QBrush(color));
                scene->addItem(pathItem);
                pp.append(pathItem);
            }
            for(int i=0;i<20;i++)qDebug()<<"h"<<a[i]<<"hue"<<hue[i];//应力大多小
            xx=1;yy=0;zz=0;}
    }

    if (ui->radioButton_2->isChecked())
    {
        if(yy==0)
        {
            legendItems = createColorLegend(scene, sigmaymin, sigmaymax);

            for(size_t a = 0; a < vertices.size()/3; a++)
            {
                QPainterPath path;
                path.moveTo(origin.x()+vertices[3*a].x*40, origin.y()-vertices[3*a].y*40);
                path.lineTo(origin.x()+vertices[3*a+1].x*40, origin.y()-vertices[3*a+1].y*40);
                path.lineTo(origin.x()+vertices[3*a+2].x*40, origin.y()-vertices[3*a+2].y*40);
                path.lineTo(origin.x()+vertices[3*a].x*40, origin.y()-vertices[3*a].y*40);

                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                QColor color = stressToColor(fabs(SE[3*a+1]), sigmaymin, sigmaymax);
                pathItem->setBrush(QBrush(color));
                scene->addItem(pathItem);
                pp.append(pathItem);
            }

            yy=1;xx=0;zz=0;}
    }

    if (ui->radioButton_3->isChecked())
    {
        if(zz==0)
        {
            legendItems = createColorLegend(scene, taumin, taumax);

            for(size_t a = 0; a < vertices.size()/3; a++)
            {
                QPainterPath path;
                path.moveTo(origin.x()+vertices[3*a].x*40, origin.y()-vertices[3*a].y*40);
                path.lineTo(origin.x()+vertices[3*a+1].x*40, origin.y()-vertices[3*a+1].y*40);
                path.lineTo(origin.x()+vertices[3*a+2].x*40, origin.y()-vertices[3*a+2].y*40);
                path.lineTo(origin.x()+vertices[3*a].x*40, origin.y()-vertices[3*a].y*40);

                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                QColor color = stressToColor(fabs(SE[3*a+2]), taumin, taumax);
                pathItem->setBrush(QBrush(color));
                scene->addItem(pathItem);
                pp.append(pathItem);
            }

            zz=1;xx=0;yy=0;}
    }
}

if(jjj==2)
{
    if (ui->radioButton->isChecked())
    {
        if(xx==0)
        {
            legendItems = createColorLegend(scene, sigmaxmin, sigmaxmax);

            for(size_t a = 0; a < verticesgaojie.size()/6; a++)
            {
                QPainterPath path;
                path.moveTo(origin.x()+verticesgaojie[6*a].x*40, origin.y()-verticesgaojie[6*a].y*40);       //40
                path.lineTo(origin.x()+verticesgaojie[6*a+2].x*40, origin.y()-verticesgaojie[6*a+2].y*40);
                path.lineTo(origin.x()+verticesgaojie[6*a+4].x*40, origin.y()-verticesgaojie[6*a+4].y*40);
                path.lineTo(origin.x()+verticesgaojie[6*a].x*40, origin.y()-verticesgaojie[6*a].y*40);

                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                QColor color = stressToColor(fabs(SE[3*a]), sigmaxmin, sigmaxmax);
                pathItem->setBrush(QBrush(color));
                scene->addItem(pathItem);
                pp.append(pathItem);
            }
            for(int i=0;i<20;i++)qDebug()<<"h"<<a[i]<<"hue"<<hue[i];//应力大多小
            xx=1;yy=0;zz=0;}
    }

    if (ui->radioButton_2->isChecked())
    {
        if(yy==0)
        {
            legendItems = createColorLegend(scene, sigmaymin, sigmaymax);

            for(size_t a = 0; a < verticesgaojie.size()/6; a++)
            {
                QPainterPath path;
                path.moveTo(origin.x()+verticesgaojie[6*a].x*40, origin.y()-verticesgaojie[6*a].y*40);
                path.lineTo(origin.x()+verticesgaojie[6*a+2].x*40, origin.y()-verticesgaojie[6*a+2].y*40);
                path.lineTo(origin.x()+verticesgaojie[6*a+4].x*40, origin.y()-verticesgaojie[6*a+4].y*40);
                path.lineTo(origin.x()+verticesgaojie[6*a].x*40, origin.y()-verticesgaojie[6*a].y*40);

                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                QColor color = stressToColor(fabs(SE[3*a+1]), sigmaymin, sigmaymax);
                pathItem->setBrush(QBrush(color));
                scene->addItem(pathItem);
                pp.append(pathItem);
            }

            yy=1;xx=0;zz=0;}
    }

    if (ui->radioButton_3->isChecked())
    {
        if(zz==0)
        {
            legendItems = createColorLegend(scene, taumin, taumax);

            for(size_t a = 0; a < verticesgaojie.size()/6; a++)
            {
                QPainterPath path;
                path.moveTo(origin.x()+verticesgaojie[6*a].x*40, origin.y()-verticesgaojie[6*a].y*40);
                path.lineTo(origin.x()+verticesgaojie[6*a+2].x*40, origin.y()-verticesgaojie[6*a+2].y*40);
                path.lineTo(origin.x()+verticesgaojie[6*a+4].x*40, origin.y()-verticesgaojie[6*a+4].y*40);
                path.lineTo(origin.x()+verticesgaojie[6*a].x*40, origin.y()-verticesgaojie[6*a].y*40);

                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                QColor color = stressToColor(fabs(SE[3*a+2]), taumin, taumax);
                pathItem->setBrush(QBrush(color));
                scene->addItem(pathItem);
                pp.append(pathItem);
            }

            zz=1;xx=0;yy=0;}
    }
}

}


void MainWindow::on_pushButton_clicked()
{
    bool ok1=false,ok2=false;
    double value = ui->chang_2->text().toDouble(&ok1);
    double value1 = ui->kuan_2->text().toDouble(&ok2);
    if (ok1&&ok2 && value > 0&&value1>0) {
        // 将值存储到变量中
        chang_2 = value;
        kuan_2 = value1;
        QString myString = "长、宽分别为%1、%2";
        QString formattedString = myString.arg(chang_2).arg(kuan_2);
        ui->textBrowser->append(formattedString);

        qreal rectWidth = chang_2*40;    /*40!!!*/
        qreal rectHeight = kuan_2*40;

        // 计算矩形左上角的坐标，使其中心与origin相同
        qreal topLeftX = origin.x() - rectWidth / 2;
        qreal topLeftY = origin.y() - rectHeight / 2;

        // 创建一个矩形项，将其位置设置为与origin相同
        CustomRectItem *rectItem = new CustomRectItem(topLeftX, topLeftY, rectWidth, rectHeight);

        // 将矩形项添加到场景中
        scene->addItem(rectItem);
    }else if(ok1==false||ok2==false)
    {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
    else{
        QMessageBox::warning(this, tr("错误"), tr("输入无效，请输入大于0的数字。"));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false,ok3=false;
    double value = ui->x_2->text().toDouble(&ok1);
    double value1 = ui->y_2->text().toDouble(&ok2);
    double value2 = ui->banjin->text().toDouble(&ok3);
    if (ok1&&ok2&&ok3 && value2>0) {
        // 将值存储到变量中
        x_2 = value;
        y_2 = value1;
        banjin=value2;

        if((origin.x()+x_2+banjin)>=(origin.x()+chang_2/2)||(origin.x()+x_2-banjin)<=(origin.x()-chang_2/2)||(origin.y()+y_2+banjin)>=(origin.y()+kuan_2/2)||(origin.y()+y_2-banjin)<=(origin.y()-kuan_2/2))
        {QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

        QString myString = "x、y、半径分别为%1、%2、%3";
        QString formattedString = myString.arg(x_2).arg(y_2).arg(banjin);
        ui->textBrowser->append(formattedString);


        qreal a = -banjin*40+origin.x()+x_2*40;    /*40!!!*/
        qreal b = -banjin*40+origin.y()+y_2*40;
        qreal c = banjin*40*2;

        yuankong s {x_2,y_2,banjin};
        yuankongList.append(s);

        CustomEllipseItem *ellipseItem = new CustomEllipseItem(a, b, c, c,s,this);

        scene->addItem(ellipseItem);
    }
    else if(ok1==false||ok2==false||ok3==false)
        {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
    else {
        QMessageBox::warning(this, tr("错误"), tr("输入无效，请输入大于0的半径。"));
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false,ok3=false,ok4=false;
    double value = ui->x_5->text().toDouble(&ok1);
    double value1 = ui->y_5->text().toDouble(&ok2);
    double value2 = ui->chang_3->text().toDouble(&ok3);
    double value3 = ui->kuan_3->text().toDouble(&ok4);
    if (ok1&&ok2&&ok3&&ok4 && value2>0&&value3>0) {
        // 将值存储到变量中
        x_5 = value;
        y_5 = value1;
        chang_3=value2;
        kuan_3=value3;

        if((origin.x()+x_5+chang_3/2)>=(origin.x()+chang_2/2)||(origin.x()+x_5-chang_3/2)<=(origin.x()-chang_2/2)||(origin.y()+y_5+kuan_3/2)>=(origin.y()+kuan_2/2)||(origin.y()+y_5-kuan_3/2)<=(origin.y()-kuan_2/2))
        {QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

        QString myString = "x、y、长、宽分别为%1、%2、%3、%4";
        QString formattedString = myString.arg(x_5).arg(y_5).arg(chang_3).arg(kuan_3);
        ui->textBrowser->append(formattedString);

        qreal a = -chang_3*40/2+origin.x()+x_5*40;    /*40!!!*/
        qreal b = -kuan_3*40/2+origin.y()+y_5*40;

        juxinkong s {x_5,y_5,chang_3,kuan_3};
        juxinkongList.append(s);

        CustomRectItem_1 *rectItem1 = new CustomRectItem_1(a, b, chang_3 * 40, kuan_3 * 40, s, this);

        scene->addItem(rectItem1);
    }
    else if(ok1==false||ok2==false||ok3==false||ok4==false)
        {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
    else {
        QMessageBox::warning(this, tr("错误"), tr("输入无效，请输入大于0的数字。"));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    bool ok1=false,ok2=false;
    double value = ui->bosongbi_2->text().toDouble(&ok1);
    double value1 = ui->tanxinmoliang_2->text().toDouble(&ok2);
    if (ok1&&ok2 && value <= 0.5&&value>0&&value1>0) {
        // 将值存储到变量中
        bosongbi_2 = value;
        tanxinmoliang_2 = value1;
        QString myString = "泊松比、弹性模量分别为%1、%2";
        QString formattedString = myString.arg(bosongbi_2).arg(tanxinmoliang_2);
        ui->textBrowser->append(formattedString);
    }
    else if(ok1==false||ok2==false)
        {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
    else {
        QMessageBox::warning(this, tr("错误"), tr("输入无效，弹性模量大于0，泊松比位于0~0.5。"));
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false,ok3=false;
    double value = ui->zuoduandian_2->text().toDouble(&ok1);
    double value1 = ui->youduandian_2->text().toDouble(&ok2);
    double value2 = ui->li_2->text().toDouble(&ok3);
    QString selectedText = ui->comboBox_5->currentText();
        qDebug() << "Selected text:" << selectedText;
    if (ok1&&ok2&&ok3&&value2!=0&&value<=value1 ) {
        if(selectedText == QStringLiteral("上端面")){
        qDebug() << "The QString variable is 上.";
        zuoduandian_2 = value;
        youduandian_2 = value1;
        li_2=value2;

        if(zuoduandian_2<-(chang_2/2)||youduandian_2>(chang_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

        QString myString = "左端点、右端点、力分别为%1、%2、%3";
        QString formattedString = myString.arg(zuoduandian_2).arg(youduandian_2).arg(li_2);
        ui->textBrowser->append(formattedString);

        shouli s {"junbuli","shang",zuoduandian_2,youduandian_2,li_2};
        shouliList.append(s);
        Point p1{zuoduandian_2,kuan_2/2,"li"};userPoints.append(p1);Point p2{youduandian_2,kuan_2/2,"li"};userPoints.append(p2);

        createArrows(zuoduandian_2,youduandian_2,li_2,kuan_2,0);}
        else if(selectedText == QStringLiteral("下端面")){
            qDebug() << "The QString variable is 下.";
            zuoduandian_2_1 = value;
            youduandian_2_1 = value1;
            li_2_1=value2;

            if(zuoduandian_2_1<-(chang_2/2)||youduandian_2_1>(chang_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

            QString myString = "左端点、右端点、力分别为%1、%2、%3";
            QString formattedString = myString.arg(zuoduandian_2_1).arg(youduandian_2_1).arg(li_2_1);
            ui->textBrowser->append(formattedString);

            shouli s {"junbuli","xia",zuoduandian_2_1,youduandian_2_1,li_2_1};
            shouliList.append(s);
            Point p1{zuoduandian_2_1,-kuan_2/2,"li"};userPoints.append(p1);Point p2{youduandian_2_1,-kuan_2/2,"li"};userPoints.append(p2);

            createArrows(zuoduandian_2_1,youduandian_2_1,li_2_1,kuan_2,1);}
    }
    else if(ok1==false||ok2==false||ok3==false)
        {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
    else if(value2==0) {
        QMessageBox::warning(this, tr("错误"), tr("输入无效，力不能为0。"));
    }
    else{QMessageBox::warning(this, tr("错误"), tr("输入无效，左端点需小于等于右端点"));}
}

void MainWindow::on_pushButton_11_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false,ok3=false;
    double value = ui->shangduandian_3->text().toDouble(&ok1);
    double value1 = ui->xiaduandian_3->text().toDouble(&ok2);
    double value2 = ui->li_6->text().toDouble(&ok3);
    QString selectedText = ui->comboBox_6->currentText();
        qDebug() << "Selected text:" << selectedText;
    if (ok1&&ok2&&ok3&&value2!=0&&value<=value1 ) {
        if(selectedText == QStringLiteral("左端面")){
        qDebug() << "The QString variable is 左.";
        shangduandian_3 = value;
        xiaduandian_3 = value1;
        li_6=value2;

        if(shangduandian_3<-(kuan_2/2)||xiaduandian_3>(kuan_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

        QString myString = "上端点、下端点、力分别为%1、%2、%3";
        QString formattedString = myString.arg(shangduandian_3).arg(xiaduandian_3).arg(li_6);
        ui->textBrowser->append(formattedString);

        shouli s {"junbuli","zuo",shangduandian_3,xiaduandian_3,li_6};
        shouliList.append(s);
        Point p1{-chang_2/2,-shangduandian_3,"li"};userPoints.append(p1);Point p2{-chang_2/2,-xiaduandian_3,"li"};userPoints.append(p2);

        createArrows(shangduandian_3,xiaduandian_3,li_6,kuan_2,2);}
        else if(selectedText == QStringLiteral("右端面")){
            qDebug() << "The QString variable is 右.";
            shangduandian_3_1 = value;
            xiaduandian_3_1 = value1;
            li_6_1=value2;

            if(shangduandian_3_1<-(kuan_2/2)||xiaduandian_3_1>(kuan_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

            QString myString = "上端点、下端点、力分别为%1、%2、%3";
            QString formattedString = myString.arg(shangduandian_3_1).arg(xiaduandian_3_1).arg(li_6_1);
            ui->textBrowser->append(formattedString);

            shouli s {"junbuli","you",shangduandian_3_1,xiaduandian_3_1,li_6_1};
            shouliList.append(s);
            Point p1{chang_2/2,-shangduandian_3_1,"li"};userPoints.append(p1);Point p2{chang_2/2,-xiaduandian_3_1,"li"};userPoints.append(p2);

            createArrows(shangduandian_3_1,xiaduandian_3_1,li_6_1,kuan_2,3);}
    }
    else if(ok1==false||ok2==false||ok3==false)
        {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
    else if(value2==0) {
        QMessageBox::warning(this, tr("错误"), tr("输入无效，力不能为0。"));
    }
    else{QMessageBox::warning(this, tr("错误"), tr("输入无效，上端点需小于等于下端点"));}
}

void MainWindow::on_pushButton_5_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false;
    double value = ui->henzongzuobiao_2->text().toDouble(&ok1);
    double value1 = ui->li_4->text().toDouble(&ok2);
     QString selectedText = ui->comboBox_7->currentText();
    if (ok1&&ok2&&value1!=0) {
        if(selectedText == QStringLiteral("上端面"))
        {
        qDebug() << "The QString variable is 上.";
        henzongzuobiao_2 = value;
        li_4 = value1;

        if(henzongzuobiao_2<-(chang_2/2)||henzongzuobiao_2>(chang_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

        QString myString = "坐标、力分别为%1、%2";
        QString formattedString = myString.arg(henzongzuobiao_2).arg(li_4);
        ui->textBrowser->append(formattedString);

        qreal xPos = origin.x()+henzongzuobiao_2*40;
        qreal yPos = origin.y()-kuan_2*40/2;
        qreal length = fabs(li_4);
        int a=(li_4<0);

        shouli s {"jizhongli","shang",henzongzuobiao_2,0,li_4};
        shouliList.append(s);
        Point p1{henzongzuobiao_2,kuan_2/2,"li"};userPoints.append(p1);

        CustomArrowItem *arrowItem = new CustomArrowItem(xPos, yPos, length, a,0);
        connect(arrowItem, &CustomArrowItem::arrowClicked, this, &MainWindow::removeAllArrows);
        arrowsList.append(arrowItem);
        scene->addItem(arrowItem);}
        if(selectedText == QStringLiteral("下端面"))
        {qDebug() << "The QString variable is 下.";
        henzongzuobiao_2_1 = value;
        li_4_1 = value1;

        if(henzongzuobiao_2_1<-(chang_2/2)||henzongzuobiao_2_1>(chang_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

        QString myString = "坐标、力分别为%1、%2";
        QString formattedString = myString.arg(henzongzuobiao_2_1).arg(li_4_1);
        ui->textBrowser->append(formattedString);

        qreal xPos = origin.x()+henzongzuobiao_2_1*40;
        qreal yPos = origin.y()+kuan_2*40/2;
        qreal length = fabs(li_4_1);
        int a=(li_4_1<0);

        shouli s {"jizhongli","xia",henzongzuobiao_2_1,0,li_4_1};
        shouliList.append(s);
        Point p1{henzongzuobiao_2_1,-kuan_2/2,"li"};userPoints.append(p1);

        CustomArrowItem *arrowItem = new CustomArrowItem(xPos, yPos, length, a,1);
        connect(arrowItem, &CustomArrowItem::arrowClicked, this, &MainWindow::removeAllArrows);
        arrowsList.append(arrowItem);
        scene->addItem(arrowItem);}
        if(selectedText == QStringLiteral("左端面"))
        {qDebug() << "The QString variable is 左.";
        henzongzuobiao_2_2 = value;
        li_4_2 = value1;

        if(henzongzuobiao_2_2<-(kuan_2/2)||henzongzuobiao_2_2>(kuan_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}

        QString myString = "坐标、力分别为%1、%2";
        QString formattedString = myString.arg(henzongzuobiao_2_2).arg(li_4_2);
        ui->textBrowser->append(formattedString);

        qreal xPos = origin.x()-chang_2/2*40;
        qreal yPos = origin.y()+henzongzuobiao_2_2*40;
        qreal length = fabs(li_4_2);
        int a=(li_4_2<0);

        shouli s {"jizhongli","zuo",henzongzuobiao_2_2,0,li_4_2};
        shouliList.append(s);
        Point p1{-chang_2/2,-henzongzuobiao_2_2,"li"};userPoints.append(p1);

        CustomArrowItem *arrowItem = new CustomArrowItem(xPos, yPos, length, a,2);
        connect(arrowItem, &CustomArrowItem::arrowClicked, this, &MainWindow::removeAllArrows);
        arrowsList.append(arrowItem);
        scene->addItem(arrowItem);}
        if(selectedText == QStringLiteral("右端面"))
        {qDebug() << "The QString variable is 右.";
        henzongzuobiao_2_3 = value;
        li_4_3 = value1;

        if(henzongzuobiao_2_3<-(kuan_2/2)||henzongzuobiao_2_3>(kuan_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
        QString myString = "坐标、力分别为%1、%2";
        QString formattedString = myString.arg(henzongzuobiao_2_3).arg(li_4_3);
        ui->textBrowser->append(formattedString);

        qreal xPos = origin.x()+chang_2/2*40;
        qreal yPos = origin.y()+henzongzuobiao_2_3*40;
        qreal length = fabs(li_4_3);
        int a=(li_4_3<0);

        shouli s {"jizhongli","you",henzongzuobiao_2_3,0,li_4_3};
        shouliList.append(s);
        Point p1{chang_2/2,-henzongzuobiao_2_3,"li"};userPoints.append(p1);

        CustomArrowItem *arrowItem = new CustomArrowItem(xPos, yPos, length, a,3);
        connect(arrowItem, &CustomArrowItem::arrowClicked, this, &MainWindow::removeAllArrows);
        arrowsList.append(arrowItem);
        scene->addItem(arrowItem);}
    }
    else if(ok1==false||ok2==false)
        {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
    else {
        QMessageBox::warning(this, tr("错误"), tr("输入无效，力不能为0。"));
    }
}


void MainWindow::on_pushButton_7_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false;
    double value = ui->shangduandian->text().toDouble(&ok1);
    double value1 = ui->xiaduandian->text().toDouble(&ok2);
    QString selectedText = ui->comboBox->currentText();
        qDebug() << "Selected text:" << selectedText;
        if(ok1&&ok2&&value<=value1)
        {if (selectedText == QStringLiteral("左端面")) {
            qDebug() << "The QString variable is equal.";
            shangduandian = value;
            xiaduandian = value1;

            if(shangduandian<-(kuan_2/2)||xiaduandian>(kuan_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
            QString myString = "上端点、下端点分别为%1、%2";
            QString formattedString = myString.arg(shangduandian).arg(xiaduandian);
            ui->textBrowser->append(formattedString);

            yueshu s {"huadong","zuo",shangduandian,xiaduandian};
            yueshuList.append(s);
            Point p1{-chang_2/2,-shangduandian,"yueshu"};userPoints.append(p1);Point p2{-chang_2/2,-xiaduandian,"yueshu"};userPoints.append(p2);

            createHinges(shangduandian,xiaduandian,chang_2,0);
        }else{shangduandian_1 = value;
                xiaduandian_1 = value1;

                if(shangduandian_1<-(kuan_2/2)||xiaduandian_1>(kuan_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
                QString myString = "上端点、下端点分别为%1、%2";
                QString formattedString = myString.arg(shangduandian_1).arg(xiaduandian_1);
                ui->textBrowser->append(formattedString);

                yueshu s {"huadong","you",shangduandian_1,xiaduandian_1};
                yueshuList.append(s);
                Point p1{chang_2/2,-shangduandian_1,"yueshu"};userPoints.append(p1);Point p2{chang_2/2,-xiaduandian_1,"yueshu"};userPoints.append(p2);

                createHinges(shangduandian_1,xiaduandian_1,chang_2,1);}}
        else if(ok1==false||ok2==false)
            {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
            else{QMessageBox::warning(this, tr("错误"), tr("输入无效，上端点需小于等于下端点。"));}

}

void MainWindow::on_pushButton_8_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false;
    double value = ui->zuoduandian_3->text().toDouble(&ok1);
    double value1 = ui->youduandian_3->text().toDouble(&ok2);
    QString selectedText1 =ui->comboBox_2->currentText();
        qDebug() << "Selected text:" << selectedText1;
        if(ok1&&ok2&&value<=value1)
        {if (selectedText1 == QStringLiteral("上端面")) {
            qDebug() << "The QString variable is equal.";
            zuoduandian_3 = value;
            youduandian_3 = value1;

            if(zuoduandian_3<-(chang_2/2)||youduandian_3>(chang_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
            QString myString = "左端点、右端点分别为%1、%2";
            QString formattedString = myString.arg(zuoduandian_3).arg(youduandian_3);
            ui->textBrowser->append(formattedString);

            yueshu s {"huadong","shang",zuoduandian_3,youduandian_3};
            yueshuList.append(s);
            Point p1{zuoduandian_3,kuan_2/2,"yueshu"};userPoints.append(p1);Point p2{youduandian_3,kuan_2/2,"yueshu"};userPoints.append(p2);

            createHinges(zuoduandian_3,youduandian_3,kuan_2,2);
        }else{zuoduandian_3_1 = value;
                youduandian_3_1 = value1;

                if(zuoduandian_3_1<-(chang_2/2)||youduandian_3_1>(chang_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
                QString myString = "左端点、右端点分别为%1、%2";
                QString formattedString = myString.arg(zuoduandian_3_1).arg(youduandian_3_1);
                ui->textBrowser->append(formattedString);

                yueshu s {"huadong","xia",zuoduandian_3_1,youduandian_3_1};
                yueshuList.append(s);
                Point p1{zuoduandian_3_1,-kuan_2/2,"yueshu"};userPoints.append(p1);Point p2{youduandian_3_1,-kuan_2/2,"yueshu"};userPoints.append(p2);

                createHinges(zuoduandian_3_1,youduandian_3_1,kuan_2,3);}
        }
        else if(ok1==false||ok2==false)
            {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
        else{QMessageBox::warning(this, tr("错误"), tr("输入无效，左端点需小于等于右端点。"));}

}

void MainWindow::on_pushButton_9_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false;
    double value = ui->shangduandian_2->text().toDouble(&ok1);
    double value1 = ui->xiaduandian_2->text().toDouble(&ok2);
    QString selectedText = ui->comboBox_3->currentText();
        qDebug() << "Selected text:" << selectedText;
        if(ok1&&ok2&&value<=value1)
        {if (selectedText == QStringLiteral("左端面")) {
            qDebug() << "The QString variable is equal.";
            shangduandian_2 = value;
            xiaduandian_2 = value1;

            if(shangduandian_2<-(kuan_2/2)||xiaduandian_2>(kuan_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
            QString myString = "上端点、下端点分别为%1、%2";
            QString formattedString = myString.arg(shangduandian_2).arg(xiaduandian_2);
            ui->textBrowser->append(formattedString);

            yueshu s {"guding","zuo",shangduandian_2,xiaduandian_2};
            yueshuList.append(s);
            Point p1{-chang_2/2,-shangduandian_2,"yueshu"};userPoints.append(p1);Point p2{-chang_2/2,-xiaduandian_2,"yueshu"};userPoints.append(p2);

            createHinges(shangduandian_2,xiaduandian_2,chang_2,4);
        }else{shangduandian_2_1 = value;
                xiaduandian_2_1 = value1;

                if(shangduandian_2_1<-(kuan_2/2)||xiaduandian_2_1>(kuan_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
                QString myString = "上端点、下端点分别为%1、%2";
                QString formattedString = myString.arg(shangduandian_2_1).arg(xiaduandian_2_1);
                ui->textBrowser->append(formattedString);

                yueshu s {"guding","you",shangduandian_2_1,xiaduandian_2_1};
                yueshuList.append(s);
                Point p1{chang_2/2,-shangduandian_2_1,"yueshu"};userPoints.append(p1);Point p2{chang_2/2,-xiaduandian_2_1,"yueshu"};userPoints.append(p2);

                createHinges(shangduandian_2_1,xiaduandian_2_1,chang_2,5);}}
        else if(ok1==false||ok2==false)
            {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
            else{QMessageBox::warning(this, tr("错误"), tr("输入无效，上端点需小于等于下端点。"));}

}

void MainWindow::on_pushButton_10_clicked()
{
    if(chang_2==INFINITY||kuan_2==INFINITY){QMessageBox::warning(this, tr("错误"), tr("先定义矩形杆件！"));return;}
    bool ok1=false,ok2=false;
    double value = ui->zuoduandian_4->text().toDouble(&ok1);
    double value1 = ui->youduandian_4->text().toDouble(&ok2);
    QString selectedText1 =ui->comboBox_4->currentText();
        qDebug() << "Selected text:" << selectedText1;
        if(ok1&&ok2&&value<=value1)
        {if (selectedText1 == QStringLiteral("上端面")) {
            qDebug() << "The QString variable is equal.";
            zuoduandian_4 = value;
            youduandian_4 = value1;

            if(zuoduandian_4<-(chang_2/2)||youduandian_4>(chang_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
            QString myString = "左端点、右端点分别为%1、%2";
            QString formattedString = myString.arg(zuoduandian_4).arg(youduandian_4);
            ui->textBrowser->append(formattedString);

            yueshu s {"guding","shang",zuoduandian_4,youduandian_4};
            yueshuList.append(s);
            Point p1{zuoduandian_4,kuan_2/2,"yueshu"};userPoints.append(p1);Point p2{youduandian_4,kuan_2/2,"yueshu"};userPoints.append(p2);

            createHinges(zuoduandian_4,youduandian_4,kuan_2,6);
        }else{  zuoduandian_4_1 = value;
                youduandian_4_1 = value1;

                if(zuoduandian_4_1<-(chang_2/2)||youduandian_4_1>(chang_2/2)){QMessageBox::warning(this, tr("错误"), tr("越界！"));return;}
                QString myString = "左端点、右端点分别为%1、%2";
                QString formattedString = myString.arg(zuoduandian_4_1).arg(youduandian_4_1);
                ui->textBrowser->append(formattedString);

                yueshu s {"guding","xia",zuoduandian_4_1,youduandian_4_1};
                yueshuList.append(s);
                Point p1{zuoduandian_4_1,-kuan_2/2,"yueshu"};userPoints.append(p1);Point p2{youduandian_4_1,-kuan_2/2,"yueshu"};userPoints.append(p2);

                createHinges(zuoduandian_4_1,youduandian_4_1,kuan_2,7);}
        }
        else if(ok1==false||ok2==false)
            {QMessageBox::warning(this, tr("错误"), tr("输入!"));}
        else{QMessageBox::warning(this, tr("错误"), tr("输入无效，左端点需小于等于右端点。"));}

}




