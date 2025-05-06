#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H

#include <vector>
#include <string>
#include "cars.h"

class AbstractReader
{
public:
    virtual bool isOpen() const = 0;
    virtual std::vector<Car> readAll() = 0;

    virtual ~AbstractReader(){};
};

#endif // ABSTRACTREADER_H
