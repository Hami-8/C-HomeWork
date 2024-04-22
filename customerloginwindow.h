#ifndef CUSTOMERLOGINWINDOW_H
#define CUSTOMERLOGINWINDOW_H

#include <QWidget>

namespace Ui {
class CustomerLoginWindow;
}

class CustomerLoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerLoginWindow(QWidget *parent = nullptr);
    ~CustomerLoginWindow();

private slots:
    void on_loginButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::CustomerLoginWindow *ui;
};

#endif // CUSTOMERLOGINWINDOW_H
