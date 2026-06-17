#ifndef PARKINGFILEMANAGER_H
#define PARKINGFILEMANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include "Car.h"
#include "ParkingStructure.h"
#include "ParkingInterfaces.h"

struct ImportCarData {
    std::string plate;
    time_t totalTime;
};

class ParkingFileManager {
public:
    static bool exportToJson(const std::string& filePath, ParkingStructure& park, const std::string& parkName) {
        std::ofstream file(filePath);
        if (!file.is_open()) return false;

        ParkingDualStackInterface& stackMsg = park.getStackMessage();
        ParkingDoublyLinkedListInterface& listMsg = park.getLinkedListMessage();

        file << "{\n";
        file << "    \"parkName\": \"" << parkName << "\",\n";
        file << "    \"capacity\": " << park.getSize() << ",\n";
        file << "    \"unitPrice\": " << park.getUnitPrice() << ",\n";
        file << "    \"cars\": [\n";

        bool first = true;

        for (int i = 0; i < stackMsg.size; i++) {
            if (!first) file << ",\n";
            file << "        {\n";
            file << "            \"plate\": \"" << stackMsg.container[i].id << "\",\n";
            file << "            \"totalTime\": " << stackMsg.container[i].totalTime << "\n";
            file << "        }";
            first = false;
        }

        for (int i = 0; i < listMsg.size; i++) {
            if (!first) file << ",\n";
            file << "        {\n";
            file << "            \"plate\": \"" << listMsg.container[i].id << "\",\n";
            file << "            \"totalTime\": 0\n";
            file << "        }";
            first = false;
        }

        file << "\n    ]\n";
        file << "}\n";

        file.close();
        return true;
    }

    static bool importFromJson(const std::string& filePath, std::string& parkName,
                               int& capacity, double& unitPrice,
                               std::vector<ImportCarData>& cars) {
        std::ifstream file(filePath);
        if (!file.is_open()) return false;

        std::string line;
        bool foundName = false, foundCapacity = false, foundPrice = false;

        while (std::getline(file, line)) {
            size_t pos;

            // 查找 parkName
            pos = line.find("\"parkName\"");
            if (pos != std::string::npos) {
                size_t start = line.find("\"", pos + 10);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    parkName = line.substr(start + 1, end - start - 1);
                    foundName = true;
                }
            }

            // 查找 capacity
            pos = line.find("\"capacity\"");
            if (pos != std::string::npos) {
                size_t colon = line.find(":", pos + 9);
                if (colon != std::string::npos) {
                    capacity = std::stoi(line.substr(colon + 1));
                    if (capacity < 0) capacity = 5;
                    foundCapacity = true;
                }
            }

            // 查找 unitPrice
            pos = line.find("\"unitPrice\"");
            if (pos != std::string::npos) {
                size_t colon = line.find(":", pos + 10);
                if (colon != std::string::npos) {
                    unitPrice = std::stod(line.substr(colon + 1));
                    if (unitPrice < 0) unitPrice = 1;
                    foundPrice = true;
                }
            }

            // 查找 cars 数组中的 plate 和 totalTime
            pos = line.find("\"plate\"");
            if (pos != std::string::npos) {
                size_t start = line.find("\"", pos + 7);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    ImportCarData carData;
                    carData.plate = line.substr(start + 1, end - start - 1);

                    // 读取同一对象中的 totalTime（在下一行或同一行）
                    if (std::getline(file, line)) {
                        size_t tpos = line.find("\"totalTime\"");
                        if (tpos != std::string::npos) {
                            size_t colon = line.find(":", tpos + 11);
                            if (colon != std::string::npos) {
                                carData.totalTime = std::stoi(line.substr(colon + 1));
                            }
                        }
                    }
                    cars.push_back(carData);
                }
            }
        }

        file.close();

        if (!foundName) parkName = "我的停车场";
        if (!foundCapacity) capacity = 5;
        if (!foundPrice) unitPrice = 1.0;

        return foundName || foundCapacity || foundPrice || !cars.empty();
    }
};

#endif
