#ifndef STRUCTURAL_H
#define STRUCTURAL_H

#endif // STRUCTURAL_H
#include<vector>
#include <QDateTime>

//店铺信息
typedef struct Shop
{
    char shop_name[100];
    char shop_distance[100];
    char shop_time[30];
}ShopInfo;

struct Time
{
    // 创建 QDateTime 对象表示店铺的营业时间
    QDateTime openingTime;//= QDateTime::fromString("09:00", "hh:mm"); // 营业开始时间 9:00
    QDateTime closingTime;//= QDateTime::fromString("22:00", "hh:mm"); // 营业结束时间 22:00
};
//将Time类型转换成字符串类型
//QString formatBusinessHours(const Time& businessHours)
//{
//    QString openingTimeStr = businessHours.openingTime.toString("hh:mm");
//    QString closingTimeStr = businessHours.closingTime.toString("hh:mm");

//    return openingTimeStr + "-" + closingTimeStr;
//}



typedef struct Food
{
    int id;
    QString number;//序号    A开头表示奶茶，B开头表示果茶
    QString name; //餐名
    double price; //价格
    int quantity;//库存
    int category_id; //奶茶种类
    QString post;//备注
}FoodInfo;

typedef struct Menu
{
    char number[10];//序号
    char food[100]; //餐名
    char price[10];//价格
    char state[20];//状态

}MenuInfo;
