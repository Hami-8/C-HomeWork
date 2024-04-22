#include "customerloginwindow.h"
#include "ui_customerloginwindow.h"
#include <QMessageBox>

CustomerLoginWindow::CustomerLoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerLoginWindow)
{
    ui->setupUi(this);
    // 连接退出按钮的点击事件到槽函数
    connect(ui->exitButton, &QPushButton::clicked, this, &CustomerLoginWindow::on_exitButton_clicked);
}

void CustomerLoginWindow::on_loginButton_clicked()
{
    // 获取顾客用户名和密码
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // 假设用户名和密码验证逻辑为简单的匹配
    if (username == "customer" && password == "123456") {
        // 登录成功，弹出提示框
        QMessageBox::information(this, "登录成功", "欢迎顾客登录！");
    } else {
        // 登录失败，弹出提示框
        QMessageBox::warning(this, "登录失败", "顾客用户名或密码错误！");
    }
}
void CustomerLoginWindow::on_exitButton_clicked()
{
    // 点击退出按钮时，关闭窗口
    close();
}

CustomerLoginWindow::~CustomerLoginWindow()
{
    delete ui;
}
