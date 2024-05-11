#ifndef MERCHANT_H
#define MERCHANT_H
#include<QString>
#include "structural.h"
#include<vector>
using namespace std;
class Merchant
{
public:
    Merchant();
    QString name;    //店铺名字
    QString locate;    //位置
    QString time;    //营业时间
    double distance;    //距离，单位：千米
    vector<Food> menu;  //店铺菜单
    int id;

};

#endif // MERCHANT_H
