#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <avl.h>
#include <QMessageBox>
#include<customerqlabel.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    avl.readTxt(PATH);
    ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_LoginButton_clicked()
{
    QString account=ui->accountLineEdit->text(),
            password=ui->passwordLineEdit->text();

    if(account==""||password==""){
        QMessageBox::about(this,"WRONG","请输入账号密码");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
        return;
    }
    bool temp=avl._search(account);
    if(temp){
        bool flag=avl.search(account,password);
        if(flag)
        {
            QMessageBox::about(this,"SUCCESS","登录成功");
                ui->accountLineEdit->clear();
                ui->passwordLineEdit->clear();
        }

        else
        {
            QMessageBox::about(this,"FAILURE","密码错误请重新登录");
            ui->passwordLineEdit->clear();
        }

    }
    else {
        QMessageBox::about(this,"WRONG","该账号不存在");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
        return;
    }
}

void MainWindow::on_QuiteButton_clicked()
{
   if(QMessageBox::Yes==QMessageBox::question(this,"question","是否确定退出?",QMessageBox::Yes|QMessageBox::No)){
       close();
   }
}

void MainWindow::on_Register_label_clicked()
{
    QString account=ui->accountLineEdit->text(),
            password=ui->passwordLineEdit->text();
    if(account==""||password==""){
        QMessageBox::about(this,"WRONG","请输入账号密码");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
        return;
    }
    bool find=avl.add(account,password);
    if(find){
        QMessageBox::about(this,"SUCCESS","注册成功");
        avl.level_OrderWrite(PATH);
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
    else {
        QMessageBox::about(this,"FAILURE","已有该账号");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
}

void MainWindow::on_Modify_label_clicked()
{
    QString account=ui->accountLineEdit->text(),
            password=ui->passwordLineEdit->text();
    if(account==""||password==""){
        QMessageBox::about(this,"WRONG","请输入账号密码");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
        return;
    }
    bool temp=avl._search(account);
    if(temp){   //登录成功
         if(QMessageBox::Yes==QMessageBox::question(this,"question","是否确定要修改密码?",QMessageBox::Yes|QMessageBox::No)){
             avl.modify(account,password);
             QMessageBox::about(this,"SUCCESS","修改成功");
             avl.level_OrderWrite(PATH);
             ui->accountLineEdit->clear();
             ui->passwordLineEdit->clear();
         }
         else{
             QMessageBox::about(this,"ATTENTION","已终止修改密码");
             ui->accountLineEdit->clear();
             ui->passwordLineEdit->clear();
         }
    }
    else {
        QMessageBox::about(this,"WRONG","账号不存在");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
}

void MainWindow::on_Logoff_label_clicked()
{
    QString account=ui->accountLineEdit->text(),
            password=ui->passwordLineEdit->text();
    if(account==""||password==""){
        QMessageBox::about(this,"WRONG","请输入账号密码");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
        return;
    }
    bool temp=avl.search(account,password);
    if(temp){         //登录成功

        if(QMessageBox::Yes==QMessageBox::question(this,"question","登录成功,是否确定要注销账号?",QMessageBox::Yes|QMessageBox::No))
            {
            avl.remove(account);
                 QMessageBox::about(this,"SUCCESS","成功注销账号:"+account);
                 avl.level_OrderWrite(PATH);
                 ui->accountLineEdit->clear();
                 ui->passwordLineEdit->clear();
        }
        else
        {
            QMessageBox::about(this,"ATTENTION","注销已终止");
            ui->accountLineEdit->clear();
            ui->passwordLineEdit->clear();
        }

    }
    else {
        QMessageBox::about(this,"WRONG","注销失败请先登录");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
}
