#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "customerloginwindow.h"
#include "merchantloginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    //setFixedSize(600,400);
}

void LoginWindow::on_customerButton_clicked()
{
    // 创建顾客登录界面并显示
    CustomerLoginWindow *customerLoginWindow = new CustomerLoginWindow();
    customerLoginWindow->show();
    this->close(); // 关闭当前登录界面
}

void LoginWindow::on_merchantButton_clicked()
{
    // 创建商家登录界面并显示
    MerchantLoginWindow *merchantLoginWindow = new MerchantLoginWindow();
    merchantLoginWindow->show();
    this->close(); // 关闭当前登录界面
}


LoginWindow::~LoginWindow()
{
    delete ui;
}
