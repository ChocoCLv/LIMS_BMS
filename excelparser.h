#ifndef EXCELPARSER_H
#define EXCELPARSER_H

#include <QObject>
#include <QList>
#include <QVariant>
#include "global.h"

//每次只解析一张表的数据

class ExcelParser : public QObject
{
    Q_OBJECT
public:
    explicit ExcelParser(QObject *parent = 0);
    void setExcelData(QList<QList<QVariant> >eData);
    void setDataType(ImportDataType t);

private:
    QList<QList<QVariant> > excelData;//存储表格的数据
    ImportDataType dataType;

signals:

public slots:
};

#endif // EXCELPARSER_H
