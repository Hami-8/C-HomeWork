#include "customer_change_passwd.h"
#include "ui_customer_change_passwd.h"

customer_change_passwd::customer_change_passwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::customer_change_passwd)
{
    ui->setupUi(this);
}

void customer_change_passwd::connectsql()
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

customer_change_passwd::~customer_change_passwd()
{
    delete ui;
}

void customer_change_passwd::on_confirmButton_clicked()
{
    connectsql();
    QSqlQuery query(db);
    query.exec(QString("select * from customer where customer_id= %1 and password='%2'").arg(id).arg(ui->currentPasswdLineEdit->text()));
     if(query.next())
       {
             if((ui->newPasswdLineEdit->text()==ui->confirmLineEdit->text())&&(ui->newPasswdLineEdit->text()!=NULL))
             {
                 password=ui->newPasswdLineEdit->text();
                 //在数据库中修改密码
                 QSqlQuery query1(db);
                 query1.exec(QString("update customer set password='%1' where customer_id = %2").arg(password).arg(id));
                 QMessageBox::information(this,"infor", "密码修改成功！");
             }
             else {
                  QMessageBox::information(this,"infor", "密码修改失败，请确保新密码和确认密码输入相同并且非空。");
             }
           }
     else {
         QMessageBox::information(this,"infor", "密码修改失败，当前密码输入错误！。");
     }
}

void customer_change_passwd::on_backButton_clicked()
{
    send_user_back();
}
