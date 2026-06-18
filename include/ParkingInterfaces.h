#ifndef PARKINGINTERFACES_H
#define PARKINGINTERFACES_H

#include <string>

using namespace std;

// 车辆信息结构体
// 用于在业务层和界面层之间传输车辆数据
struct CarInfo {
    string id;           // 车牌号
    int totalTime;       // 累计停车时间（秒）
    double price;        // 应缴费用（元）

    CarInfo(string id="", int totalTime=0) {
        this->id = id;
        this->totalTime = totalTime;
    }
};

// 停车场信息接口基类
// 包含动态数组 container，存储 CarInfo 对象
struct ParkingInterface {
    CarInfo* container;  // CarInfo 动态数组
    int size;            // 数组大小

    // 构造函数：分配指定大小的数组
    ParkingInterface(int size) {
        this->size = size;
        this->container = new CarInfo[this->size];
    }

    // 拷贝构造函数
    ParkingInterface(const ParkingInterface& otherObj) {
        this->size = otherObj.size;
        this->container = new CarInfo[this->size];

        for(int i=0; i<this->size; i++) {
            this->container[i] = otherObj.container[i];
        }
    }

    // 赋值运算符
    ParkingInterface& operator= (const ParkingInterface& otherObj) {
        if (this == & otherObj) {
            return *this;
        }

        delete [] this->container;

        this->size = otherObj.size;
        this->container = new CarInfo[this->size];

        for(int i=0; i<this->size; i++) {
            this->container[i] = otherObj.container[i];
        }

        return *this;
    }

    // 析构函数：释放动态数组
    ~ParkingInterface() {
        delete [] this->container;
    }

};

// 共享双栈信息接口（继承自 ParkingInterface）
struct ParkingDualStackInterface : public ParkingInterface {
    const string _STRUCT_NAME = "Dual Stack";

    ParkingDualStackInterface(int size) : ParkingInterface(size) {}
};

// 双向链表信息接口（继承自 ParkingInterface）
struct ParkingDoublyLinkedListInterface : public ParkingInterface {
    const string _STRUCT_NAME = "Doubly Linked List";

    ParkingDoublyLinkedListInterface(int size) : ParkingInterface(size) {}
};

#endif
