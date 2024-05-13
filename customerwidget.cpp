#include "customerwidget.h"
#include "ui_customerwidget.h"
#include "customerloginwindow.h"

#include<QDebug>
#include<QMessageBox>

CustomerWidget::CustomerWidget(int m_id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerWidget)
{
    this->id = m_id;
    ui->setupUi(this);
    //初始化订单
    order = new Order;
    //连接数据库
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
    // 获取表头
    QHeaderView *header3 = ui->cartTableWidget->horizontalHeader();

    // 设置表头字体
    header3->setFont(QFont("Arial", 12)); // 使用Arial字体，大小为12像素


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


    // 获取表头
    QHeaderView *header4 = ui->orderTableWidget->horizontalHeader();

    // 设置表头字体
    header4->setFont(QFont("Arial", 12)); // 使用Arial字体，大小为12像素

    //将一些lineEdit设置为只读
    ui->shopLineEdit->setReadOnly(1);
    ui->nickLineEdit->setReadOnly(1);
    ui->phoneLineEdit->setReadOnly(1);

    //获取用户的昵称和电话，显示到“我的”界面
    QSqlQuery query(db);
    //qDebug()<<id;
    query.exec(QString("select * from Customer where customer_id= %1").arg(id));  //建立查询
      if(query.next())
       {
               username=query.value(1).toString();
               password=query.value(2).toString();
               phone=query.value(3).toString();
               nick=query.value(4).toString();
       }
    ui->nickLineEdit->setText(nick);
    ui->phoneLineEdit->setText(phone);

    //获取当前用户的历史订单信息，显示到“订单”界面；
    refreshOrder();   //刷新订单列表

    //从数据库中获取店铺信息，打印在”选择店铺”界面
    query.exec("SELECT * FROM Merchant");

    // 遍历查询结果集
    while (query.next()) {
        // 创建商家对象
        Merchant merchant;
        merchant.id = query.value("merchant_id").toInt();
        merchant.name = query.value("name").toString();
        merchant.locate = query.value("address").toString();
        merchant.time = query.value("opening_hours").toString();
        merchant.distance = query.value("distance").toDouble();

        // 将商家对象添加到商家列表中
        ShopVec.push_back(merchant);
    }

    changePasswdWidget.id = id;
    //密码修改界面返回到主界面
    auto mainwindow_show=[&]()
    {
        this->show();
        changePasswdWidget.hide();
    };
    connect(&changePasswdWidget,&customer_change_passwd::send_user_back,this,mainwindow_show);



    //先设定一组店铺
//    QDateTime openingTime= QDateTime::fromString("09:00", "hh:mm"); // 营业开始时间 9:00
//    QDateTime closingTime= QDateTime::fromString("22:00", "hh:mm"); // 营业结束时间 22:00
//    Time time1;
//    time1.openingTime = openingTime;
//    time1.closingTime = closingTime;
    Merchant m1,m2;
    m1.name="蜜雪冰城天津总店";  m2.name = "蜜雪冰城全国总店";
    m1.distance=1.6; m2.distance = 1.3;
    m1.time = "9:00至23:00";  m2.time = "10:00至21:00";
    Food f1; f1.name = "奶茶1";f1.number ="A001";f1.price=12.3;f1.quantity = 100;
    Food f2; f2.name = "果茶1";f2.number ="B001";f2.price=11.2;f2.quantity = 90;
    Food f3; f3.name = "奶茶2";f3.number ="A002";f3.price=9.9;f3.quantity = 2;
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


        //QString openingTimeStr = ShopVec[j].time.openingTime.toString("hh:mm");
        //QString closingTimeStr = ShopVec[j].time.closingTime.toString("hh:mm");

        QString time_string = ShopVec[j].time;
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

    // 获取表头
    QHeaderView *header = ui->milkTeaTableWidget->horizontalHeader();

    // 设置表头字体
    header->setFont(QFont("Arial", 12)); // 使用Arial字体，大小为12像素

    QStringList headtext1;
    headtext1<<"序号"<<"果茶名"<<"价格"<<"库存"<<"操作";
    ui->fruitTeaTableWidget->setColumnCount(headtext1.count());    //列表设置为和headtext相等
    ui->fruitTeaTableWidget->setHorizontalHeaderLabels(headtext1); //插入表头
    ui->fruitTeaTableWidget->setRowCount(0);
    // 设置选择模式为整行选择
    ui->fruitTeaTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->fruitTeaTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 获取表头
    QHeaderView *header2 = ui->fruitTeaTableWidget->horizontalHeader();

    // 设置表头字体
    header2->setFont(QFont("Arial", 12)); // 使用Arial字体，大小为12像素

    // 调整表头，使其适应内容
    ui->fruitTeaTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int j=0;j< order->merchant.menu.size();j++)
    {
        //QChar type = order->merchant.menu[j].number[0];    //A表示奶茶，B表示果茶

        if( order->merchant.menu[j].category_id==1)    //是奶茶
        {
            int rowcount = ui->milkTeaTableWidget->rowCount();
            ui->milkTeaTableWidget->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(QString::number(order->merchant.menu[j].id));
            QTableWidgetItem *column1 = new QTableWidgetItem(order->merchant.menu[j].name);
            QTableWidgetItem *column2 = new QTableWidgetItem(QString::number(order->merchant.menu[j].price));
            QTableWidgetItem *column3 = new QTableWidgetItem(QString::number(order->merchant.menu[j].quantity));

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
            QTableWidgetItem *column = new QTableWidgetItem(QString::number(order->merchant.menu[j].id));
            QTableWidgetItem *column1 = new QTableWidgetItem(order->merchant.menu[j].name);
            QTableWidgetItem *column2 = new QTableWidgetItem(QString::number(order->merchant.menu[j].price));
            QTableWidgetItem *column3 = new QTableWidgetItem(QString::number(order->merchant.menu[j].quantity));


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

//根据数据库信息，刷新OrderVec，并刷新订单界面
void CustomerWidget::refreshOrder()
{
    // 清空订单表格
    ui->orderTableWidget->clearContents();
    // 将行数设置为 0，从而删除所有行
    ui->orderTableWidget->setRowCount(0);
    OrderVec.clear();

    QSqlQuery query(db);
    query.exec(QString("SELECT * FROM Orders WHERE customer_id = %1").arg(id));

    // 遍历结果集
    while (query.next()) {

        // 从结果集中获取每一行的数据，并赋值给 Order 对象
        order->id= query.value("order_id").toInt();
        order->delivery_way= query.value("delivery_way").toInt();
        order->merchant.id= query.value("merchant_id").toInt();
        order->m_sum = query.value("total_money").toDouble();
        order->date = query.value("order_date").toDateTime();
        order->state = query.value("state").toInt();

        // 获取商户名称
        QSqlQuery merchantQuery(db);
        merchantQuery.exec(QString("SELECT name FROM Merchant WHERE merchant_id = %1").arg(order->merchant.id));
        if (merchantQuery.next()) {
            order->merchant.name = merchantQuery.value("name").toString();
        }

        //获取订单信息，赋值给Order的FoodVec
        // 获取订单包含的菜品信息
        QSqlQuery dishQuery(db);
        dishQuery.exec(QString("SELECT d.* "
                               "FROM DishInOrder dio "
                               "JOIN Dish d ON dio.dish_id = d.dish_id "
                               "WHERE dio.order_id = %1").arg(order->id));

        // 遍历菜品查询结果集，将菜品信息添加到 Order 的 FoodVec 数组中
        while (dishQuery.next()) {
            Food food;
            food.id = dishQuery.value("dish_id").toInt();
            food.name = dishQuery.value("name").toString();
            food.price = dishQuery.value("price").toDouble();
            food.category_id = dishQuery.value("category_id").toInt();
            food.quantity = dishQuery.value("quantity").toInt();

            order->FoodVec.push_back(food);
        }
        OrderVec.push_back(*order);
        // 清空当前订单对象
        order->FoodVec.clear();
        order->m_sum = 0.0;
    }

for (const auto& order : OrderVec)
{
    int rowcount = ui->orderTableWidget->rowCount();
    ui->orderTableWidget->insertRow(rowcount);
    //订单序号
    QTableWidgetItem *column = new QTableWidgetItem(QString::number(order.id));
    //Order::num++;
    //配送方式
    QString delivery_way;
    if(order.delivery_way==0)
    {
        delivery_way = "到店自取";
    }
    else
    {
        delivery_way = "雪王外送";
    }
    QTableWidgetItem *column1 = new QTableWidgetItem(delivery_way);
    //店铺
    QTableWidgetItem *column2 = new QTableWidgetItem(order.merchant.name);
    //内容
    QString content = order.FoodVec[0].name + " 等";
    QTableWidgetItem *column3 = new QTableWidgetItem(content);
    //状态
    QString state;
    if(order.state==0)
    {
        state = "进行中";
    }
    else
    {
        state = "已完成";
    }
    QTableWidgetItem *column4 = new QTableWidgetItem(state);
    //总额
    QTableWidgetItem *column5 = new QTableWidgetItem(QString::number(order.m_sum));

    //操作
    QPushButton *detailButton = new QPushButton("查看详情");
    connect(detailButton, &QPushButton::clicked, this, [=]() {
        //在此完善代码，当点击detailButton时，弹出一个对话框，用于显示详细信息
        showOrderDetailDialog(order);
    });

    ui->orderTableWidget->setItem(rowcount,0,column);
    ui->orderTableWidget->setItem(rowcount,1,column1);
    ui->orderTableWidget->setItem(rowcount,2,column2);
    ui->orderTableWidget->setItem(rowcount,3,column3);
    ui->orderTableWidget->setItem(rowcount,4,column4);
    ui->orderTableWidget->setItem(rowcount,5,column5);
    ui->orderTableWidget->setCellWidget(rowcount, 6, detailButton);

}
}

void CustomerWidget::showOrderDetailDialog(const Order& order)
{
    // 创建订单详情对话框
    QDialog *orderDetailDialog = new QDialog(this);
    orderDetailDialog->setWindowTitle("订单详情");

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(orderDetailDialog);

    // 添加订单详细信息到布局中，待修改
    QLabel *label = new QLabel("订单详细信息");
    layout->addWidget(label);
    QLabel *orderIdLabel = new QLabel("订单ID: " + QString::number(order.id));
    layout->addWidget(orderIdLabel);

    QLabel *merchantNameLabel = new QLabel("店铺名称: " + order.merchant.name);
    layout->addWidget(merchantNameLabel);

    QLabel *orderDateLabel = new QLabel("下单时间: " + order.date.toString("yyyy-MM-dd hh:mm:ss"));
    layout->addWidget(orderDateLabel);

    QLabel *stateLabel = new QLabel(QString("订单状态: ") + (order.state ? "已完成" : "进行中"));

    //QLabel *stateLabel = new QLabel("订单状态: " + (order.state ? "已完成" : "未完成"));
    layout->addWidget(stateLabel);

    QLabel *deliveryWayLabel = new QLabel(QString("配送方式: ") + (order.delivery_way ? "雪王外送" : "到店自取"));
    layout->addWidget(deliveryWayLabel);

    QString orderContentStr;
    for (const FoodInfo &food : order.FoodVec) {
        orderContentStr += food.name + " - 价格: " + QString::number(food.price) + "\n";
    }
    QLabel *orderContentLabel = new QLabel(QString("订单内容: ") + orderContentStr);
    layout->addWidget(orderContentLabel);

    QLabel *totalMoneyLabel = new QLabel(QString("总额: ") + QString::number(order.m_sum, 'f', 2));
    layout->addWidget(totalMoneyLabel);

    // 添加按钮到布局中
    QPushButton *closeButton = new QPushButton("关闭");
    connect(closeButton, &QPushButton::clicked, orderDetailDialog, &QDialog::close);
    layout->addWidget(closeButton);

    // 设置布局
    orderDetailDialog->setLayout(layout);

    // 显示订单详情对话框
    orderDetailDialog->exec();
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

    //从数据库中获取当前店铺的菜单
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    // 执行 SQL 查询，获取当前店铺id的菜单数据
//    query.prepare("SELECT d.dish_id, d.name, d.price, d.category_id, d.quantity "
//                  "FROM DishInMerchant dm "
//                  "JOIN Dish d ON dm.dish_id = d.dish_id "
//                  "WHERE dm.merchant_id = :merchantId");
//    query.bindValue(":merchantId", row+1);
    query.exec(QString("SELECT d.dish_id, d.name, d.price, d.category_id, d.quantity FROM DishInMerchant dm JOIN Dish d ON dm.dish_id = d.dish_id WHERE dm.merchant_id =%1").arg(row+1));
    // 遍历查询结果集
//    if(!query.next())
//    {
//        qDebug()<<"Fail";
//    }
    while (query.next()) {
        // 创建菜品对象
        Food food;
        food.id = query.value("dish_id").toInt();
        food.name = query.value("name").toString();
        food.price = query.value("price").toDouble();
        food.category_id = query.value("category_id").toInt();
        food.quantity = query.value("quantity").toInt();

        // 将菜品对象添加到当前店铺的菜单列表中
        order->merchant.menu.push_back(food);

    }



    //待实现接收店铺的菜单信息
    getMenu();
}

void CustomerWidget::on_payButton_clicked()
{

    //接收购物车中的菜品，提交给订单界面
    // 遍历购物车表格，将每个商品添加至订单的FoodVec中
    for (int row = 0; row < ui->cartTableWidget->rowCount(); ++row) {
        FoodInfo food;
        food.number = ui->cartTableWidget->item(row, 0)->text();
        food.id = ui->cartTableWidget->item(row, 0)->text().toInt();
        food.name = ui->cartTableWidget->item(row, 1)->text();
        food.price = ui->cartTableWidget->item(row, 2)->text().toDouble();
        food.quantity = ui->cartTableWidget->item(row, 3)->text().toInt();
        food.post = ""; // 备注暂时为空

        order->FoodVec.push_back(food);


        // 更新对应店铺菜单中的库存
        for (int i = 0; i < order->merchant.menu.size(); ++i) {
            if (order->merchant.menu[i].id == food.id) {
                order->merchant.menu[i].quantity--; // 减去购买的数量
                break;
            }
        }
    }

    // 将当前订单提交至数据库中
    QSqlQuery query(db);

    // 插入订单信息到 Orders 表中
//    query.prepare("INSERT INTO Orders (customer_id, merchant_id, state, delivery_way, total_money) "
//                  "VALUES (:customer_id, :merchant_id, :state, :delivery_way, :total_money)");
//    query.bindValue(":customer_id", id);
//    query.bindValue(":merchant_id", order->merchant.id);
//    //query.bindValue(":order_date", QDateTime::currentDateTime()); // 当前时间作为订单时间
//    query.bindValue(":state", 0); // 初始状态为进行中
//    query.bindValue(":delivery_way", order->delivery_way);
//    query.bindValue(":total_money", order->m_sum);

    QString queryString = QString("INSERT INTO Orders (customer_id, merchant_id, state, delivery_way, total_money) "
                                  "VALUES (%1, %2, %3, %4, %5)")
                            .arg(id)
                            .arg(order->merchant.id)
                            .arg(0)
                            .arg(order->delivery_way)
                            .arg(order->m_sum);
    if (!query.exec(queryString)) {
        qDebug() << "Error inserting order into database:" << query.lastError().text();
        //return;
    }

    // 获取刚插入订单的订单号
    int orderId = query.lastInsertId().toInt();

//    // 插入订单中包含的菜品信息到 DishInOrder 表中
//    for (const auto& food : order->FoodVec) {
//        query.prepare("INSERT INTO DishInOrder (order_id, dish_id, count) "
//                      "VALUES (:order_id, :dish_id, :count)");
//        query.bindValue(":order_id", orderId);
//        query.bindValue(":dish_id", food.id);
//        query.bindValue(":count", food.quantity);
//        if (!query.exec()) {
//            qDebug() << "Error inserting dish in order into database:" << query.lastError().text();
//            return;
//        }
//    }

    // 插入订单中包含的菜品信息到 DishInOrder 表中
    for (const auto& food : order->FoodVec) {
        QString insertQuery = QString("INSERT INTO DishInOrder (order_id, dish_id, count) "
                                      "VALUES (%1, %2, %3)")
                                  .arg(orderId)
                                  .arg(food.id)
                                  .arg(1);
        if (!query.exec(insertQuery)) {
            qDebug() << "Error inserting dish in order into database:" << query.lastError().text();
            //return;
        }
    }

//    // 更新 Dish 表，减去订单中已购买的菜品的数量
//    for (const auto& food : order->FoodVec) {
//        query.prepare("UPDATE Dish SET quantity = quantity - :count WHERE dish_id = :dish_id");
//        query.bindValue(":count", food.quantity);
//        query.bindValue(":dish_id", food.id);
//        if (!query.exec()) {
//            qDebug() << "Error updating dish quantity in database:" << query.lastError().text();
//            return;
//        }
//    }

    // 更新 Dish 表，减去订单中已购买的菜品的数量
    for (const auto& food : order->FoodVec) {
        QString updateQuery = QString("UPDATE Dish SET quantity = quantity - %1 WHERE dish_id = %2")
                                  .arg(1)
                                  .arg(food.id);
        if (!query.exec(updateQuery)) {
            qDebug() << "Error updating dish quantity in database:" << query.lastError().text();
            return;
        }
    }

    // 清空当前订单对象
    order->FoodVec.clear();
    order->m_sum = 0.0;

    //OrderVec.push_back(*order);

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

void CustomerWidget::on_changeNickButton_clicked()
{
    ui->nickLineEdit->setReadOnly(0);
}

void CustomerWidget::on_saveNickButton_clicked()
{
    nick = ui->nickLineEdit->text();
    ui->nickLineEdit->setReadOnly(1);
    //在数据库中修改昵称
    QSqlQuery query(db);
    query.exec(QString("update customer set nick='%1' where customer_id= %2").arg(nick).arg(id));
}

void CustomerWidget::on_changePhoneButton_clicked()
{
    ui->phoneLineEdit->setReadOnly(false);
}

void CustomerWidget::on_savePhoneButton_clicked()
{
    //如果手机号格式正确，则能够保存，否则无法保存
    phone=ui->phoneLineEdit->text();
    if(IsValidPhoneNumber(phone)==true)
    {
        qDebug()<<"格式正确";
        ui->phoneLineEdit->setReadOnly(true);
        //在数据库中修改电话号码
        QSqlQuery query(db);
        query.exec(QString("update customer set phone='%1' where customer_id='%2'").arg(phone).arg(id));
    }
    else {
        QMessageBox::information(this,"infor", "手机号格式错误");
    }
}

//判断手机号格式是否正确
bool CustomerWidget::IsValidPhoneNumber(const QString & phoneNum)
{
    QRegularExpression regex("^1[3456789]\\d{9}$");
        return regex.match(phoneNum).hasMatch();
}

void CustomerWidget::on_changePasswordButton_clicked()
{
    changePasswdWidget.show();
    this->hide();
}
