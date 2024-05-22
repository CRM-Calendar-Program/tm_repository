#pragma once
#ifndef DATA_FINDER_H
#define DATA_FINDER_H

#include <vector>
#include <string>

class DataFinder {
public:
    // 특정 자료를 찾는 함수
    static std::vector<std::string> findData(const std::vector<std::vector<std::string>>& data, const std::string& searchValue);

    // 몇 번째 행 또는 열에 있는지 확인하는 함수
    static void findIndex(const std::vector<std::vector<std::string>>& data, const std::string& value);
};

#endif // DATA_FINDER_H
