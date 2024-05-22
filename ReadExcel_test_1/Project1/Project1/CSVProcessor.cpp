#include "CSVProcessor.h"
#include "CSVReader.h"
#include "DataFinder.h"
#include <iostream>

using namespace std;

CSVProcessor::CSVProcessor(const string& filename) : filename(filename) {}

void CSVProcessor::processInput(const string& searchValue) {
    CSVReader csvReader(filename);
    vector<vector<string>> data = csvReader.getData();

    // 특정 자료를 찾는 예제
    vector<string> searchData = DataFinder::findData(data, searchValue);
    if (!searchData.empty()) {
        cout << "Data found: ";
        for (const auto& data : searchData) {
            cout << data << " ";
        }
        cout << endl;
    }
    else {
        cout << "Data not found." << endl;
    }

    // 몇 번째 행 또는 열에 있는지 확인하는 예제
    DataFinder::findIndex(data, searchValue);
}
