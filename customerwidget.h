#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include <QWidget>


#include "order.h"

using namespace std;

namespace Ui {
class CustomerWidget;
}

class CustomerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerWidget(QWidget *parent = nullptr);
    ~CustomerWidget();
    //void showWidget(); // 显示主界面

    void getShop(vector<Merchant> ShopVec);   //打印附近店铺界面
    void getMenu();
    void addToCart(int row,QChar type);


private slots:
    void on_selfButton_clicked();

    void on_sendButton_clicked();

    void on_homeBackButton_clicked();


    void on_selectedShopButton_clicked();

    void on_payButton_clicked();

    void on_cartButton_clicked();

    void on_backToOrderButton_clicked();

    void on_toPayButton_clicked();

private:
    Ui::CustomerWidget *ui;
    Order * order;
    vector<Merchant> ShopVec;
};

#endif // CUSTOMERWIDGET_H
