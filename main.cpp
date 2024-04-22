#include "widget.h"
#include "loginwindow.h" // 包含登录窗口头文件
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 创建登录窗口并显示
    LoginWindow loginWindow;
    loginWindow.show();
    return a.exec();
}
