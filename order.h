#ifndef ORDER_H
#define ORDER_H

#include"merchant.h"
using namespace std;

enum DeliveryOption {
    Pickup = 0,   // 到店自取
    Delivery = 1  // 雪王外送
};
class Order
{
public:
    Order();
    int id;
    bool delivery_way;    //0为到店自取，1为雪王外送
    Merchant merchant;    //店铺
    vector<FoodInfo> FoodVec;
    double m_sum;    //总额
    bool state;    //订单状态，0为进行中，1为已完成
    //static int num;    //订单序号，从1开始
    QDateTime date;

};
//extern int Order::num;


#endif // ORDER_H
