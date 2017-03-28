#include "excelparser.h"

ExcelParser::ExcelParser(QObject *parent) : QObject(parent)
{

}

void ExcelParser::ImportCourseTimerTable(QList<QList<QVariant> > data)
{
    excelData = data;
    dataType = COURSE_TIME_TABLE;
    QList<QVariant> row;
    QVariant cell;
    foreach (row, data) {
        foreach (cell, row) {
           QString courses = cell.toString();
           if(courses.contains("◇")){
                QStringList courseList = courses.split("\n");
                QString course;
                foreach (course, courseList) {
                   qDebug()<<course<<endl;
                }
           }
        }
    }
}
