#ifndef EXCELPARSER_H
#define EXCELPARSER_H

#include <QObject>
#include <QList>
#include <QVariant>
#include "global.h"
#include <QDebug>
#include "coursetimetable.h"

//每次只解析一张表的数据

class ExcelParser : public QObject
{
    Q_OBJECT
public:
    explicit ExcelParser(QObject *parent = 0);
    void ImportCourseTimerTable(QList<QList<QVariant> > data);

private:
    QList<QList<QVariant> > excelData;//存储表格的数据
    ImportDataType dataType;
    CourseTimeTable courseTimeTable;

signals:

public slots:
};

#endif // EXCELPARSER_H
