#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include <QWidget>

namespace Ui {
class CustomerWidget;
}

class CustomerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerWidget(QWidget *parent = nullptr);
    ~CustomerWidget();
    //void showWidget(); // 显示主界面

private slots:
    void on_selfButton_clicked();

    void on_sendButton_clicked();

    void on_homeBackButton_clicked();

private:
    Ui::CustomerWidget *ui;
};

#endif // CUSTOMERWIDGET_H
