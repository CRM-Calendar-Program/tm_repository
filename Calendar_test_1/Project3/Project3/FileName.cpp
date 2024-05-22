#include <iostream>
#include <ctime>
#include <conio.h>
#include <string.h>
using namespace std;

//function will return total number of days
int getNumberOfDays(int month, int year)
{
    //leap year condition, if month is 2
    if (month == 2)
    {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    //months which has 31 days
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
        || month == 10 || month == 12)
        return 31;
    else
        return 30;
}

void printCalendar(const char** weekDays, int firstWeekDayOfMonth, int numberOfDays, int reminderDay) {
    int w, d;
    for (w = 0; w < 7; w++) {
        cout << weekDays[w] << "    ";
    }
    cout << "\n-------------------------------------------------\n";
    cout << "\n";
    int tempBreak = 1;

    for (w = 0; w < firstWeekDayOfMonth; w++) {
        cout << "       ";
        tempBreak++;
    }
    for (d = 1; d <= numberOfDays; d++) {
        if (d == reminderDay) {
            cout << "*" << d << "*" << "   ";
        }
        else {
            cout << d << "     ";
        }

        if (d < 10)
            cout << " ";
        if (tempBreak >= 7)
        {
            cout << "\n";
            tempBreak = 0;
        }
        tempBreak++;
    }
}

int main()
{
    int i, w, d, n, j, c = 1;
    const char* months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
    const char* weekDays[7] = { "Sun","Mon","Tue","Wed","Thu","Fri","Sat" };

    time_t currentTime = time(0);   // get time now
    struct tm now;
    localtime_s(&now, &currentTime);
    int currentDay = now.tm_mday;
    int weekDay = now.tm_wday;
    int currentYear = now.tm_year;
    int currentMonth = now.tm_mon;
    int numberOfDays = getNumberOfDays(now.tm_mon + 1, now.tm_year);

    struct tm tFirst = { 0, 0, 0, 1, currentMonth, currentYear };
    time_t time_temp = mktime(&tFirst);
    struct tm firstTime;
    localtime_s(&firstTime, &time_temp);

    int firstWeekDayOfMonth = firstTime.tm_wday;

    cout << "Current Date: " << firstTime.tm_mday << "-" << months[firstTime.tm_mon] << "-" << firstTime.tm_year + 1900 << "\n\n";
    cout << months[currentMonth] << "-" << currentYear + 1900 << "\n\n";

    printCalendar(weekDays, firstWeekDayOfMonth, numberOfDays, 0);

    cout << "\n";
    cout << "Press enter key to add reminder\n";
    _getch(); // using _getch instead of getch


    int dayReminder;
    char reminderMessage[100];

    cout << "Enter reminder day: ";
    cin >> dayReminder;
    cout << "Enter reminder message:";
    cin.ignore(); // to clear the input buffer
    cin.getline(reminderMessage, 100);

    cout << "\n\n";

    printCalendar(weekDays, firstWeekDayOfMonth, numberOfDays, dayReminder);

    cout << "\n==================Reminder=====================\n";
    cout << "Reminder day:" << dayReminder;
    cout << "\n" << reminderMessage;
    _getch(); // using _getch instead of getch

    return 0;
}
