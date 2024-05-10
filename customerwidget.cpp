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
    connectsql();
    //初始化各个界面的默认值
    ui->customerWidget->setCurrentIndex(0);
    ui->homePage_stacked->setCurrentIndex(0);
    ui->orderStackedWidget->setCurrentIndex(0);
    ui->menuTabWidget->setCurrentIndex(0);
    //初始化商店表格
    // 设置选择模式为整行选择
    ui->shopTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->shopTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 调整表头，使其适应内容
    ui->shopTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->shopTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //初始化购物车表格
    QStringList headtext;
    headtext<<"序号"<<"奶茶名"<<"价格"<<"库存" << "操作";
    ui->cartTableWidget->setColumnCount(headtext.count());     //列表设置为和headtext相等
    ui->cartTableWidget->setHorizontalHeaderLabels(headtext);  //插入表头
    ui->cartTableWidget->setRowCount(0);
    // 设置选择模式为整行选择
    ui->cartTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->cartTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 调整表头，使其适应内容
    ui->cartTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    // 初始化订单表格
    QStringList headerLabels;
    headerLabels << "订单序号" << "配送方式" << "店铺" << "内容" << "状态" << "总额"<<"操作";
    ui->orderTableWidget->setColumnCount(headerLabels.count());
    ui->orderTableWidget->setHorizontalHeaderLabels(headerLabels);
    // 设置选择模式为整行选择
    ui->orderTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->orderTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 调整表头，使其适应内容
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //将一些lineEdit设置为只读
    ui->shopLineEdit->setReadOnly(1);
    ui->nickLineEdit->setReadOnly(1);
    ui->phoneLineEdit->setReadOnly(1);

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

void CustomerWidget::connectsql()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("Mixue_64");
    db.setUserName("root");
    db.setPassword("abc000000");
    db.open();
    bool ok=db.open();
    if(ok)
    {
        qDebug()<<"数据库连接成功！";
    }
    else
    {
       QMessageBox::information(this,"infor", "link failed");
       qDebug()<<"error open database because"<<db.lastError().text();
    }
}
void CustomerWidget::refresh()
{
    // 清空购物车界面的表格
    ui->cartTableWidget->clearContents();
    // 清空店铺菜单界面的表格
    ui->milkTeaTableWidget->clearContents();
    ui->fruitTeaTableWidget->clearContents();

    // 重新加载店铺菜单界面的内容
    getMenu();

    // 将总价钱置为0
    ui->sumLcdNumber->display(0.0);
    //刷新订单页
    refreshOrder();
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
    headtext<<"序号"<<"奶茶名"<<"价格"<<"库存"<<"操作";
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
                // 更新总价钱显示
                updateTotalPrice();
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
                // 更新总价钱显示
                updateTotalPrice();
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
    //增加删除按钮
    QPushButton *removeFromCartButton = new QPushButton("删除");
    connect(removeFromCartButton, &QPushButton::clicked, this, [=]() {
        // 获取按钮所在的行号
        int row = ui->cartTableWidget->indexAt(removeFromCartButton->pos()).row();

        // 删除当前行
        ui->cartTableWidget->removeRow(row);
        // 弹出删除成功对话框
        QMessageBox::information(this, "提示", "删除成功！");
        // 更新总价钱显示
        updateTotalPrice();
    });
    ui->cartTableWidget->setCellWidget(rowCount, 4, removeFromCartButton); // 将删除按钮放置在第5列
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
        //增加删除按钮
        QPushButton *removeFromCartButton = new QPushButton("删除");
        connect(removeFromCartButton, &QPushButton::clicked, this, [=]() {
            // 获取按钮所在的行号
            int row = ui->cartTableWidget->indexAt(removeFromCartButton->pos()).row();

            // 删除当前行
            ui->cartTableWidget->removeRow(row);
            // 弹出删除成功对话框
            QMessageBox::information(this, "提示", "删除成功！");
            // 更新总价钱显示
            updateTotalPrice();
        });
        ui->cartTableWidget->setCellWidget(rowCount, 4, removeFromCartButton); // 将删除按钮放置在第5列
    }
}

//更新购物车中的商品总额
void CustomerWidget::updateTotalPrice() {
    double totalPrice = 0.0;

    // 遍历购物车表格，累加每种商品的价格
    for (int row = 0; row < ui->cartTableWidget->rowCount(); ++row) {
        // 获取当前行的商品价格，并累加到总价钱上
        QString priceStr = ui->cartTableWidget->item(row, 2)->text(); // 第3列是价格列
        double price = priceStr.toDouble();
        totalPrice += price;
    }
    order->m_sum = totalPrice;

    // 将总价钱显示在sumLcdNumber控件上
    ui->sumLcdNumber->display(totalPrice);
}

void CustomerWidget::refreshOrder()
{

    int rowcount = ui->orderTableWidget->rowCount();
    ui->orderTableWidget->insertRow(rowcount);
    //订单序号
    QTableWidgetItem *column = new QTableWidgetItem(QString::number(order->num));
    Order::num++;
    //配送方式
    QString delivery_way;
    if(order->delivery_way==0)
    {
        delivery_way = "到店自取";
    }
    else
    {
        delivery_way = "雪王外送";
    }
    QTableWidgetItem *column1 = new QTableWidgetItem(delivery_way);
    //店铺
    QTableWidgetItem *column2 = new QTableWidgetItem(order->merchant.name);
    //内容
    QString content = order->FoodVec[0].name + " 等";
    QTableWidgetItem *column3 = new QTableWidgetItem(content);
    //状态
    QString state;
    if(order->state==0)
    {
        state = "进行中";
    }
    else
    {
        state = "已完成";
    }
    QTableWidgetItem *column4 = new QTableWidgetItem(state);
    //总额
    QTableWidgetItem *column5 = new QTableWidgetItem(QString::number(order->m_sum));

    //操作
    QPushButton *detailButton = new QPushButton("查看详情");
    connect(detailButton, &QPushButton::clicked, this, [=]() {
        //在此完善代码，当点击detailButton时，弹出一个对话框，用于显示详细信息
    });

    ui->orderTableWidget->setItem(rowcount,0,column);
    ui->orderTableWidget->setItem(rowcount,1,column1);
    ui->orderTableWidget->setItem(rowcount,2,column2);
    ui->orderTableWidget->setItem(rowcount,3,column3);
    ui->orderTableWidget->setItem(rowcount,4,column4);
    ui->orderTableWidget->setItem(rowcount,5,column5);
    ui->orderTableWidget->setCellWidget(rowcount, 6, detailButton);
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

    //待实现，接收购物车中的菜品，提交给订单界面
    // 遍历购物车表格，将每个商品添加至订单的FoodVec中
    for (int row = 0; row < ui->cartTableWidget->rowCount(); ++row) {
        FoodInfo food;
        food.number = ui->cartTableWidget->item(row, 0)->text();
        food.name = ui->cartTableWidget->item(row, 1)->text();
        food.price = ui->cartTableWidget->item(row, 2)->text().toDouble();
        food.quatity = ui->cartTableWidget->item(row, 3)->text().toInt();
        food.post = ""; // 备注暂时为空

        order->FoodVec.push_back(food);

        // 更新对应店铺菜单中的库存
        for (int i = 0; i < order->merchant.menu.size(); ++i) {
            if (order->merchant.menu[i].number == food.number) {
                order->merchant.menu[i].quatity--; // 减去购买的数量
                break;
            }
        }
    }


    // 提示结算成功
    QMessageBox::information(this, "提示", "结算成功！");
    refresh();
    ui->customerWidget->setCurrentIndex(2);
}


void CustomerWidget::on_cartButton_clicked()
{
    ui->orderStackedWidget->setCurrentIndex(1);

}

void CustomerWidget::on_backToOrderButton_clicked()
{
    ui->orderStackedWidget->setCurrentIndex(0);
}

void CustomerWidget::on_toPayButton_clicked()
{
    ui->orderStackedWidget->setCurrentIndex(1);
}
