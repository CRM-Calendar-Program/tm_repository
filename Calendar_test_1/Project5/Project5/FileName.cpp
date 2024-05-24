#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

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

void drawCalendar(sf::RenderWindow& window, const vector<string>& weekDays, int firstWeekDayOfMonth, int numberOfDays, const unordered_map<int, string>& reminders, int today)
{
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
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

        if (d == today)
        {
            dayText.setFillColor(sf::Color::Green);
        }
        else if (reminders.count(d))
        {
            dayText.setFillColor(sf::Color::Red);
        }
        else
        {
            dayText.setFillColor(sf::Color::Black);
        }

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

    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("N: Create Reminder  D: Delete Reminder  ESC: Cancel Input");
    instructions.setCharacterSize(14);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(10, window.getSize().y - 20);
    window.draw(instructions);
}

void saveReminders(const unordered_map<int, string>& reminders, const string& filename)
{
    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    for (const auto& reminder : reminders)
    {
        outFile << reminder.first << ":" << reminder.second << endl;
    }
}

void loadReminders(unordered_map<int, string>& reminders, const string& filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        size_t colonPos = line.find(':');
        if (colonPos != string::npos)
        {
            int day = stoi(line.substr(0, colonPos));
            string message = line.substr(colonPos + 1);
            reminders[day] = message;
        }
    }
}

void showTodayReminder(const string& reminder, const string& todayDate)
{
    sf::RenderWindow reminderWindow(sf::VideoMode(400, 200), "today's reminder");

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        cerr << "폰트 파일을 불러오는 데 실패했습니다." << endl;
        return;
    }

    sf::Text reminderText;
    reminderText.setFont(font);
    reminderText.setString(reminder);
    reminderText.setCharacterSize(24);
    reminderText.setFillColor(sf::Color::Black);
    reminderText.setPosition(20, 80);

    sf::Text dateText;
    dateText.setFont(font);
    dateText.setString(todayDate + "\nToday's reminder");
    dateText.setCharacterSize(18);
    dateText.setFillColor(sf::Color::Black);
    dateText.setPosition(20, 20);

    while (reminderWindow.isOpen())
    {
        sf::Event event;
        while (reminderWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                reminderWindow.close();
        }

        reminderWindow.clear(sf::Color::White);
        reminderWindow.draw(reminderText);
        reminderWindow.draw(dateText);
        reminderWindow.display();
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
    loadReminders(reminders, "reminders.txt");

    if (reminders.count(currentDay))
    {
        string todayDate = to_string(currentYear) + "-" + to_string(currentMonth + 1) + "-" + to_string(currentDay);
        showTodayReminder(reminders[currentDay], todayDate);
    }

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        cerr << "폰트 파일을 불러오는 데 실패했습니다." << endl;
        return 1;
    }

    string inputText;
    bool inputActive = false;
    bool deleteActive = false;
    int reminderDay = 0;
    string displayReminder;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                int col = (x - 50) / 100;
                int row = (y - 100) / 50;
                int day = col + row * 7 - firstWeekDayOfMonth + 1;
                if (day >= 1 && day <= numberOfDays)
                {
                    reminderDay = day;
                    displayReminder = reminders.count(day) ? reminders[day] : "";
                    inputActive = true;
                    inputText.clear();
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (inputActive && event.key.code == sf::Keyboard::Escape)
                {
                    inputActive = false;
                }
                else if (inputActive && event.key.code == sf::Keyboard::Enter)
                {
                    reminders[reminderDay] = inputText;
                    saveReminders(reminders, "reminders.txt");
                    inputActive = false;
                }
                else if (deleteActive && event.key.code == sf::Keyboard::Enter)
                {
                    reminders.erase(reminderDay);
                    saveReminders(reminders, "reminders.txt");
                    deleteActive = false;
                }
                else if (event.key.code == sf::Keyboard::N)
                {
                    inputActive = true;
                    deleteActive = false;
                    inputText.clear();
                }
                else if (event.key.code == sf::Keyboard::D)
                {
                    deleteActive = true;
                    inputActive = false;
                    inputText.clear();
                }
            }

            if (inputActive && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b' && !inputText.empty())
                {
                    inputText.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\b')
                {
                    inputText += static_cast<char>(event.text.unicode);
                }
            }
        }

        window.clear(sf::Color::White);
        drawCalendar(window, weekDays, firstWeekDayOfMonth, numberOfDays, reminders, currentDay);

        if (inputActive)
        {
            sf::RectangleShape inputBox(sf::Vector2f(700, 50));
            inputBox.setFillColor(sf::Color::White);
            inputBox.setOutlineColor(sf::Color::Black);
            inputBox.setOutlineThickness(1);
            inputBox.setPosition(50, 550);
            window.draw(inputBox);

            sf::Text inputTextDisplay(inputText, font, 24);
            inputTextDisplay.setFillColor(sf::Color::Black);
            inputTextDisplay.setPosition(60, 560);
            window.draw(inputTextDisplay);

            if (!displayReminder.empty())
            {
                sf::Text reminderDisplay("Reminder: " + displayReminder, font, 24);
                reminderDisplay.setFillColor(sf::Color::Red);
                reminderDisplay.setPosition(50, 500);
                window.draw(reminderDisplay);
            }
        }

        if (deleteActive)
        {
            sf::Text deleteText;
            deleteText.setFont(font);
            deleteText.setString("Press Enter to delete reminder for day " + to_string(reminderDay));
            deleteText.setCharacterSize(24);
            deleteText.setFillColor(sf::Color::Red);
            deleteText.setPosition(50, 550);
            window.draw(deleteText);
        }

        window.display();
    }

    return 0;
}
