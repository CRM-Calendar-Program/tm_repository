#include <iostream>
#include <string>
#include "libxl.h"
using namespace std;
using namespace libxl;

void readExcel(const wstring& filename) {
    Book* book = xlCreateXMLBook();
    if (book->load(filename.c_str())) {
        Sheet* sheet = book->getSheet(0);
        if (sheet) {
            int rows = sheet->lastRow();
            int cols = sheet->lastCol();

            cout << "Contents of the Excel file:" << endl;
            int numRowsToRead = min(3, rows); // 처음 세 개의 행만 읽어옴
            for (int row = 0; row < numRowsToRead; ++row) {
                for (int col = 0; col < cols; ++col) {
                    wstring data = sheet->readStr(row + 1, col + 1); // 셀은 1부터 시작하므로 인덱스를 1씩 증가시킴
                    wcout << data << "\t";
                }
                cout << endl;
            }
        }
        else {
            cerr << "Failed to retrieve sheet." << endl;
        }
    }
    else {
        cerr << "Failed to load Excel file." << endl;
    }
    book->release();
}

int main() {
    wstring filename = L"test1.xlsx"; // 엑셀 파일 이름의 데이터 형식을 std::wstring으로 변경
    readExcel(filename); // 엑셀 파일의 데이터 읽어오기

    return 0;
}
