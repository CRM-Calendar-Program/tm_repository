#ifndef CSV_READER_H
#define CSV_READER_H

#include <string>
#include <vector>

class CSVReader {
private:
    std::string filename;
    std::vector<std::vector<std::string>> data;

    void readCSV();

public:
    CSVReader(const std::string& filename);

    std::vector<std::vector<std::string>> getData() const;
};

#endif // CSV_READER_H
