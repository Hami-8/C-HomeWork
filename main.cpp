#include "widget.h"
//#include "loginwindow.h" // 包含登录窗口头文件
#include <QApplication>
#include "customerloginwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建登录窗口并显示
    CustomerLoginWindow customerloginWindow;
    customerloginWindow.show();
    //LoginWindow loginWindow;
    //loginWindow.show();
    return a.exec();
}
