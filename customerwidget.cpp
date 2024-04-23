#include "customerwidget.h"
#include "ui_customerwidget.h"
#include "customerloginwindow.h"

CustomerWidget::CustomerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerWidget)
{
    ui->setupUi(this);
    ui->homePage_stacked->setCurrentIndex(0);
}

CustomerWidget::~CustomerWidget()
{
    delete ui;
}

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
}

void CustomerWidget::on_sendButton_clicked()
{
    ui->homePage_stacked->setCurrentIndex(1);
}

void CustomerWidget::on_homeBackButton_clicked()
{
    ui->homePage_stacked->setCurrentIndex(0);
}
