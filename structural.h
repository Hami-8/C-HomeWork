#ifndef STRUCTURAL_H
#define STRUCTURAL_H

#endif // STRUCTURAL_H
#include<vector>

//店铺信息
typedef struct Shop
{
    char shop_name[100];
    char shop_distance[100];
    char shop_time[30];
}ShopInfo;





typedef struct Food
{
    char table[5]; //桌号
    char number[10];//序号
    char food[100]; //餐名
    char price[10];//价格
    char quatity[10];//数量
    char post[200];//备注
}FoodInfo;

typedef struct Menu
{
    char number[10];//序号
    char food[100]; //餐名
    char price[10];//价格
    char state[20];//状态

}MenuInfo;
