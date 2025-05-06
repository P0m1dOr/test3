#include "csvwriter.h"

CsvWriter::CsvWriter(std::string saveFileName)
{
    fout = std::ofstream(saveFileName);
}

bool CsvWriter::isOpen() const
{
    return fout.is_open();
}

void CsvWriter::writeAll(std::vector<Car> cars)
{

    if(this -> isOpen())
    {
        for (const Car& c: cars) {

            fout        <<   c.id
                 << ";" << c.name
                 << ";" << c.type
                 << ";" <<   c.hp
                 <<     std::endl;
        }
        fout.close();
    }
}
