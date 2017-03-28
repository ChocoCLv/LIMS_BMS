#ifndef COURSETIMETABLE_H
#define COURSETIMETABLE_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QList>

//以连续的课时为单位
class ClassHour{
private:
    int dayOfWeek;          //某一次课的上课星期数。一次课可能有多个课时
    QList<int> sections;    //上课的节次
    QString address;        //上课的地点
public:
    void setWeekday(int _weekday){dayOfWeek = _weekday;}
    void addSection(int section){sections.push_back(section);}
    void setAddress(QString _address){address = _address;}
};

//存储导入的课程时间表信息
class CourseInformation
{
private:
    int id;                         //课程ID
    QString name;                   //课程名称
    QString teacherName;            //课程老师名字
    int startWeek;
    int endWeek;
    //QList<classHour> classHours;    //课程的课时信息
    //QList<int> classIDs;          //上课的班级ID

public:
    void setId(int _id){id = _id;}
    void setName(QString _name){name = _name;}
    void setTeacher(QString _teacherName){_teacherName = teacherName;}
    //void addClassHour(ClassHour ch){classHours.push_back(ch);}
    void setStartWeek(int _startWeek){startWeek = _startWeek;}
    void setEndWeek(int _endWeek){endWeek = _endWeek;}
};

class CourseTimeTable : public QObject
{
    Q_OBJECT
public:
    explicit CourseTimeTable(QObject *parent = 0);
    void addCourse(CourseInformation course);
    QByteArray getStreamData();

private:
    //存储课程信息条目
    //课程ID 课程名称 授课老师 上课时间（节次 周次） 上课班级
    QList<CourseInformation> courseInformations;
signals:

public slots:
};

#endif // COURSETIMETABLE_H
