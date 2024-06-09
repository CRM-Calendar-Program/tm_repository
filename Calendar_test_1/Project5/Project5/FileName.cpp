#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

// 해당 월의 일수를 계산하는 함수
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

// 달력을 그리는 함수
void drawCalendar(sf::RenderWindow& window, const vector<string>& weekDays, const string& currentMonthStr, int firstWeekDayOfMonth, int numberOfDays, const unordered_map<string, string>& reminders, int today, const string& currentYearMonth)
{
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        cerr << "폰트 파일을 불러오는 데 실패했습니다." << endl;
        return;
    }

    int xOffset = 50, yOffset = 50;
    int cellWidth = 100, cellHeight = 50;

    // 현재 월을 표시
    sf::Text monthText;
    monthText.setFont(font);
    monthText.setString(currentMonthStr);
    monthText.setCharacterSize(30);
    monthText.setFillColor(sf::Color::Black);
    monthText.setPosition(50, 10);
    window.draw(monthText);

    // 요일 헤더를 그림
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

    // 일자를 그리며 리마인더가 있는 날짜는 다른 색상으로 표시
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

    // 단축키 설명을 화면에 표시
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Insert: Create Reminder  Delete: Delete Reminder  ESC: Cancel");
    instructions.setCharacterSize(14);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(50, window.getSize().y - 220);
    window.draw(instructions);
}

// 리마인더를 파일에 저장하는 함수
void saveReminders(const unordered_map<string, string>& reminders, const string& filename)
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

// 파일에서 리마인더를 불러오는 함수
void loadReminders(unordered_map<string, string>& reminders, const string& filename)
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
            string date = line.substr(0, colonPos);
            string message = line.substr(colonPos + 1);
            reminders[date] = message;
        }
    }
}

// 오늘의 리마인더를 표시하는 함수
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

    // 현재 날짜를 불러와 초기화
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

    // 리마인더를 파일에서 불러옴
    unordered_map<string, string> reminders;
    loadReminders(reminders, "reminders.txt");

    string currentYearMonth = to_string(currentYear) + "-" + (currentMonth + 1 < 10 ? "0" : "") + to_string(currentMonth + 1);

    // 오늘 리마인더가 있는 경우 표시
    string todayKey = currentYearMonth + "-" + (currentDay < 10 ? "0" : "") + to_string(currentDay);
    if (reminders.count(todayKey))
    {
        string todayDate = to_string(currentYear) + "-" + (currentMonth + 1 < 10 ? "0" : "") + to_string(currentMonth + 1) + "-" + (currentDay < 10 ? "0" : "") + to_string(currentDay);
        showTodayReminder(reminders[todayKey], todayDate);
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

            // 날짜를 클릭하면 저장된 리마인더를 표시
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

            // 키보드 이벤트 처리
            if (event.type == sf::Event::KeyPressed)
            {
                // ESC 키를 누르면 (입력 및 삭제)모든 모드 종료
                if (/*(inputActive || deleteActive) &&*/ event.key.code == sf::Keyboard::Escape)
                {
                    inputActive = false;
                    deleteActive = false;
                    displayActive = false;
                }
                // Enter 키를 누르면 리마인더 저장
                else if (inputActive && event.key.code == sf::Keyboard::Enter)
                {
                    string key = currentYearMonth + "-" + (reminderDay < 10 ? "0" : "") + to_string(reminderDay);
                    reminders[key] = inputText;
                    saveReminders(reminders, "reminders.txt");
                    inputActive = false;
                    displayActive = false;
                }
                // Enter 키를 누르면 리마인더 삭제
                else if (deleteActive && event.key.code == sf::Keyboard::Enter)
                {
                    string key = currentYearMonth + "-" + (reminderDay < 10 ? "0" : "") + to_string(reminderDay);
                    reminders.erase(key);
                    saveReminders(reminders, "reminders.txt");
                    deleteActive = false;
                    displayActive = false;
                }
                // Insert 키를 누르면 리마인더 입력 모드로 전환
                else if (event.key.code == sf::Keyboard::Insert)
                {
                    inputActive = true;
                    deleteActive = false;
                    inputText.clear();
                }
                // Delete 키를 누르면 리마인더 삭제 모드로 전환
                else if (event.key.code == sf::Keyboard::Delete)
                {
                    deleteActive = true;
                    inputActive = false;
                    inputText.clear();
                }
            }

            // 입력 모드에서 텍스트 입력 처리
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

        // 입력 창(사각형)을 화면에 표시 inputActive 기능
        if (inputActive)
        {
            //insert키를 누르면 텍스트 표시
            sf::Text insertText;
            insertText.setFont(font);
            insertText.setString("Press Enter to input reminder for day " + to_string(reminderDay));
            insertText.setCharacterSize(24);
            insertText.setFillColor(sf::Color::Red);
            insertText.setPosition(50, window.getSize().y - 210);
            window.draw(insertText);

            // 입력 상자 그리기
            sf::RectangleShape inputBox(sf::Vector2f(700, 100));
            inputBox.setFillColor(sf::Color::White);
            inputBox.setOutlineColor(sf::Color::Black);
            inputBox.setOutlineThickness(1);
            inputBox.setPosition(50, window.getSize().y - 150);
            window.draw(inputBox);

            // 입력 텍스트 창 위치
            sf::Text inputTextDisplay(inputText, font, 24);
            inputTextDisplay.setFillColor(sf::Color::Black);
            inputTextDisplay.setPosition(60, window.getSize().y - 150);
            window.draw(inputTextDisplay);

            // 선택된 날짜에 리마인더가 있는 경우 표시
            displayActive = true;
        }

        // 삭제 모드 메시지 표시 deleteActive 기능
        if (deleteActive)
        {
            sf::Text deleteText;
            deleteText.setFont(font);
            deleteText.setString("Press Enter to delete reminder for day " + to_string(reminderDay));
            deleteText.setCharacterSize(24);
            deleteText.setFillColor(sf::Color::Red);
            deleteText.setPosition(50, window.getSize().y - 210);
            window.draw(deleteText);

            // 선택된 날짜에 리마인더가 있는 경우 표시
            displayActive = true;
        }

        // 리마인더가 있으면 화면에 표시 displayActivate 기능
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
