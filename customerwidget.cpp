#include "customerwidget.h"
#include "ui_customerwidget.h"
#include "customerloginwindow.h"
#include<QDebug>
#include<QMessageBox>

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
    //初始化商店表格
    // 设置选择模式为整行选择
    ui->shopTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->shopTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 调整表头，使其适应内容
    ui->shopTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->shopTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //初始化购物车表格
    QStringList headtext;
    headtext<<"序号"<<"奶茶名"<<"价格"<<"库存";
    ui->cartTableWidget->setColumnCount(headtext.count());     //列表设置为和headtext相等
    ui->cartTableWidget->setHorizontalHeaderLabels(headtext);  //插入表头
    ui->cartTableWidget->setRowCount(0);
    // 设置选择模式为整行选择
    ui->cartTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->cartTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 调整表头，使其适应内容
    ui->cartTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


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
    headtext<<"序号"<<"奶茶名"<<"价格"<<"库存"<<"选项";
    ui->milkTeaTableWidget->setColumnCount(headtext.count());     //列表设置为和headtext相等
    ui->milkTeaTableWidget->setHorizontalHeaderLabels(headtext);  //插入表头
    ui->milkTeaTableWidget->setRowCount(0);
    // 设置选择模式为整行选择
    ui->milkTeaTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->milkTeaTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 调整表头，使其适应内容
    ui->milkTeaTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    QStringList headtext1;
    headtext1<<"序号"<<"果茶名"<<"价格"<<"库存"<<"选项";
    ui->fruitTeaTableWidget->setColumnCount(headtext1.count());    //列表设置为和headtext相等
    ui->fruitTeaTableWidget->setHorizontalHeaderLabels(headtext1); //插入表头
    ui->fruitTeaTableWidget->setRowCount(0);
    // 设置选择模式为整行选择
    ui->fruitTeaTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->fruitTeaTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 调整表头，使其适应内容
    ui->fruitTeaTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int j=0;j< order->merchant.menu.size();j++)
    {
        QChar type = order->merchant.menu[j].number[0];    //A表示奶茶，B表示果茶

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
            //添加“添加至购物车”按钮
            QPushButton *addToCartButton = new QPushButton("添加至购物车");
            connect(addToCartButton, &QPushButton::clicked, this, [=]() {
                // 获取按钮所在的单元格
                QPushButton* button = dynamic_cast<QPushButton*>(sender());
                //                if (!button) {
                //                    return; // 如果按钮无效，则退出槽函数
                //                }

                // 获取按钮所在的行号
                int row = ui->milkTeaTableWidget->indexAt(button->pos()).row();
                //qDebug()<<row;

                addToCart(row,'A');
                QMessageBox::information(this, "提示", "添加成功！");
            });// 调用槽函数，传入当前行号
            ui->milkTeaTableWidget->setCellWidget(rowcount,4,addToCartButton);
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
            //添加“添加至购物车”按钮
            QPushButton *addToCartButton = new QPushButton("添加至购物车");
            connect(addToCartButton, &QPushButton::clicked, this, [=]() {
                //qDebug()<<rowcount;
                addToCart(rowcount,'B');
                QMessageBox::information(this, "提示", "添加成功！");
            });// 调用槽函数，传入当前行号
            ui->fruitTeaTableWidget->setCellWidget(rowcount,4,addToCartButton);
        }
     }
}

//实现“添加至购物车”槽函数
void CustomerWidget::addToCart(int row,QChar type) {
    if(type == 'A')
    {
    // 获取当前行的数据

    QString str = ui->milkTeaTableWidget->item(row,0)->text(); //获取序号
    QString str1 = ui->milkTeaTableWidget->item(row,1)->text();//获取奶茶名
    QString str2 = ui->milkTeaTableWidget->item(row,2)->text();//获取价格



    QString str3 = ui->milkTeaTableWidget->item(row,3)->text();//获取库存
    // 在购物车的TableWidget中添加数据
    int rowCount = ui->cartTableWidget->rowCount();
    ui->cartTableWidget->insertRow(rowCount);
    ui->cartTableWidget->setItem(rowCount, 0, new QTableWidgetItem(str));
    ui->cartTableWidget->setItem(rowCount, 1, new QTableWidgetItem(str1));
    ui->cartTableWidget->setItem(rowCount, 2, new QTableWidgetItem(str2));
    ui->cartTableWidget->setItem(rowCount, 3, new QTableWidgetItem(str3));
    }
    else
    {
        // 获取当前行的数据

        QString str = ui->fruitTeaTableWidget->item(row,0)->text(); //获取序号
        QString str1 = ui->fruitTeaTableWidget->item(row,1)->text();//获取奶茶名
        QString str2 = ui->fruitTeaTableWidget->item(row,2)->text();//获取价格

        QString str3 = ui->fruitTeaTableWidget->item(row,3)->text();//获取库存
        // 在购物车的TableWidget中添加数据
        int rowCount = ui->cartTableWidget->rowCount();
        ui->cartTableWidget->insertRow(rowCount);
        ui->cartTableWidget->setItem(rowCount, 0, new QTableWidgetItem(str));
        ui->cartTableWidget->setItem(rowCount, 1, new QTableWidgetItem(str1));
        ui->cartTableWidget->setItem(rowCount, 2, new QTableWidgetItem(str2));
        ui->cartTableWidget->setItem(rowCount, 3, new QTableWidgetItem(str3));
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
