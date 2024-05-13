#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QDebug>
#include "customer_change_passwd.h"


#include "order.h"

using namespace std;

namespace Ui {
class CustomerWidget;
}

class CustomerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerWidget(int m_id,QWidget *parent = nullptr);
    ~CustomerWidget();
    //void showWidget(); // 显示主界面
    void connectsql();
    void getShop(vector<Merchant> ShopVec);   //打印附近店铺界面
    void getMenu();    //打印菜单
    void addToCart(int row,QChar type);    //添加至购物车
    void refresh();    //结算后，刷新菜单界面，购物车界面
    void refreshOrder();    //点击结算后，刷新订单界面

    void showOrderDetailDialog(const Order& order); //显示订单详情
    int id;
    bool IsValidPhoneNumber(const QString & phoneNum);

private slots:
    void updateTotalPrice();    //更新购物车中的总价钱

    void on_selfButton_clicked();

    void on_sendButton_clicked();

    void on_homeBackButton_clicked();


    void on_selectedShopButton_clicked();

    void on_payButton_clicked();

    void on_cartButton_clicked();

    void on_backToOrderButton_clicked();

    void on_toPayButton_clicked();

    void on_changeNickButton_clicked();

    void on_saveNickButton_clicked();

    void on_changePhoneButton_clicked();

    void on_savePhoneButton_clicked();

    void on_changePasswordButton_clicked();

private:
    Ui::CustomerWidget *ui;
    Order * order;
    vector<Order> OrderVec;
    vector<Merchant> ShopVec;
    QSqlDatabase db;
    QString username;
    QString password;
    QString phone;
    QString nick;
    customer_change_passwd changePasswdWidget;


};

#endif // CUSTOMERWIDGET_H
