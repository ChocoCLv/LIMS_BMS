#include "excelparser.h"

ExcelParser::ExcelParser(QObject *parent) : QObject(parent)
{

}


void ExcelParser::SetExcelData(QList<QList<QVariant> > eData)
{
    excelData = eData;
}

void ExcelParser::SetDataType(ImportDataType type)
{
    dataType = type;
}

void ExcelParser::ParseData()
{
    switch(dataType)
    {
    case COURSE_TIME_TABLE:
        ImportCourseTimerTable();
        break;
    default:
        break;
    }
}

void ExcelParser::ImportCourseTimerTable()
{
    CourseTimeTable *couseTimeTable = new CourseTimeTable();
    int startColumn =

}
