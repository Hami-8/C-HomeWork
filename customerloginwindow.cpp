#include "customerloginwindow.h"
#include "ui_customerloginwindow.h"
#include <QMessageBox>
#include "customerwidget.h"

CustomerLoginWindow::CustomerLoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerLoginWindow)
{
    ui->setupUi(this);
    connectsql();
    // 连接退出按钮的点击事件到槽函数
    connect(ui->exitButton, &QPushButton::clicked, this, &CustomerLoginWindow::on_exitButton_clicked);
    //将登录成功信号连接至顾客点餐界面
    connect(this, &CustomerLoginWindow::loginSucceeded, this,&CustomerLoginWindow::showCustomerWidget);

//    //美化界面
//    QString buttonStyleSheet = "QPushButton {"
//                               "background-color: #4CAF50;"
//                               "border-style: outset;"
//                               "border-width: 2px;"
//                               "border-radius: 10px;"
//                               "border-color: beige;"
//                               "font: bold 14px;"
//                               "min-width: 10em;"
//                               "padding: 6px;"
//                               "}"
//                               "QPushButton:hover {"
//                               "background-color: #45a049;"
//                               "}";
//    this->setStyleSheet(buttonStyleSheet);
}

void CustomerLoginWindow::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CustomerLoginWindow::connectsql()
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

void CustomerLoginWindow::on_loginButton_clicked()
{
    // 获取顾客用户名和密码
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    //数据库中查找用户名和密码
    QSqlQuery query(db);
    query.exec(QString("select * from Customer where username='%1' and password='%2'").arg(ui->usernameLineEdit->text()).arg(ui->passwordLineEdit->text()));  //建立查询
     if(query.next())
       {
        id=query.value(0).toString().toInt();
        qDebug() <<id;
        QMessageBox::information(this, "登录成功", "欢迎顾客登录！");
        emit loginSucceeded();
       }
    else {
        QMessageBox::information(this,"infor", "登录失败，用户名或密码错误！");
    }
    // 假设用户名和密码验证逻辑为简单的匹配
    //if (username == "customer" && password == "123456") {
        // 登录成功，弹出提示框
//        QMessageBox::information(this, "登录成功", "欢迎顾客登录！");
//        emit loginSucceeded();
    //} else {
        // 登录失败，弹出提示框
        //QMessageBox::warning(this, "登录失败", "顾客用户名或密码错误！");
    //}
}
void CustomerLoginWindow::on_exitButton_clicked()
{
    // 点击退出按钮时，关闭窗口
    close();
}
void CustomerLoginWindow::showCustomerWidget()
{
    CustomerWidget *customerWidget =new CustomerWidget(id);
    //customerWidget->id = id; //把顾客信息传过去
    customerWidget->show();
    this->close();
}

CustomerLoginWindow::~CustomerLoginWindow()
{
    delete ui;
}
