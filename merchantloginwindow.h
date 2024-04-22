#ifndef MERCHANTLOGINWINDOW_H
#define MERCHANTLOGINWINDOW_H

#include <QWidget>

namespace Ui {
class MerchantLoginWindow;
}

class MerchantLoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MerchantLoginWindow(QWidget *parent = nullptr);
    ~MerchantLoginWindow();

private slots:
    void on_loginButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::MerchantLoginWindow *ui;
};

#endif // MERCHANTLOGINWINDOW_H
