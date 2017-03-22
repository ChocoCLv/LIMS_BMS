#include "coursetimetable.h"

CourseTimeTable::CourseTimeTable(QObject *parent) : QObject(parent)
{

}

void CourseTimeTable::addCourse(CourseInformation course)
{
    courseInformations.push_back(course);
}


