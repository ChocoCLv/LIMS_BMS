#ifndef COURSETIMETABLE_H
#define COURSETIMETABLE_H

#include <QObject>
#include <QMap>
#include <QPair>

//存储导入的课程时间表信息

class CourseTimeTable : public QObject
{
    Q_OBJECT
public:
    explicit CourseTimeTable(QObject *parent = 0);

private:


signals:

public slots:
};

#endif // COURSETIMETABLE_H
