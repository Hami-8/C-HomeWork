#include "customerwidget.h"
#include "ui_customerwidget.h"
#include "customerloginwindow.h"
#include<QDebug>


CustomerWidget::CustomerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerWidget)
{
    ui->setupUi(this);
    //初始化订单
    order = new Order;
    //初始化各个界面的默认值
    ui->customerWidget->setCurrentIndex(0);
    ui->homePage_stacked->setCurrentIndex(0);
    ui->orderStackedWidget->setCurrentIndex(0);
    //初始化表格
    // 设置选择模式为整行选择
    ui->shopTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->shopTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 调整表头，使其适应内容
    ui->shopTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->shopTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //将一些lineEdit设置为只读
    ui->shopLineEdit->setReadOnly(1);

    //先设定一组店铺
    QDateTime openingTime= QDateTime::fromString("09:00", "hh:mm"); // 营业开始时间 9:00
    QDateTime closingTime= QDateTime::fromString("22:00", "hh:mm"); // 营业结束时间 22:00
    Time time1;
    time1.openingTime = openingTime;
    time1.closingTime = closingTime;
    Merchant m1,m2;
    m1.name="店铺1";  m2.name = "店铺2";
    m1.distance=1.6; m2.distance = 1.3;
    m1.time = time1;  m2.time = time1;
    Food f1; f1.name = "奶茶1";f1.number ="A001";f1.price=12.3;f1.quatity = 100;
    Food f2; f2.name = "果茶1";f2.number ="B001";f2.price=11.2;f2.quatity = 90;
    Food f3; f3.name = "奶茶2";f3.number ="A002";f3.price=9.9;f3.quatity = 2;
    m1.menu.push_back(f1);
    m1.menu.push_back(f2);
    m1.menu.push_back(f3);
    //往shopTableWidget中添加信息
    ShopVec.push_back(m1);
    ShopVec.push_back(m2);
    getShop(ShopVec);

}

CustomerWidget::~CustomerWidget()
{
    delete ui;
}

//打印店铺
void CustomerWidget::getShop(vector<Merchant> ShopVec)
{
    QStringList headtext;
    headtext<<"店名"<<"距离(km)"<<"营业时间";

    ui->shopTableWidget->setColumnCount(headtext.count());//列表设置为和headtext相等
    ui->shopTableWidget->setHorizontalHeaderLabels(headtext);//插入表头
    ui->shopTableWidget->setRowCount(0);


    for (int j=0;j<(ShopVec).size();j++)
    {
        int rowcount = ui->shopTableWidget->rowCount();

        ui->shopTableWidget->insertRow(rowcount);

        QTableWidgetItem *column = new QTableWidgetItem(ShopVec[j].name);
        QString distance_string = QString::number(ShopVec[j].distance);
        QTableWidgetItem *column1 = new QTableWidgetItem(distance_string);


        QString openingTimeStr = ShopVec[j].time.openingTime.toString("hh:mm");
        QString closingTimeStr = ShopVec[j].time.closingTime.toString("hh:mm");

        QString time_string = openingTimeStr + "-" +closingTimeStr;
        QTableWidgetItem *column2 = new QTableWidgetItem(time_string);


        ui->shopTableWidget->setItem(rowcount,0,column);
        ui->shopTableWidget->setItem(rowcount,1,column1);
        ui->shopTableWidget->setItem(rowcount,2,column2);

     }
}

//打印菜单
void CustomerWidget::getMenu()
{
    QStringList headtext;
    headtext<<"序号"<<"奶茶名"<<"价格"<<"库存";
    ui->milkTeaTableWidget->setColumnCount(headtext.count());     //列表设置为和headtext相等
    ui->milkTeaTableWidget->setHorizontalHeaderLabels(headtext);  //插入表头
    ui->milkTeaTableWidget->setRowCount(0);

    QStringList headtext1;
    headtext1<<"序号"<<"果茶名"<<"价格"<<"库存";
    ui->fruitTeaTableWidget->setColumnCount(headtext1.count());    //列表设置为和headtext相等
    ui->fruitTeaTableWidget->setHorizontalHeaderLabels(headtext1); //插入表头
    ui->fruitTeaTableWidget->setRowCount(0);

    for (int j=0;j< order->merchant.menu.size();j++)
    {
        QCharRef type = order->merchant.menu[j].number[0];    //A表示奶茶，B表示果茶

        if( type == 'A')    //是奶茶
        {
            int rowcount = ui->milkTeaTableWidget->rowCount();
            ui->milkTeaTableWidget->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(order->merchant.menu[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(order->merchant.menu[j].name);
            QTableWidgetItem *column2 = new QTableWidgetItem(QString::number(order->merchant.menu[j].price));
            QTableWidgetItem *column3 = new QTableWidgetItem(QString::number(order->merchant.menu[j].quatity));

            ui->milkTeaTableWidget->setItem(rowcount,0,column);
            ui->milkTeaTableWidget->setItem(rowcount,1,column1);
            ui->milkTeaTableWidget->setItem(rowcount,2,column2);
            ui->milkTeaTableWidget->setItem(rowcount,3,column3);
        }
        else
        {
            int rowcount = ui->fruitTeaTableWidget->rowCount();
            ui->fruitTeaTableWidget->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(order->merchant.menu[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(order->merchant.menu[j].name);
            QTableWidgetItem *column2 = new QTableWidgetItem(QString::number(order->merchant.menu[j].price));
            QTableWidgetItem *column3 = new QTableWidgetItem(QString::number(order->merchant.menu[j].quatity));


            ui->fruitTeaTableWidget->setItem(rowcount,0,column);
            ui->fruitTeaTableWidget->setItem(rowcount,1,column1);
            ui->fruitTeaTableWidget->setItem(rowcount,2,column2);
            ui->fruitTeaTableWidget->setItem(rowcount,3,column3);
        }
     }
}

void CustomerWidget::on_selfButton_clicked()
{
    ui->homePage_stacked->setCurrentIndex(1);
    //接收到店自取信号
    order->delivery_way=DeliveryOption::Pickup;
}

void CustomerWidget::on_sendButton_clicked()
{
    ui->homePage_stacked->setCurrentIndex(1);
    //接收雪王外送信号
    order->delivery_way=DeliveryOption::Delivery;
}

void CustomerWidget::on_homeBackButton_clicked()
{
    ui->homePage_stacked->setCurrentIndex(0);
}

void CustomerWidget::on_selectedShopButton_clicked()
{
    ui->customerWidget->setCurrentIndex(1);
    //接收选中的店铺
    int row = ui->shopTableWidget->currentRow();
    qDebug()<<row;
    order->merchant = ShopVec[row];
    ui->shopLineEdit->setText(ShopVec[row].name);
    //待实现接收店铺的菜单信息
    getMenu();
}

void CustomerWidget::on_payButton_clicked()
{
    ui->customerWidget->setCurrentIndex(2);
    //待实现，接收购物车中的菜品，提交给订单界面
}

void CustomerWidget::on_cartButton_clicked()
{
    ui->orderStackedWidget->setCurrentIndex(1);
    //待实现，刷新购物车功能
}

void CustomerWidget::on_backToOrderButton_clicked()
{
    ui->orderStackedWidget->setCurrentIndex(0);
}

void CustomerWidget::on_toPayButton_clicked()
{
    ui->orderStackedWidget->setCurrentIndex(1);
}
