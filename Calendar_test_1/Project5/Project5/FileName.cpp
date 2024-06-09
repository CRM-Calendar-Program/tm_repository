#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

// �ش� ���� �ϼ��� ����ϴ� �Լ�
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

// �޷��� �׸��� �Լ�
void drawCalendar(sf::RenderWindow& window, const vector<string>& weekDays, const string& currentMonthStr, int firstWeekDayOfMonth, int numberOfDays, const unordered_map<string, string>& reminders, int today, const string& currentYearMonth)
{
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        cerr << "��Ʈ ������ �ҷ����� �� �����߽��ϴ�." << endl;
        return;
    }

    int xOffset = 50, yOffset = 50;
    int cellWidth = 100, cellHeight = 50;

    // ���� ���� ǥ��
    sf::Text monthText;
    monthText.setFont(font);
    monthText.setString(currentMonthStr);
    monthText.setCharacterSize(30);
    monthText.setFillColor(sf::Color::Black);
    monthText.setPosition(50, 10);
    window.draw(monthText);

    // ���� ����� �׸�
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

    // ���ڸ� �׸��� �����δ��� �ִ� ��¥�� �ٸ� �������� ǥ��
    for (int d = 1; d <= numberOfDays; d++)
    {
        sf::Text dayText;
        dayText.setFont(font);
        dayText.setString(to_string(d));
        dayText.setCharacterSize(24);

        string key = currentYearMonth + "-" + (d < 10 ? "0" : "") + to_string(d);

        if (d == today)
        {
            dayText.setFillColor(sf::Color::Green);
        }
        else if (reminders.count(key))
        {
            dayText.setFillColor(sf::Color::Blue);
        }
        else
        {
            dayText.setFillColor(sf::Color::Black);
        }

        dayText.setPosition(xOffset + (tempBreak - 1) * cellWidth, yOffset + cellHeight);

        if (reminders.count(key))
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

    // ����Ű ������ ȭ�鿡 ǥ��
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Insert: Create Reminder  Delete: Delete Reminder  ESC: Cancel");
    instructions.setCharacterSize(14);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(50, window.getSize().y - 220);
    window.draw(instructions);
}

// �����δ��� ���Ͽ� �����ϴ� �Լ�
void saveReminders(const unordered_map<string, string>& reminders, const string& filename)
{
    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
        return;
    }

    for (const auto& reminder : reminders)
    {
        outFile << reminder.first << ":" << reminder.second << endl;
    }
}

// ���Ͽ��� �����δ��� �ҷ����� �Լ�
void loadReminders(unordered_map<string, string>& reminders, const string& filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        size_t colonPos = line.find(':');
        if (colonPos != string::npos)
        {
            string date = line.substr(0, colonPos);
            string message = line.substr(colonPos + 1);
            reminders[date] = message;
        }
    }
}

// ������ �����δ��� ǥ���ϴ� �Լ�
void showTodayReminder(const string& reminder, const string& todayDate)
{
    sf::RenderWindow reminderWindow(sf::VideoMode(400, 200), "today's reminder");

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        cerr << "��Ʈ ������ �ҷ����� �� �����߽��ϴ�." << endl;
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

    // ���� ��¥�� �ҷ��� �ʱ�ȭ
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

    // �����δ��� ���Ͽ��� �ҷ���
    unordered_map<string, string> reminders;
    loadReminders(reminders, "reminders.txt");

    string currentYearMonth = to_string(currentYear) + "-" + (currentMonth + 1 < 10 ? "0" : "") + to_string(currentMonth + 1);

    // ���� �����δ��� �ִ� ��� ǥ��
    string todayKey = currentYearMonth + "-" + (currentDay < 10 ? "0" : "") + to_string(currentDay);
    if (reminders.count(todayKey))
    {
        string todayDate = to_string(currentYear) + "-" + (currentMonth + 1 < 10 ? "0" : "") + to_string(currentMonth + 1) + "-" + (currentDay < 10 ? "0" : "") + to_string(currentDay);
        showTodayReminder(reminders[todayKey], todayDate);
    }

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        cerr << "��Ʈ ������ �ҷ����� �� �����߽��ϴ�." << endl;
        return 1;
    }

    string inputText;
    bool inputActive = false;
    bool deleteActive = false;
    bool displayActive = false;
    int reminderDay = 0;
    string displayReminder;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // ��¥�� Ŭ���ϸ� ����� �����δ��� ǥ��
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
                    string key = currentYearMonth + "-" + (day < 10 ? "0" : "") + to_string(day);
                    displayReminder = reminders.count(key) ? reminders[key] : "";
                    displayActive = true;
                    inputText.clear();
                }
            }

            // Ű���� �̺�Ʈ ó��
            if (event.type == sf::Event::KeyPressed)
            {
                // ESC Ű�� ������ (�Է� �� ����)��� ��� ����
                if (/*(inputActive || deleteActive) &&*/ event.key.code == sf::Keyboard::Escape)
                {
                    inputActive = false;
                    deleteActive = false;
                    displayActive = false;
                }
                // Enter Ű�� ������ �����δ� ����
                else if (inputActive && event.key.code == sf::Keyboard::Enter)
                {
                    string key = currentYearMonth + "-" + (reminderDay < 10 ? "0" : "") + to_string(reminderDay);
                    reminders[key] = inputText;
                    saveReminders(reminders, "reminders.txt");
                    inputActive = false;
                    displayActive = false;
                }
                // Enter Ű�� ������ �����δ� ����
                else if (deleteActive && event.key.code == sf::Keyboard::Enter)
                {
                    string key = currentYearMonth + "-" + (reminderDay < 10 ? "0" : "") + to_string(reminderDay);
                    reminders.erase(key);
                    saveReminders(reminders, "reminders.txt");
                    deleteActive = false;
                    displayActive = false;
                }
                // Insert Ű�� ������ �����δ� �Է� ���� ��ȯ
                else if (event.key.code == sf::Keyboard::Insert)
                {
                    inputActive = true;
                    deleteActive = false;
                    inputText.clear();
                }
                // Delete Ű�� ������ �����δ� ���� ���� ��ȯ
                else if (event.key.code == sf::Keyboard::Delete)
                {
                    deleteActive = true;
                    inputActive = false;
                    inputText.clear();
                }
            }

            // �Է� ��忡�� �ؽ�Ʈ �Է� ó��
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
        drawCalendar(window, weekDays, months[currentMonth] + " " + to_string(currentYear), firstWeekDayOfMonth, numberOfDays, reminders, currentDay, currentYearMonth);

        // �Է� â(�簢��)�� ȭ�鿡 ǥ�� inputActive ���
        if (inputActive)
        {
            //insertŰ�� ������ �ؽ�Ʈ ǥ��
            sf::Text insertText;
            insertText.setFont(font);
            insertText.setString("Press Enter to input reminder for day " + to_string(reminderDay));
            insertText.setCharacterSize(24);
            insertText.setFillColor(sf::Color::Red);
            insertText.setPosition(50, window.getSize().y - 210);
            window.draw(insertText);

            // �Է� ���� �׸���
            sf::RectangleShape inputBox(sf::Vector2f(700, 100));
            inputBox.setFillColor(sf::Color::White);
            inputBox.setOutlineColor(sf::Color::Black);
            inputBox.setOutlineThickness(1);
            inputBox.setPosition(50, window.getSize().y - 150);
            window.draw(inputBox);

            // �Է� �ؽ�Ʈ â ��ġ
            sf::Text inputTextDisplay(inputText, font, 24);
            inputTextDisplay.setFillColor(sf::Color::Black);
            inputTextDisplay.setPosition(60, window.getSize().y - 150);
            window.draw(inputTextDisplay);

            // ���õ� ��¥�� �����δ��� �ִ� ��� ǥ��
            displayActive = true;
        }

        // ���� ��� �޽��� ǥ�� deleteActive ���
        if (deleteActive)
        {
            sf::Text deleteText;
            deleteText.setFont(font);
            deleteText.setString("Press Enter to delete reminder for day " + to_string(reminderDay));
            deleteText.setCharacterSize(24);
            deleteText.setFillColor(sf::Color::Red);
            deleteText.setPosition(50, window.getSize().y - 210);
            window.draw(deleteText);

            // ���õ� ��¥�� �����δ��� �ִ� ��� ǥ��
            displayActive = true;
        }

        // �����δ��� ������ ȭ�鿡 ǥ�� displayActivate ���
        if (displayActive) {
            if (!displayReminder.empty())
            {
                sf::Text reminderDisplay("Reminder: " + displayReminder, font, 24);
                reminderDisplay.setFillColor(sf::Color::Black);
                reminderDisplay.setPosition(50, window.getSize().y - 185);
                window.draw(reminderDisplay);
            }
        }

        window.display();
    }
    //system ("python main.py");
    return 0;
}
