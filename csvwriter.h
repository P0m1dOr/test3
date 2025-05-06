#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <vector>
#include <fstream>

#include "cars.h"

class CsvWriter
{
public:
    CsvWriter(std::string nameSaveFile);
    bool isOpen() const;
    void writeAll(std::vector<Car> vec);

private:
    std::ofstream fout;

};

#endif // CSVWRITER_H
