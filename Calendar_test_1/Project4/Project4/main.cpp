#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int getNumberOfDays(int month, int year)
{
    if (month == 2)
    {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
        || month == 10 || month == 12)
        return 31;
    else
        return 30;
}

void drawCalendar(sf::RenderWindow& window, const vector<string>& weekDays, int firstWeekDayOfMonth, int numberOfDays, const unordered_map<int, string>& reminders)
{
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))  // 폰트 파일의 경로를 지정합니다.
    {
        cerr << "폰트 파일을 불러오는 데 실패했습니다." << endl;
        return;
    }

    int xOffset = 50, yOffset = 50;
    int cellWidth = 100, cellHeight = 50;

    for (int w = 0; w < 7; ++w)
    {
        sf::Text dayText;
        dayText.setFont(font);
        dayText.setString(weekDays[w]);
        dayText.setCharacterSize(24);
        dayText.setFillColor(sf::Color::Black);
        dayText.setPosition(xOffset + w * cellWidth, yOffset);
        window.draw(dayText);
    }

    int tempBreak = 1;
    for (int w = 0; w < firstWeekDayOfMonth; w++)
    {
        tempBreak++;
    }

    for (int d = 1; d <= numberOfDays; d++)
    {
        sf::Text dayText;
        dayText.setFont(font);
        dayText.setString(to_string(d));
        dayText.setCharacterSize(24);
        dayText.setFillColor(reminders.count(d) ? sf::Color::Red : sf::Color::Black);
        dayText.setPosition(xOffset + (tempBreak - 1) * cellWidth, yOffset + cellHeight);

        if (reminders.count(d))
        {
            sf::RectangleShape reminderBox(sf::Vector2f(cellWidth, cellHeight));
            reminderBox.setFillColor(sf::Color::Yellow);
            reminderBox.setPosition(xOffset + (tempBreak - 1) * cellWidth, yOffset + cellHeight);
            window.draw(reminderBox);
        }

        window.draw(dayText);

        if (tempBreak >= 7)
        {
            yOffset += cellHeight;
            tempBreak = 0;
        }
        tempBreak++;
    }
}

int main()
{
    int currentDay, currentMonth, currentYear, firstWeekDayOfMonth, numberOfDays;
    vector<string> months = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
    vector<string> weekDays = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

    time_t currentTime = time(0);
    struct tm now;
    localtime_s(&now, &currentTime);
    currentDay = now.tm_mday;
    currentYear = now.tm_year + 1900;
    currentMonth = now.tm_mon;
    numberOfDays = getNumberOfDays(currentMonth + 1, currentYear);

    struct tm tFirst = { 0, 0, 0, 1, currentMonth, currentYear - 1900 };
    time_t time_temp = mktime(&tFirst);
    struct tm firstTime;
    localtime_s(&firstTime, &time_temp);

    firstWeekDayOfMonth = firstTime.tm_wday;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Calendar");

    unordered_map<int, string> reminders;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                int reminderDay;
                string reminderMessage;

                cout << "리마인더 날짜를 입력하세요: ";
                cin >> reminderDay;
                cout << "리마인더 메시지를 입력하세요: ";
                cin.ignore();
                getline(cin, reminderMessage);

                if (reminderDay >= 1 && reminderDay <= numberOfDays)
                {
                    reminders[reminderDay] = reminderMessage;
                }
                else
                {
                    cout << "잘못된 날짜입니다. 다시 시도하세요." << endl;
                }

                char anotherReminder;
                cout << "다른 리마인더를 추가하시겠습니까? (y/n): ";
                cin >> anotherReminder;
                if (anotherReminder != 'y' && anotherReminder != 'Y')
                {
                    break;
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        drawCalendar(window, weekDays, firstWeekDayOfMonth, numberOfDays, reminders);
        window.display();
    }

    if (!reminders.empty())
    {
        cout << "\n==================리마인더=====================\n";
        for (const auto& reminder : reminders)
        {
            cout << "리마인더 날짜: " << reminder.first << "\n" << reminder.second << endl;
        }
    }

    return 0;
}
