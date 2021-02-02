#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <avl.h>
#include"customerqlabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_LoginButton_clicked();         //登录的槽

    void on_QuiteButton_clicked();         //退出的槽

    void on_Register_label_clicked();      //注册的槽

    void on_Modify_label_clicked();        //修改密码的槽

    void on_Logoff_label_clicked();        //注销的槽

private:
    Ui::MainWindow *ui;
     AVL<QString,QString> avl;
     const QString PATH=".\\test2.txt";

};

#endif // MAINWINDOW_H
