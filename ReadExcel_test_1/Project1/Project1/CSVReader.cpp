#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

CSVReader::CSVReader(const string& filename) : filename(filename) {
    readCSV();
}

void CSVReader::readCSV() {
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

vector<vector<string>> CSVReader::getData() const {
    return data;
}
