#include "customerwidget.h"
#include "ui_customerwidget.h"
#include "customerloginwindow.h"


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
    //将一些lineEdit设置为只读
    ui->shopLineEdit->setReadOnly(1);

}

CustomerWidget::~CustomerWidget()
{
    delete ui;
}

//打印店铺
void CustomerWidget::getShop(vector<ShopInfo> ShopVec)
{
    QStringList headtext;
    headtext<<"店名"<<"距离"<<"营业时间";

    ui->shopTableWidget->setColumnCount(headtext.count());//列表设置为和headtext相等
    ui->shopTableWidget->setHorizontalHeaderLabels(headtext);//插入表头
    ui->shopTableWidget->setRowCount(0);


    for (int j=1;j<(ShopVec).size();j++)
    {
        int rowcount = ui->shopTableWidget->rowCount();

        ui->shopTableWidget->insertRow(rowcount);

        QTableWidgetItem *column = new QTableWidgetItem(ShopVec[j].shop_name);
        QTableWidgetItem *column1 = new QTableWidgetItem(ShopVec[j].shop_distance);
        QTableWidgetItem *column2 = new QTableWidgetItem(ShopVec[j].shop_time);


        ui->shopTableWidget->setItem(rowcount,0,column);
        ui->shopTableWidget->setItem(rowcount,1,column1);
        ui->shopTableWidget->setItem(rowcount,2,column2);

     }
}



void CustomerWidget::on_selfButton_clicked()
{
    ui->homePage_stacked->setCurrentIndex(1);
    //待实现接收到店自取信号的功能
}

void CustomerWidget::on_sendButton_clicked()
{
    ui->homePage_stacked->setCurrentIndex(1);
    //待实现接收雪王外送信号的功能
}

void CustomerWidget::on_homeBackButton_clicked()
{
    ui->homePage_stacked->setCurrentIndex(0);
}

void CustomerWidget::on_selectedShopButton_clicked()
{
    ui->customerWidget->setCurrentIndex(1);
    //待实现接收选中的店铺功能
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
