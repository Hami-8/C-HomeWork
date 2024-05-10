#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include<QMessageBox>
#include <QSqlError>


namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    void connectsql();

private slots:
    void on_customerButton_clicked();
    void on_merchantButton_clicked();

    void on_btn_merchant_clicked();

private:
    Ui::LoginWindow *ui;
    QSqlDatabase db;
};

#endif // LOGINWINDOW_H
