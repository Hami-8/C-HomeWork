#ifndef CUSTOMERLOGINWINDOW_H
#define CUSTOMERLOGINWINDOW_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QDebug>
#include <QPainter>
#include <QStyleOption>



namespace Ui {
class CustomerLoginWindow;
}

class CustomerLoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerLoginWindow(QWidget *parent = nullptr);
    ~CustomerLoginWindow();
    void showCustomerWidget(); // 显示主界面
    void connectsql();
    int id;
    void paintEvent(QPaintEvent *e);


private slots:
    void on_loginButton_clicked();
    void on_exitButton_clicked();

signals:
    void loginSucceeded();

private:
    Ui::CustomerLoginWindow *ui;
    QSqlDatabase db;
};

#endif // CUSTOMERLOGINWINDOW_H
