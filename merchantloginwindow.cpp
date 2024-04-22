#include "merchantloginwindow.h"
#include "ui_merchantloginwindow.h"
#include <QMessageBox> // 弹出消息框的头文件

MerchantLoginWindow::MerchantLoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MerchantLoginWindow)
{
    ui->setupUi(this);
    // 连接退出按钮的点击事件到槽函数
    connect(ui->exitButton, &QPushButton::clicked, this, &MerchantLoginWindow::on_exitButton_clicked);
}

void MerchantLoginWindow::on_loginButton_clicked()
{
    // 获取商家用户名和密码
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // 假设用户名和密码验证逻辑为简单的匹配
    if (username == "merchant" && password == "123456") {
        // 登录成功，弹出提示框
        QMessageBox::information(this, "登录成功", "欢迎商家登录！");
    } else {
        // 登录失败，弹出提示框
        QMessageBox::warning(this, "登录失败", "商家用户名或密码错误！");
    }
}

void MerchantLoginWindow::on_exitButton_clicked()
{
    // 点击退出按钮时，关闭窗口
    close();
}


MerchantLoginWindow::~MerchantLoginWindow()
{
    delete ui;
}
