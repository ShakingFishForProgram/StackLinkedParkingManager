#ifndef PARKINGDUALSTACK_H
#define PARKINGDUALSTACK_H

#include "DualStack.h"
#include "Car.h"
#include "ParkingInterfaces.h"

// 停车场双栈业务类
// 继承 DualStack<Car>，封装停车场内车辆的具体业务逻辑：
//   - 入场时自动记录时间戳
//   - 离场时自动计算停车时间
//   - 支持调试加时
//   - 生成信息查询接口
class ParkingDualStack : public DualStack<Car> {
    public:
        // 构造函数：传入双栈容量
        ParkingDualStack(int size) : DualStack<Car> (size){}

        // 拷贝构造函数
        ParkingDualStack(const ParkingDualStack& otherObj) : DualStack<Car>(otherObj) {}

        // 赋值运算符
        ParkingDualStack& operator= (const ParkingDualStack& otherObj) {
            if(this == &otherObj) {
                return *this;
            }

            DualStack<Car>::operator=(otherObj);
            return *this;
        }

        // 车辆入场：先记录入场时间戳，再压入左栈
        bool pushDualStack(Car& obj) {
            obj.inPark();                                       // 记录入场时间
            if (DualStack<Car>::pushDualStack(obj)) {
                return true;
            } else {
                return false;
            }
        }

        // 车辆离场：从左栈搜索弹出，再记录离场时间戳
        bool popDualStack(string value, Car& obj) {
            if (DualStack<Car>::popDualStack(value, obj)) {
                obj.outPark();                                  // 记录离场时间
                return true;
            } else {
                return false;
            }
        }

        // 生成栈内车辆信息接口
        // 遍历左栈车辆，获取车牌号和累计停车时间（含调试加时）
        ParkingDualStackInterface& getParkingMessage() {
            ParkingDualStackInterface* pdsi = new ParkingDualStackInterface(this->leftNumber);
            for(int i=0; i<this->leftNumber; i++) {
                pdsi->container[i].id = this->container[i].getId();
                this->container[i].markMaintainingTimeStamp();  // 更新查询时间戳
                pdsi->container[i].totalTime = this->container[i].getMaintainingTime();
            }

            return *pdsi;
        }

        // 调试加时：给左栈和右栈中所有车辆增加额外时间
        bool addExtraTime(int seconds=0, int minutes=0, int hours=0) {
            int extraTime = 3600*hours + 60*minutes + seconds;

            // 遍历左栈车辆
            for(int i=0; i<this->leftNumber; i++) {
                int originalTime = this->container[i].getExtraTime();
                this->container[i].setExtraTime(originalTime + extraTime);
            }

            // 遍历右栈车辆（从数组尾部开始）
            for(int i=0; i<this->rightNumber; i++) {
                int originalTime = this->container[this->size-1-i].getExtraTime();
                this->container[this->size-1-i].setExtraTime(originalTime + extraTime);
            }
            return true;
        }

};

#endif
