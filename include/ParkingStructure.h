#ifndef PARKINGSTRUCTURE_H
#define PARKINGSTRUCTURE_H

#include <string>
#include <iostream>
#include "Car.h"
#include "ParkingDualStack.h"
#include "ParkingDoublyLinkedList.h"
#include "ParkingInterfaces.h"

// 停车场顶层管理类
// 组合 ParkingDualStack（栈内车辆）和 ParkingDoublyLinkedList（便道车辆）
// 提供统一的入场、离场、计费、查询、加时、改价接口
class ParkingStructure {
    private:
        ParkingDualStack* pds;                // 栈内车辆管理器
        ParkingDoublyLinkedList* pdll;        // 便道车辆管理器

        double unitPrice;                     // 每小时单价（元）

        int size;                             // 停车场总容量
        int numberForParkingDualStack;        // 栈内车辆数（冗余记录）
        int numberForDoublyLinkedList;        // 便道车辆数（冗余记录）

    public:
        // 构造函数：指定总容量和单价
        ParkingStructure(int size, double unitPrice=1) {
            this->size = size;
            this->unitPrice = unitPrice;

            pds = new ParkingDualStack(this->size);
            pdll = new ParkingDoublyLinkedList();

            this->numberForParkingDualStack = 0;
            this->numberForDoublyLinkedList = 0;
        }

        // 拷贝构造函数
        ParkingStructure(const ParkingStructure& otherObj) {
            this->size = otherObj.size;
            this->unitPrice = otherObj.unitPrice;

            this->pds = new ParkingDualStack(*otherObj.pds);
            this->pdll = new ParkingDoublyLinkedList(*otherObj.pdll);

            this->numberForParkingDualStack = otherObj.numberForParkingDualStack;
            this->numberForDoublyLinkedList = otherObj.numberForDoublyLinkedList;
        }

        // 赋值运算符
        ParkingStructure& operator= (const ParkingStructure& otherObj) {
            if (this == &otherObj) {
                return *this;
            }

            delete pds;
            delete pdll;

            this->size = otherObj.size;
            this->unitPrice = otherObj.unitPrice;

            this->pds = new ParkingDualStack(*otherObj.pds);
            this->pdll = new ParkingDoublyLinkedList(*otherObj.pdll);

            this->numberForParkingDualStack = otherObj.numberForParkingDualStack;
            this->numberForDoublyLinkedList = otherObj.numberForDoublyLinkedList;

            return *this;
        }

        // 析构函数：释放停车场和便道对象
        ~ParkingStructure() {
            delete pds;
            delete pdll;
        }

        // 获取停车场总容量
        int getSize() {
            return this->size;
        }

        // 车辆入场
        // 栈未满时入栈（记录入场时间），栈满时进入便道等待
        bool insertPark (Car& insertCar) {
            if(!pds->isFull()) {
                pds->pushDualStack(insertCar);      // 有空位，入栈
            } else {
                pdll->push(insertCar);              // 已满，入便道
            }
            return true;
        }

        // 车辆离场（核心逻辑）
        // 1. 先在栈中查找，找到则离场，便道第一辆车补入栈
        // 2. 栈中未找到，在便道链表中查找
        // 3. 均未找到返回 false
        bool leavePark(string key, Car& leftCar) {
            bool isExistInStack = false;
            isExistInStack = pds->popDualStack(key, leftCar); // 从栈中查找并弹出
            if(isExistInStack && (!pdll->isEmpty())) {
                // 便道第一辆车补入栈
                Car* targetCar = new Car("");
                pdll->get(0, *targetCar);
                pdll->erase(0);
                pds->pushDualStack(*targetCar);
                delete targetCar;
                return true;
            } else {
                // 在便道链表中查找并删除
                int indexOfCar = pdll->indexOfString(key);
                if(indexOfCar >= 0) {
                    pdll->get(indexOfCar, leftCar);
                    pdll->erase(indexOfCar);
                    return true;
                }
                return false;
            }

            return false;
        }

        // 显示停车场状态（控制台用）
        void showPark() const {
            cout << "Stack: \n";
            pds->showLeftStack();
            cout << "\nLinkedList: \n";
            pdll->showDoublyLinkedList();
        }

        // 调试加时：给栈内和便道所有车辆增加额外时间
        bool addExtraTime(int seconds=0, int minutes=0, int hours=0) {
            this->pds->addExtraTime(seconds, minutes, hours);
            this->pdll->addExtraTime(seconds, minutes, hours);
            return true;
        }

        // 设置每小时单价
        void setUnitPrice(double unitPrice) {
            this->unitPrice = unitPrice;
        }

        // 获取每小时单价
        double getUnitPrice() {
            return this->unitPrice;
        }

        // 计算信息接口中所有车辆的费用
        // 公式：price = totalTime × unitPrice ÷ 3600
        void calculateTotalPrice(ParkingInterface& pi) {
            for(int i=0; i<pi.size; i++) {
                pi.container[i].price = (double)pi.container[i].totalTime * this->unitPrice / 3600;
            }
        }

        // 计算指定时间的费用
        double calculatePrice(time_t time) {
            return time * this->unitPrice / 3600;
        }

        // 获取栈内车辆信息接口（包含计算好的费用）
        ParkingDualStackInterface& getStackMessage() {
            ParkingDualStackInterface* msg = new ParkingDualStackInterface(this->pds->getParkingMessage());
            this->calculateTotalPrice(*msg);
            return *msg;
        }

        // 获取便道车辆信息接口（费用强制为 0）
        ParkingDoublyLinkedListInterface& getLinkedListMessage() {
            ParkingDoublyLinkedListInterface* msg = new ParkingDoublyLinkedListInterface(this->pdll->getParkingMessage());
            for(int i=0; i<msg->size; i++) {
                msg->container[i].price = 0;
            }
            return *msg;
        }

        // 判断车牌号是否已存在（遍历栈和便道）
        bool isExist(string key) {
            ParkingDualStackInterface& stackMsg = this->getStackMessage();
            ParkingDoublyLinkedListInterface& linkedListMsg = this->getLinkedListMessage();

            bool isExist = false;
            for(int i=0; i<stackMsg.size; i++) {
                if(stackMsg.container[i].id == key) {
                    isExist = true;
                    break;
                }
            }
            if (!isExist) {
                for (int i=0; i<linkedListMsg.size; i++) {
                    if(linkedListMsg.container[i].id == key) {
                        isExist = true;
                        break;
                    }
                }
            }
            return isExist;
        }

};

#endif
