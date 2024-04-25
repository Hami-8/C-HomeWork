#ifndef ORDER_H
#define ORDER_H
#include "structural.h"
#include<vector>
using namespace std;

class Order
{
public:
    Order();
    bool delivery_way;    //0为到店自取，1为雪王外送
    Shop shop;    //把shop实现成类？
    vector<FoodInfo> FoodVec;
    bool state;    //订单状态，0为进行中，1为已完成
};

#endif // ORDER_H
