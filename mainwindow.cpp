#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->fileSelectBtn,SIGNAL(clicked()),this,SLOT(getFileName()));
    connect(ui->openBtn,SIGNAL(clicked(bool)),this,SLOT(openFile()));
    connect(ui->openAction,SIGNAL(triggered(bool)),this,SLOT(openFile()));
}

void MainWindow::getFileName()
{
    fileName = QFileDialog::getOpenFileName(this,"Open File",QDir::currentPath(),"表格文件(*.xls *.xlsx)");
    ui->fileNameEdt->setText(fileName);
}

void MainWindow::openFile()
{
    excel = new ExcelEngine(fileName);
    excel->Open();
    excel->ReadDataToTable(ui->tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
