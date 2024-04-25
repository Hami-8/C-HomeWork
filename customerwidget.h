#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include <QWidget>
#include<vector>

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

    void getShop(vector<ShopInfo> ShopVec);   //打印附近店铺界面


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
};

#endif // CUSTOMERWIDGET_H
