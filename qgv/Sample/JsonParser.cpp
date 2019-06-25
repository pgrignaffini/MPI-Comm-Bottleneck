#include "JsonParser.h"
#include <fstream>

JsonParser::JsonParser(std::string fileName)
{
    std::ifstream in(fileName);
    if (in.is_open())
    {
        in >> file;
        size = file["size"];
    }
    else
    {
        std::cout << "Unable to open the file" << std::endl;
        file = NULL;
    }
}

int JsonParser::parseField(std::string fieldName)
{
    return file[fieldName];
}

int JsonParser::parseValue(std::string fieldName, int index)
{
    return file[fieldName][index];
}

int** JsonParser::parseMatrix(std::string matrixName)
{
    int** parsedMatrix;
    parsedMatrix = new int*[size];

    for (int h = 0; h < size; h++)
    {
        parsedMatrix[h] = new int[size];
        for (int w = 0; w < size; w++)
        {
            parsedMatrix[h][w] = file[matrixName][h][w];
        }
     }

    return parsedMatrix;
}

void JsonParser::printMatrix(int **matrix)
{
    for (int h = 0; h < size; h++)
    {
        for (int w = 0; w < size; w++)
        {
            std::cout << matrix[h][w] << " ";
        }

        std::cout << std::endl;
    }
}

int JsonParser::getSize()
{
    return size;
}

std::vector<int> JsonParser::parseMatrixLine(int** matrix, int index)
{
    std::vector<int> line;
    for (int i = 0; i < size; i++)
        line.push_back(matrix[index][i]);

    return line;
}
