#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <cmath>
#include <QButtonGroup>

#include "customarrowitem.h"
#include "hinge.h"

typedef struct {
    double x_2,y_2,banjin;
} yuankong;

typedef struct {
    double x_5,y_5,chang_3,kuan_3;
} juxinkong;

typedef struct {
    QString a;   //"jizhongli","junbuli"
    QString b;   //"shang","xia","zuo","you"
    double duandian,duandian1,li;
} shouli;

typedef struct {
    QString a;   //"guding","huadong"
    QString b;   //"shang","xia","zuo","you"
    double duandian,duandian1;
} yueshu;

typedef struct {
    double x;
    double y;
    QString a;  //"li","yueshu","jian"
} Point;

struct Vertex {                     //网格三角形顶点
    int id;                         //编号
    double x;
    double y;
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } //C++命名空间
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<yuankong> yuankongList;
    QList<juxinkong> juxinkongList;
    QList<shouli> shouliList;
    QList<yueshu> yueshuList;
    QList<Point> userPoints;     //以gmsh的坐标
    std::vector<Vertex> vertices,vertices1, verticesgaojie,verticesgaojie1; //C++vector
    std::vector<std::vector<double>> ST1;
    int jjj=0;double* P;
    void appendToTextBrowser(const QString &text);

private slots:
    void on_jihe_triggered();
    void on_cailiao_triggered();
    void on_shouli_triggered();
    void on_huadong_triggered();
    void on_guding_triggered();
    void on_shencheng_triggered();
    void on_jisuan_2_triggered();
    void on_keshihua_2_triggered();
    void on_jisuan_4_triggered();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_17_clicked();


    void createArrows(double duandian,double duandian1,double li_2,double kuan_2,int b);
    void createHinges(double x,double y,double a,int b);
    void ESM(int NE,double EK[][6],double S[][6]);
    void ESMgaojie(int NE,double EK[][12]);
    void Load(double P[]);void Loadgaojie(double P[]);
    void GSM(double** GK);void GSMgaojie(double** GK);
    void Solver(double** GK,double P[],int NBW,int MNN);void Solvergaojie(double** GK,double P[]);
    void Stress(double P[],double ST[][3],double SE[]);void Stressgaojie(double P[],double ST[][3],double SE[]);
    QList<QGraphicsItem*> createColorLegend(QGraphicsScene* scene, double minStress, double maxStress);

   public slots:
     void removeAllArrows();
     void removeHinges();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QList<CustomArrowItem *> arrowsList;
    QList<hinge *> hingeList;
    QButtonGroup *buttonGroup;

    double chang_2=INFINITY,kuan_2=INFINITY, //C++ <cmath> INFINITY
    x_2,y_2,banjin,
    x_5,y_5,chang_3,kuan_3,
    bosongbi_2=INFINITY,tanxinmoliang_2=INFINITY,
    zuoduandian_2,youduandian_2,li_2,zuoduandian_2_1,youduandian_2_1,li_2_1,
    shangduandian_3,xiaduandian_3,li_6,shangduandian_3_1,xiaduandian_3_1,li_6_1,
    henzongzuobiao_2,li_4,henzongzuobiao_2_1,li_4_1,henzongzuobiao_2_2,li_4_2,henzongzuobiao_2_3,li_4_3,
    shangduandian,shangduandian_1,shangduandian_2,shangduandian_2_1,xiaduandian,xiaduandian_1,xiaduandian_2,xiaduandian_2_1,
    zuoduandian_3,youduandian_3,zuoduandian_3_1,youduandian_3_1,zuoduandian_4,youduandian_4,zuoduandian_4_1,youduandian_4_1,
    lineEdit,lineEdit_2,lineEdit_3,lineEdit_4;

};
#endif // MAINWINDOW_H
