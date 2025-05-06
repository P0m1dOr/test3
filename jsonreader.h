#ifndef JSONREADER_H
#define JSONREADER_H

#include "abstractreader.h"
#include <fstream>
#include <vector>
#include <string>
#include "cars.h"
#include "json.hpp"

class JSONReader : public AbstractReader
{
public:
    explicit JSONReader(const std::string& filename);
    bool isOpen() const override;
    std::vector<Car> readAll() override;

private:
    std::ifstream fin;
};

#endif // JSONREADER_H
