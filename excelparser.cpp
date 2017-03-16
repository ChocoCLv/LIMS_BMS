#include "excelparser.h"

ExcelParser::ExcelParser(QObject *parent) : QObject(parent)
{

}


void ExcelParser::setExcelData(QList<QList<QVariant> > eData)
{
    excelData = eData;
}

void ExcelParser::setDataType(ImportDataType type)
{
    dataType = type;
}
