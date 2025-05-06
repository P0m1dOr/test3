#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <fstream>

#include "cars.h"
#include "abstractreader.h"

class CsvReader : public AbstractReader
{
public:
    CsvReader(const std::string& filename);

    bool isOpen() const override;

    std::vector<Car> readAll() override;

private:
    std::ifstream fin;
};

#endif // CSVREADER_H
