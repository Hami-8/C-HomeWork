#ifndef CUSTOMER_CHANGE_PASSWD_H
#define CUSTOMER_CHANGE_PASSWD_H

#include <QWidget>

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QDebug>
#include<QString>

namespace Ui {
class customer_change_passwd;
}

class customer_change_passwd : public QWidget
{
    Q_OBJECT

public:
    explicit customer_change_passwd(QWidget *parent = nullptr);
    ~customer_change_passwd();
    void connectsql();
    int id;


signals:
    void send_user_back();

private slots:
    void on_confirmButton_clicked();

    void on_backButton_clicked();

private:
    Ui::customer_change_passwd *ui;
    QSqlDatabase db;
    QString password;
};

#endif // CUSTOMER_CHANGE_PASSWD_H
