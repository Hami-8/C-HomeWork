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

//void CustomerWidget::showWidget()
//{
//    CustomerWidget *p =new CustomerWidget();
//    p->show();
//}

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
