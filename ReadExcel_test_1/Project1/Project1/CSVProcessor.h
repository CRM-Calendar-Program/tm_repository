#ifndef CSV_PROCESSOR_H
#define CSV_PROCESSOR_H

#include <string>

class CSVProcessor {
private:
    std::string filename;

public:
    CSVProcessor(const std::string& filename);

    // 사용자 입력을 처리하는 함수
    void processInput(const std::string& searchValue);
};

#endif // CSV_PROCESSOR_H
