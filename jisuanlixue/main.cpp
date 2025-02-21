#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // 设置QMenuBar和QMenu样式
       QString styleSheet = R"(
           QMenuBar, QMenu {
               font-size: 16px; /* 调整字体大小 */
           }
       )";
       a.setStyleSheet(styleSheet);

    w.show();
    return a.exec();
}
