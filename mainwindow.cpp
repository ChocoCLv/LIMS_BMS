#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataTypeRadioButtonGrp = new QButtonGroup(this);
    dataTypeRadioButtonGrp->addButton(ui->courseTimeTableRB,COURSE_TIME_TABLE);
    dataTypeRadioButtonGrp->addButton(ui->radioButton_2,1);
    dataTypeRadioButtonGrp->addButton(ui->radioButton_3,2);

    connect(ui->fileSelectBtn,SIGNAL(clicked()),this,SLOT(getFileName()));
    connect(ui->openBtn,SIGNAL(clicked(bool)),this,SLOT(openFile()));
    connect(ui->openAction,SIGNAL(triggered(bool)),this,SLOT(openFile()));

    connect(ui->courseTimeTableRB,SIGNAL(clicked(bool)),this,SLOT(setDataType()));
    connect(ui->radioButton_2,SIGNAL(clicked(bool)),this,SLOT(setDataType()));
    connect(ui->radioButton_3,SIGNAL(clicked(bool)),this,SLOT(setDataType()));
}

void MainWindow::getFileName()
{
    fileName = QFileDialog::getOpenFileName(this,"Open File",QDir::currentPath(),"表格文件(*.xls *.xlsx)");
    ui->fileNameEdt->setText(fileName);
}

void MainWindow::openFile()
{
    excelEngine = new ExcelEngine(fileName);
    excelEngine->Open();
    excelEngine->ReadDataToTable(ui->tableWidget);
    ui->optionGrp->setEnabled(true);
}

void MainWindow::setDataType()
{
    switch(dataTypeRadioButtonGrp->checkedId())
    {
    case COURSE_TIME_TABLE:
        excelParser = new ExcelParser(this);
        excelParser->ImportCourseTimerTable(excelEngine->GetExcelData());
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
