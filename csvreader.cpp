#include "csvreader.h"

std::vector<std::string> split_line(const std::string &str, char delim) {

    std::vector<std::string> tokens;

    if (!str.empty()) {
        size_t start = 0, end;
        do {
            tokens.push_back(str.substr(start, (str.find(delim, start) - start)));
            end = str.find(delim, start);
            start = end + 1;
        } while (end != std::string::npos);
    }

    return tokens;
}


CsvReader::CsvReader(const std::string& filename)
{
    fin = std::ifstream(filename);
}

bool CsvReader::isOpen()
{
    return fin.is_open();
}

std::vector<Car> CsvReader::readAll()
{
    std::vector<Car> cars;
    while (!fin.eof()) {
        std::string line;
        std::getline(fin, line);

        auto tokens = split_line(line, ';');
        if (tokens.size() == 4) {
            Car c;
            c.id = std::strtoll(tokens[0].data(), nullptr, 10);
            c.name = tokens[1];
            c.type = tokens[2];
            c.hp = std::strtoll(tokens[3].data(), nullptr, 10);

            cars.push_back(c);
        }
    }
    return cars;
}
