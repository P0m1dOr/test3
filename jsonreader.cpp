#include "jsonreader.h"
#include <iostream>

JSONReader::JSONReader(const std::string& filename)
{
    fin.open(filename);
}

bool JSONReader::isOpen() const
{
    return fin.is_open();
}

std::vector<Car> JSONReader::readAll()
{
    std::vector<Car> cars;

    nlohmann::json j;
    fin >> j;

    for (const auto& car_json : j["cars"]) {
        if (car_json.is_object()) {
            Car c;
            c.id = car_json.at("Id").get<int>();
            c.name = car_json.at("Model").get<std::string>();
            c.type = car_json.at("Engine type").get<std::string>();
            c.hp = car_json.at("Horsepower").get<int>();
            cars.push_back(c);
        }
    }

    fin.close();
    return cars;
}
