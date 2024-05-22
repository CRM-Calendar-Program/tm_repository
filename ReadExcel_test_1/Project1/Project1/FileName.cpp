/*
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class CSVReader {
private:
    string filename;
    vector<vector<string>> data;

    // CSV 파일을 읽는 함수
    void readCSV() {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Unable to open file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream lineStream(line);
            string cell;
            vector<string> rowData;

            while (getline(lineStream, cell, ',')) {
                rowData.push_back(cell);
            }

            data.push_back(rowData);
        }

        file.close();
    }

public:
    CSVReader(const string& filename) : filename(filename) {
        readCSV();
    }

    // 데이터를 반환하는 함수
    vector<vector<string>> getData() const {
        return data;
    }
};

class DataFinder {
public:
    // 특정 자료를 찾는 함수
    static vector<string> findData(const vector<vector<string>>& data, const string& searchValue) {
        vector<string> result;
        for (const auto& row : data) {
            for (const auto& cell : row) {
                if (cell == searchValue) {
                    result = row;
                    return result;
                }
            }
        }
        return result; // 찾지 못했을 경우 빈 벡터 반환
    }

    // 몇 번째 행 또는 열에 있는지 확인하는 함수
    static void findIndex(const vector<vector<string>>& data, const string& value) {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                if (data[i][j] == value) {
                    cout << "Value '" << value << "' found at Row: " << i + 1 << ", Column: " << j + 1 << endl;
                    return;
                }
            }
        }
        cout << "Value '" << value << "' not found in the CSV." << endl;
    }
};

int main() {
    // 현재 실행 파일이 위치한 디렉토리
    string currentDirectory = "./";
    string filename = currentDirectory + "test1.csv";

    CSVReader csvReader(filename);
    vector<vector<string>> data = csvReader.getData();

    // 특정 자료를 찾는 예제
    string searchValue = "김태민";
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
    DataFinder::findIndex(data, "이름");

    return 0;
}

*/