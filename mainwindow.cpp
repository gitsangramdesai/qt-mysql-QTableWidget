#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QHBoxLayout>
#include <QSignalMapper>



void ShowMsg(QString strMsg)
{
    QMessageBox msgBox;
    msgBox.setText(strMsg);
    msgBox.exec();
}

void LoadData(Ui::MainWindow* ui)
{
    QSqlDatabase db;


    db = QSqlDatabase::addDatabase("QMYSQL","MyConnect");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("sangram");
    db.setDatabaseName("sangram");
    int rowIndex =0;

    if (db.open()) {
        QSqlQuery query(QSqlDatabase::database("MyConnect"));
        query.prepare("SELECT * FROM MyUser");
        if(query.exec())
       {
               ui->tableWidget->setRowCount(query.size());
               ui->tableWidget->setColumnCount(4);
               ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Id" << "ShortName" << "Telephone"<<"SAVE");

               while(query.next())
               {
                   QString id  = query.value(0).toString();
                   QString ShortName = query.value(1).toString();
                   QString tel = query.value(2).toString();

                   //data columns
                   QTableWidgetItem* item1 = new QTableWidgetItem();
                   item1->setText(id);
                   ui->tableWidget->setItem(rowIndex,0,item1);

                   QTableWidgetItem* item2 = new QTableWidgetItem();
                   item2->setText(ShortName);
                   ui->tableWidget->setItem(rowIndex,1,item2);

                   QTableWidgetItem* item3 = new QTableWidgetItem();
                   item3->setText(tel);
                   ui->tableWidget->setItem(rowIndex,2,item3);

                   rowIndex = rowIndex +1;
               }

       }
    }

    //set text field empty
    ui->leID->setText("");
    ui->leName->setText("");
    ui->leTel->setText("");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LoadData(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbSubmit_clicked()
{
    QSqlDatabase db;

    db = QSqlDatabase::addDatabase("QMYSQL","MyConnect");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("sangram");
    db.setDatabaseName("sangram");

   QString name = ui->leName->text();
   QString  tel = ui->leTel->text();
   QString id = ui->leID->text();

    if (db.open()) {
        QMessageBox::information(this,"Connection","Connection Success");
        QSqlQuery query(QSqlDatabase::database("MyConnect"));
        if(id =="")
        {
            query.prepare("INSERT INTO MyUser(shortName,tel)values(:shortName,:telephone)");
            query.bindValue(":shortName",name);
            query.bindValue(":telephone",tel);
        }else{
            query.prepare("UPDATE MyUser SET shortName=:shortName ,tel=:tel where id=:id");
            query.bindValue(":shortName",name);
            query.bindValue(":tel",tel);
            query.bindValue(":id",id);
        }
         if(query.exec())
        {
             LoadData(ui);
             QMessageBox::information(this,"Insertion","Insertion Success");
        }else
        {
                QMessageBox::information(this,"Insertion","Insertion failed");
        }


    }else{
        QMessageBox::information(this,"Connection","Connection failed");
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    QString Id = ui->tableWidget->item(row,0)->text();
    QString name = ui->tableWidget->item(row,1)->text();
    QString  tel = ui->tableWidget->item(row,2)->text();

    ui->leID->setText(Id);
    ui->leName->setText(name);
    ui->leTel->setText(tel);

    ShowMsg("Clicked2-" + QString::number(row));
}

