#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QButtonGroup>

#include "excelengine.h"
#include "excelparser.h"
#include "global.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ExcelEngine *excelEngine;
    ExcelParser *excelParser;
    QString fileName;
    QButtonGroup *dataTypeRadioButtonGrp;

public slots:
    void getFileName();
    void openFile();
    void setDataType();
};

#endif // MAINWINDOW_H
