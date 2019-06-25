#ifndef JSONPARSER_H
#define JSONPARSER_H
#include "json.h"

using json = nlohmann::json;

class JsonParser
{
private:
    std::string fileName;
    int size = 0;
    json file;

public:
    JsonParser(std::string name);
    int parseField(std::string fieldName);
    int parseValue(std::string fieldName, int index);
    int** parseMatrix(std::string matrixName);
    void printMatrix(int** matrix);
    std::vector<int> parseMatrixLine(int** matrix, int index);
    int getSize();
};

#endif // JSONPARSER_H
