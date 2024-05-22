#include <iostream>
#include "CSVProcessor.h"

using namespace std;

int main() {
    // CSV 파일의 경로
    string filename = "test1.csv";

    // CSVProcessor 객체 생성
    CSVProcessor csvProcessor(filename);

    // 사용자 입력 받기
    cout << "Enter the value you want to search: ";
    string searchValue;
    getline(cin, searchValue);

    // 사용자 입력 처리
    csvProcessor.processInput(searchValue);

    return 0;
}
