#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "customerloginwindow.h"
#include "merchantloginwindow.h"


LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connectsql();

    //setFixedSize(600,400);
}


void LoginWindow::on_customerButton_clicked()
{
    // 创建顾客登录界面并显示
    CustomerLoginWindow *customerLoginWindow = new CustomerLoginWindow();
    customerLoginWindow->show();
    this->close(); // 关闭当前登录界面
}
void LoginWindow::connectsql()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("Mixue_64");
    db.setUserName("root");
    db.setPassword("abc000000");
    db.open();
    bool ok=db.open();
    if(ok)
    {
        qDebug()<<"数据库连接成功！";
    }
    else
    {
       QMessageBox::information(this,"infor", "link failed");
       qDebug()<<"error open database because"<<db.lastError().text();
    }
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
