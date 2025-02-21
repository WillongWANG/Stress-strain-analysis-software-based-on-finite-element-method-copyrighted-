#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "mainwindow.h"

class MyGraphicsScene : public QGraphicsScene
{
public:
    MyGraphicsScene(MainWindow *mainWindow, QObject *parent = nullptr)
        : QGraphicsScene(parent), mainWindow(mainWindow) {} //C++官网 Member initialization in constructors,What is inherited from the base class

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    MainWindow *mainWindow;
};

#endif // MYGRAPHICSSCENE_H
