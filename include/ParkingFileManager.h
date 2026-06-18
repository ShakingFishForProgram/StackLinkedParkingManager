#ifndef PARKINGFILEMANAGER_H
#define PARKINGFILEMANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include "Car.h"
#include "ParkingStructure.h"
#include "ParkingInterfaces.h"

// 导入车辆数据结构体
// 存储从 JSON 中解析出的车牌号和总时间
struct ImportCarData {
    std::string plate;       // 车牌号
    time_t totalTime;        // 导入时恢复的历史总时间（秒）
};

// 停车场文件管理类
// 提供静态方法，支持 JSON 格式的导入和导出
// JSON 格式：
// {
//     "parkName": "...",
//     "capacity": 5,
//     "unitPrice": 2.0,
//     "cars": [
//         { "plate": "京A001", "totalTime": 3600 },
//         { "plate": "京A002", "totalTime": 0 }
//     ]
// }
class ParkingFileManager {
public:
    // 导出停车场数据到 JSON 文件
    // 包含：名称、容量、单价、栈内车辆（含总时间）、便道车辆（时间计 0）
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

        // 写入栈内车辆
        for (int i = 0; i < stackMsg.size; i++) {
            if (!first) file << ",\n";
            file << "        {\n";
            file << "            \"plate\": \"" << stackMsg.container[i].id << "\",\n";
            file << "            \"totalTime\": " << stackMsg.container[i].totalTime << "\n";
            file << "        }";
            first = false;
        }

        // 写入便道车辆
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

    // 从 JSON 文件导入停车场数据
    // 解析后通过输出参数返回名称、容量、单价和车辆列表
    static bool importFromJson(const std::string& filePath, std::string& parkName,
                               int& capacity, double& unitPrice,
                               std::vector<ImportCarData>& cars) {
        std::ifstream file(filePath);
        if (!file.is_open()) return false;

        std::string line;
        bool foundName = false, foundCapacity = false, foundPrice = false;

        while (std::getline(file, line)) {
            size_t pos;

            // 解析 parkName
            pos = line.find("\"parkName\"");
            if (pos != std::string::npos) {
                size_t start = line.find("\"", pos + 10);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    parkName = line.substr(start + 1, end - start - 1);
                    foundName = true;
                }
            }

            // 解析 capacity
            pos = line.find("\"capacity\"");
            if (pos != std::string::npos) {
                size_t colon = line.find(":", pos + 9);
                if (colon != std::string::npos) {
                    capacity = std::stoi(line.substr(colon + 1));
                    if (capacity < 0) capacity = 5;
                    foundCapacity = true;
                }
            }

            // 解析 unitPrice
            pos = line.find("\"unitPrice\"");
            if (pos != std::string::npos) {
                size_t colon = line.find(":", pos + 10);
                if (colon != std::string::npos) {
                    unitPrice = std::stod(line.substr(colon + 1));
                    if (unitPrice < 0) unitPrice = 1;
                    foundPrice = true;
                }
            }

            // 解析 cars 数组中的 plate 和 totalTime
            pos = line.find("\"plate\"");
            if (pos != std::string::npos) {
                size_t start = line.find("\"", pos + 7);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    ImportCarData carData;
                    carData.plate = line.substr(start + 1, end - start - 1);

                    // 读取下一行的 totalTime
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

        // 未找到字段时设置默认值
        if (!foundName) parkName = "我的停车场";
        if (!foundCapacity) capacity = 5;
        if (!foundPrice) unitPrice = 1.0;

        return foundName || foundCapacity || foundPrice || !cars.empty();
    }
};

#endif
