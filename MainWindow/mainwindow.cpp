#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QProcess"
#include "QString"
#include "QDebug"
#include "QScrollBar"
#include "QFile"
#include "QTextStream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
