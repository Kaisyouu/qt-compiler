#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QProcess"
#include "QString"
#include "QDebug"
#include "QScrollBar"
#include "QFile"
#include "QTextStream"
#include "QStandardItem"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model->setColumnCount(6);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("name"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("lev"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("type"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("addr"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("is_arr"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("arrlen"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0,30);
    ui->tableView->setColumnWidth(1,30);
    ui->tableView->setColumnWidth(2,30);
    ui->tableView->setColumnWidth(3,30);
    ui->tableView->setColumnWidth(4,30);
    ui->tableView->setColumnWidth(5,30);

    model->setItem(0, 0, new QStandardItem("a00"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser_2->append("=====Updating compiler using make=====\n");
    QProcess bash_proc;
    bash_proc.start("make -C ../src");
    bash_proc.waitForStarted();
    bash_proc.waitForFinished();
    QString output = bash_proc.readAllStandardOutput();
    QString error = bash_proc.readAllStandardError();
    if (!output.isEmpty())
    {
        ui->textBrowser_2->append(output);
    }
    else
    {
        ui->textBrowser_2->append(error);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser_2->append("=====Starting compiler...=====");
    QString filename = "mytest.x0";
    QFile file(filename);
    QString code = ui->textBrowser->toPlainText();
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << code << endl;
    }
    ui->textBrowser_2->append("=====Your code is saved as 'mytest.x0'=====");
    QProcess bash_proc;
    bash_proc.start("../src/com mytest.x0");
    bash_proc.waitForStarted();
    bash_proc.waitForFinished();
//    QString output = bash_proc.readAllStandardOutput();
    QString error = bash_proc.readAllStandardError();
    if (!error.isEmpty())
    {
        ui->textBrowser_2->append(error);
    }
    else
    {
        ui->textBrowser_4->clear();
        model->removeRows(0, model->rowCount());
        ui->textBrowser_3->clear();
        //read output
        QString line;
        QFile foutput("./fresult.txt");
        if (foutput.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&foutput);
            while (!stream.atEnd()){
                line = stream.readLine();
                ui->textBrowser_4->append(line);
            }
        }
        file.close();
        //read table
        QFile ftable("./ftable.txt");
        if (ftable.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&ftable);
            int lcnt = 0;
            while (!stream.atEnd()){
                line = stream.readLine();
                QStringList tokenList = line.split(" ");
                for (int i=0;i<tokenList.length();i++)
                {
                    model->setItem(lcnt, i, new QStandardItem(tokenList[i]));
                }
                lcnt++;
            }
        }
        file.close();
        //read pcode
        QFile fcode("./fcode.txt");
        if (fcode.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&fcode);
            while (!stream.atEnd()){
                line = stream.readLine();
                ui->textBrowser_3->append(line);
            }
        }
        file.close();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser_2->clear();
}
